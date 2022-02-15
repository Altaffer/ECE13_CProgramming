// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>



//CMPE13 Support Library
#include "BOARD.h"
#include "Leds_Lab06.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// **** Declare any datatypes here ****

struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};


// **** Define global, module-level, or external variables here ****
static struct Timer TimerA;

typedef enum {
    LEFT, RIGHT
} SysState;
static SysState state;


// **** Declare function prototypes ****
char twosc(char c);

int main(void) {
    BOARD_Init();

    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to taltaffe's lab6 part2 (bounce_switch).  Compiled on %s %s.\n", __TIME__, __DATE__);

    LEDS_INIT();
    TimerA.event = FALSE;
    TimerA.timeRemaining = SWITCH_STATES();
    LEDS_SET(0x01); // start at right and shift left
    state = LEFT;

    while (1) {
        if (TimerA.event) {
            if (state == LEFT) {
                if (LATEbits.LATE7) {
                    state = RIGHT;
                } else {
                    LEDS_SET(twosc(LEDS_GET() << 1));
                }
            }
            if (state == RIGHT) {
                if (LATEbits.LATE0) {
                    state = LEFT;
                    LEDS_SET(twosc(LEDS_GET() << 1));
                } else {
                    LEDS_SET(twosc(LEDS_GET() >> 1) &0x00ff);
                }
            }
            TimerA.event = FALSE;
        }
    }

    while (1);


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}

/**
 * This is the interrupt for the Timer1 peripheral. It will trigger at the frequency of the peripheral
 * clock, divided by the timer 1 prescaler and the interrupt interval.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void) {
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    if (SWITCH_STATES() < TimerA.timeRemaining) {
        TimerA.timeRemaining = SWITCH_STATES();
    } else {
        if (TimerA.timeRemaining++ == SWITCH_STATES()) {
            TimerA.event = TRUE;
            TimerA.timeRemaining = 0;
        }
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}

char twosc(char c) {
    char to_ret = c;
    if (c < 0) {
        to_ret = ~c + 1;
    }
    return (to_ret & 0xff);
}