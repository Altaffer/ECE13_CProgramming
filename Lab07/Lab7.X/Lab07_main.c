// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>

//CSE13E Support Library
#include "BOARD.h"
#include "Adc.h"
#include "Buttons.h"
#include "Oled.h"
#include "Leds.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>


// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer
#define TIMER_2HZ_RESET() (TMR1 = 0)
#define TIMER_FREQ 5
#define LONG_PRESS 1
#define Q_SECOND_25_HZ 25
// macros to init oven
#define ADC_TEMP_CONVERSION 300
#define ADC_TIME_CONVERSION 1
#define STARTING_TEMP_BAKE 350
#define STARTING_TEMP_BROIL 500
// Oven icon
#define SIZE_OVEN_RACK sizeof(OVEN_TOP_OFF) * 5
#define BUILD_LINE(x, c)                sprintf(x, "|%s%s%s%s%s|", c, c, c, c, c)
// macros to get adc values for time or temp
#define GET_ADC_TIME (AdcRead() >> 2) + ADC_TIME_CONVERSION
#define GET_ADC_TEMP (AdcRead() >> 2) + ADC_TEMP_CONVERSION
// macros to get min and sec
#define GET_MIN(x) x / 60
#define GET_SEC(x) x % 60
// Cooking states
#define BAKE  1
#define BROIL 2
#define TOAST 3
// LEDs starting value where they are all on/off
#define LEDS_START 0xFF
#define LEDS_OFF 0x00
#define LED_RATIO 8
#define LED_FE 0xFE
#define LED_FC 0xFC
#define LED_F8 0xF8
#define LED_F0 0xF0
#define LED_E0 0xE0
#define LED_C0 0xC0
#define LED_80 0x80
// for extra credit 
#define FLASH_MAX 4
#define IS_ODD(x) x % 2 // used to know if flash amount is odd or even

// **** Set any local typedefs here ****

typedef enum {
    SETUP, SELECTOR_CHANGE_PENDING, COOKING, RESET_PENDING, FLASHING
} OvenState;

typedef struct {
    // holds current state
    OvenState state;
    // this is only used to know when oven came from cooking so it resets to prev data not adc data
    OvenState prevState;
    // timing
    uint16_t time_start;
    uint16_t time_curr;
    // temp
    uint16_t temp_start;
    uint16_t temp_curr;
    // mode
    uint8_t cookmode;
    // bool to track what the user is selecting
    uint8_t selecting_time;
    // for buttons
    uint8_t ButtonState;
    // times the oven has flashed
    uint16_t flashAmount;
} OvenData;

// **** Declare any datatypes here ****

// **** Define any module-level, global, or external variables here ****
static uint16_t FRT; // global counter
static uint16_t startTime; // gets the start time of an event from frt
static uint16_t currTime; // gets the end time of an event from frt
static uint16_t elapsed; // used to check the time elapsed from start to curr
static uint8_t TIMER_TICK; // bool to tell when the 5hz timer was entered 
static uint8_t oneSecond; // signifies that a full second has passed
static uint16_t countToFive; // variable used to count to 5 in the 5hz ISR to only count at 1000ms
static uint16_t countTo25; // variable used to count to 50 in the 100hz ISR to only count at 500ms
static uint8_t ADC_change; // bool to know if the adc has changed from Adc_changed()
static uint8_t qSec; // bool to know if the 100hz timer has counter quarter a sec
static OvenData oven; // oven object 
// oven graphics
static char top_on[SIZE_OVEN_RACK];
static char bot_on[SIZE_OVEN_RACK];
static char top_off[SIZE_OVEN_RACK];
static char bot_off[SIZE_OVEN_RACK];
static char deg[sizeof (DEGREE_SYMBOL)];
static char draw_this[OLED_CHARS_PER_LINE];


// **** Put any helper functions here ****

/*This function initializes all oven data to what appropriate starting values*/

void Oven_init() {
    // enter the setup state and bake mode
    oven.state = SETUP;
    oven.prevState = SETUP;

    oven.time_start = GET_ADC_TIME;
    oven.time_curr = oven.time_start;

    oven.temp_start = STARTING_TEMP_BAKE;
    oven.temp_curr = STARTING_TEMP_BAKE;

    oven.cookmode = BAKE;
    oven.selecting_time = TRUE;

    oven.ButtonState = BUTTON_EVENT_NONE;

    oven.flashAmount = 0;

}

