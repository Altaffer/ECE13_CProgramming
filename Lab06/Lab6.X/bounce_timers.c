// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
#include <GenericTypeDefs.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Leds_Lab06.h"


// **** Declare any datatypes here ****

struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

// **** Define global, module-level, or external variables here ****

int16_t A_MAX = 2 * 38;
int16_t B_MAX = 3 * 38;
int16_t C_MAX = 5 * 38;
int16_t MAX = 60 * 38;

static struct Timer TimerA;
static struct Timer TimerB;
static struct Timer TimerC;

// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();
    LEDS_INIT();

    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    IFS0bits.T1IF = 0; //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to taltaffe's lab6 part1 (timers).  Compiled on %s %s.\n", __TIME__, __DATE__);

    // init timers 
    TimerA.event = FALSE;
    TimerA.timeRemaining = A_MAX;
    
    TimerB.event = FALSE;
    TimerB.timeRemaining = B_MAX;
    
    TimerC.event = FALSE;
    TimerC.timeRemaining = C_MAX;
    
    
    
    while (1) {
        if (TimerA.event) {
            TimerA.event = FALSE;
            printf("ta= %d ", MAX/38);
            printf("A\n");
            LEDS_SET(0x01);
        }
        if (TimerB.event) {
            TimerB.event = FALSE;
            printf("tb= %d ", MAX/38);
            printf("B\n");
            LEDS_SET(0x02);
        }
        if (TimerC.event) {
            TimerC.event = FALSE;
            printf("tc= %d ", MAX/38);
            printf("C\n");
            LEDS_SET(0x03);
            
        }
    }


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
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;
    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/

    --MAX;
    --TimerA.timeRemaining;
    --TimerB.timeRemaining;
    --TimerC.timeRemaining;
    
    if (TimerA.timeRemaining == 0) {
        TimerA.event = TRUE;
        TimerA.timeRemaining = A_MAX;
    }
    if (TimerB.timeRemaining == 0) {
        TimerB.event = TRUE;
        TimerB.timeRemaining = B_MAX;
    }
    if (TimerC.timeRemaining == 0) {
        TimerC.event = TRUE;
        TimerC.timeRemaining = C_MAX;
    }
    
    
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}