/*This function checks the state of the oven and start time - curr time to update the leds*/

void updateLEDs() {
    uint16_t ratio = (LED_RATIO * oven.time_curr) / oven.time_start;
    uint16_t shift = LED_RATIO - ratio;
    // if cooking or reseting turn on LEDs else they should be off
    if (oven.state == COOKING || oven.state == RESET_PENDING) {
        // if the starting time is < 8, use hardcoded values
        // else use the shift value calculated above to shift the leds from FF to 00
        if (oven.time_start < 8) {
            if (oven.time_curr == 7) {
                LEDS_SET(LED_FE);
            } else if (oven.time_curr == 6) {
                LEDS_SET(LED_FC);
            } else if (oven.time_curr == 5) {
                LEDS_SET(LED_F8);
            } else if (oven.time_curr == 4) {
                LEDS_SET(LED_F0);
            } else if (oven.time_curr == 3) {
                LEDS_SET(LED_E0);
            } else if (oven.time_curr == 2) {
                LEDS_SET(LED_C0);
            } else if (oven.time_curr == 1) {
                LEDS_SET(LED_80);
            } else {
                LEDS_SET(LEDS_OFF);
            }
        } else {
            if (oven.time_curr == oven.time_start) {
                LEDS_SET(LEDS_START);
            } else {
                LEDS_SET(LEDS_START << shift);
            }
        }
    } else {
        LEDS_SET(LEDS_OFF);
    }
}

/*This function will update your OLED to reflect the state .*/
void updateOvenOLED(OvenData ovenData) {
    //update OLED here
    if (ovenData.state == FLASHING) {
        if (IS_ODD(ovenData.flashAmount)) {
            OledSetDisplayInverted();
        } else {
            OledSetDisplayNormal();
        }
        OledUpdate();
        return;
    }

    OledSetDisplayNormal();
    OledClear(OLED_COLOR_BLACK);

    if (ovenData.state == COOKING || ovenData.state == RESET_PENDING) {
        if (ovenData.cookmode == BAKE) {
            sprintf(draw_this,
                    "%s   Mode: Bake\n|     |   Time: %d:%02d\n|-----|   Temp: %d%s\n%s",
                    top_on,
                    GET_MIN(ovenData.time_curr), GET_SEC(ovenData.time_curr),
                    ovenData.temp_start, deg,
                    bot_on);
        } else if (ovenData.cookmode == BROIL) {
            sprintf(draw_this,
                    "%s   Mode: Broil\n|     |   Time: %d:%02d\n|-----|   Temp: %d%s\n%s",
                    top_on,
                    GET_MIN(ovenData.time_curr), GET_SEC(ovenData.time_curr),
                    STARTING_TEMP_BROIL, deg,
                    bot_off);
        } else if (ovenData.cookmode == TOAST) {
            sprintf(draw_this,
                    "%s   Mode: Toast\n|     |   Time: %d:%02d\n|-----|\n%s",
                    top_off,
                    GET_MIN(ovenData.time_curr), GET_SEC(ovenData.time_curr),
                    bot_on);
        }
    } else {
        if (ovenData.cookmode == BAKE) {
            if (ovenData.selecting_time) {
                sprintf(draw_this,
                        "%s   Mode: Bake\n|     |  >Time: %d:%02d\n|-----|   Temp: %d%s\n%s",
                        top_off,
                        GET_MIN(ovenData.time_start), GET_SEC(ovenData.time_start),
                        ovenData.temp_start, deg,
                        bot_off);
            } else {
                sprintf(draw_this,
                        "%s   Mode: Bake\n|     |   Time: %d:%02d\n|-----|  >Temp: %d%s\n%s",
                        top_off,
                        GET_MIN(ovenData.time_start), GET_SEC(ovenData.time_start),
                        ovenData.temp_start, deg,
                        bot_off);
            }
        } else if (ovenData.cookmode == BROIL) {

            sprintf(draw_this,
                    "%s   Mode: Broil\n|     |   Time: %d:%02d\n|-----|   Temp: %d%s\n%s",
                    top_off,
                    GET_MIN(ovenData.time_start), GET_SEC(ovenData.time_start),
                    STARTING_TEMP_BROIL, deg,
                    bot_off);
        } else if (ovenData.cookmode == TOAST) {
            sprintf(draw_this,
                    "%s   Mode: Toast\n|     |   Time: %d:%02d\n|-----|\n%s",
                    top_off,
                    GET_MIN(ovenData.time_start), GET_SEC(ovenData.time_start),
                    bot_off);
        }
    }
    OledDrawString(draw_this);
    OledUpdate();
}

/*This function will execute your state machine.  
 * It should ONLY run if an event flag has been set.*/
void runOvenSM(void) {
    switch (oven.state) {
        case SETUP:
            // turn off leds
            LEDS_SET(LEDS_OFF);
            updateOvenOLED(oven);

            if (oven.ButtonState & BUTTON_EVENT_3DOWN) {
                startTime = FRT;
                oven.prevState = SETUP;
                oven.state = SELECTOR_CHANGE_PENDING;
            } else if (oven.ButtonState & BUTTON_EVENT_4DOWN) {
                oven.time_curr = oven.time_start;
                oven.prevState = SETUP;
                oven.state = COOKING;
            }

            if (ADC_change) {
                // this keeps the prev cooking time and temp until the adc changes one more time
                if (oven.prevState == COOKING) {
                    oven.prevState = SETUP;
                    updateOvenOLED(oven);
                    break;
                }

                if (oven.cookmode == BAKE) {
                    // BAKE CHANGES TIME AND TEMP
                    // check the button
                    if (oven.selecting_time) {
                        oven.time_start = GET_ADC_TIME;
                    } else {
                        oven.temp_start = GET_ADC_TEMP;
                    }
                } else if (oven.cookmode == BROIL) {
                    // BROIL CHANGES TIME
                    oven.time_start = GET_ADC_TIME;
                } else if (oven.cookmode == TOAST) {
                    // TOAST CHANGES TIME
                    oven.time_start = GET_ADC_TIME;
                }
                updateOvenOLED(oven);
            }

            break;

        case SELECTOR_CHANGE_PENDING:

            // turn off leds
            LEDS_SET(LEDS_OFF);

            // check how long button was pushed down
            // if it is up, store the current time and calculate the elapsed time
            //      if elapsed time < long press switch modes
            //      else switch selection only when in bake mode
            // update oven
            // stay in this state until the button is up
            if (oven.ButtonState & BUTTON_EVENT_3UP) {
                currTime = FRT;
                elapsed = currTime - startTime;
                if (elapsed < LONG_PRESS) {
                    if (oven.cookmode == BAKE) {
                        oven.cookmode = TOAST;
                    } else if (oven.cookmode == TOAST) {
                        oven.cookmode = BROIL;
                    } else if (oven.cookmode == BROIL) {
                        oven.cookmode = BAKE;
                    }
                } else {
                    if (oven.cookmode == BAKE) {
                        if (oven.selecting_time == TRUE) {
                            oven.selecting_time = FALSE;
                        } else {
                            oven.selecting_time = TRUE;
                        }
                    }
                }
                updateOvenOLED(oven);
                oven.prevState = SELECTOR_CHANGE_PENDING;
                oven.state = SETUP;
            }
            break;

        case COOKING:

            // check if user wants to reset
            // store time and go to reset pending
            if (oven.ButtonState & BUTTON_EVENT_4DOWN) {
                startTime = FRT;
                oven.prevState = COOKING;
                oven.state = RESET_PENDING;
            }
            // if a second has passed, decrease time if still cooking or finish cooking
            if ((oneSecond)) {
                if (oven.time_curr > 0) {
                    oven.time_curr--;
                    // update led
                    updateLEDs();
                    updateOvenOLED(oven);
                } else {
                    // THIS IS WHERE THE OLED WOULD FLASH
                    // AFTER COOKING IS FINISHED
                    oven.state = FLASHING;
                    // reset settings
                    oven.time_curr = oven.time_start;
                    if (oven.cookmode == BAKE) {
                        oven.temp_curr = oven.temp_start;
                    }
                    oven.prevState = COOKING;
                }
                oneSecond = FALSE;
            }
            break;

        case RESET_PENDING:
            // wait until b4 is up 
            //      long press reset to start data
            //      short press goes back to cooking
            if (oven.ButtonState & BUTTON_EVENT_4UP) {
                currTime = FRT;
                elapsed = currTime - startTime;

                if (elapsed >= LONG_PRESS) {
                    // end cooking
                    // reset settings
                    oven.time_curr = oven.time_start;
                    if (oven.cookmode == BAKE) {
                        oven.temp_curr = oven.temp_start;
                    }
                    // update OLED
                    oven.state = RESET_PENDING;
                    oven.state = SETUP;
                    updateOvenOLED(oven);
                    // UPDATE LED (TURN OFF)
                    updateLEDs();
                } else {
                    oven.state = RESET_PENDING;
                    oven.state = COOKING;
                }
            }
            // continue cooking while b4 is down
            if (TIMER_TICK) {
                if (oneSecond) {
                    if (oven.time_curr > 0) {
                        oven.time_curr--;
                        // update led
                        updateLEDs();
                        updateOvenOLED(oven);
                    } else {
                        // THIS IS WHERE THE OLED WOULD FLASH
                        // AFTER COOKING IS FINISHED
                        // reset settings
                        oven.state = FLASHING;
                        oven.time_curr = oven.time_start;
                        if (oven.cookmode == BAKE) {
                            oven.temp_curr = oven.temp_start;
                        }
                        oven.prevState = COOKING;
                    }
                    oneSecond = FALSE;
                }
                TIMER_TICK = FALSE;
            }
            break;

        case FLASHING:
            // update this every quarter second until the max amount of flashes is reached
            if (qSec) {
                if (oven.flashAmount <= FLASH_MAX) {
                    updateOvenOLED(oven);
                    oven.flashAmount++;
                } else {
                    oven.flashAmount = 0;
                    oven.state = SETUP;
                }
                qSec = FALSE;
            }
            break;
    }
}

int main() {
    BOARD_Init();
    LEDS_INIT();
    AdcInit();
    ButtonsInit();
    OledInit();

    //initalize timers and timer ISRs:
    // <editor-fold defaultstate="collapsed" desc="TIMER SETUP">

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.

    T2CON = 0; // everything should be off
    T2CONbits.TCKPS = 0b100; // 1:16 prescaler
    PR2 = BOARD_GetPBClock() / 16 / 100; // interrupt at .5s intervals
    T2CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T2IF = 0; //clear the interrupt flag before configuring
    IPC2bits.T2IP = 4; // priority of  4
    IPC2bits.T2IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T2IE = 1; // turn the interrupt on

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescaler, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.

    T3CON = 0; // everything should be off
    T3CONbits.TCKPS = 0b111; // 1:256 prescaler
    PR3 = BOARD_GetPBClock() / 256 / 5; // interrupt at .5s intervals
    T3CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T3IF = 0; //clear the interrupt flag before configuring
    IPC3bits.T3IP = 4; // priority of  4
    IPC3bits.T3IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T3IE = 1; // turn the interrupt on;

    // </editor-fold>

    printf("Welcome to taltaffe's Lab07 (Toaster Oven).  Compiled on %s %s.", __TIME__, __DATE__);

    //initialize state machine (and anything else you need to init) here
    FRT = 0;
    TIMER_TICK = FALSE;
    ADC_change = FALSE;
    oneSecond = FALSE;

    // create the oven on/off states
    BUILD_LINE(top_on, OVEN_TOP_ON);
    BUILD_LINE(bot_on, OVEN_BOTTOM_ON);
    BUILD_LINE(top_off, OVEN_TOP_OFF);
    BUILD_LINE(bot_off, OVEN_BOTTOM_OFF);
    sprintf(deg, DEGREE_SYMBOL);

    // initialize oven to be in bake mode
    Oven_init();


    while (1) {
        if (TIMER_TICK) {
            runOvenSM();
            TIMER_TICK = FALSE;
        }
        if (oven.ButtonState != BUTTON_EVENT_NONE) {
            runOvenSM();
            oven.ButtonState = BUTTON_EVENT_NONE;
        }
        if (ADC_change) {
            runOvenSM();
            ADC_change = FALSE;
        }
    }
}

/*The 5hz timer is used to update the free-running timer and to generate TIMER_TICK events*/
void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;

    //add event-checking code here
    TIMER_TICK = TRUE;
    ++countToFive;
    if (countToFive == TIMER_FREQ) {
        FRT++;
        oneSecond = TRUE;
        countToFive = 0;
    }
}

/*The 100hz timer is used to check for button and ADC events*/
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    //add event-checking code here
     countTo25++;
    if (countTo25 == Q_SECOND_25_HZ) {
        qSec = TRUE;
        countTo25 = 0;
    }
    oven.ButtonState = ButtonsCheckEvents();
    ADC_change = AdcChanged();
}