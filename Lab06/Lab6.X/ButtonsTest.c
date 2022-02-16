// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Buttons.h"
#include "Oled.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries

// **** Set macros and preprocessor directives ****

// table size = 2 * (C(4,1) + C(4,2) + C(4,3) + C(4,4)) = 30 cases :/
// the amount of combinations of buttons to press mult by 2 since a button can be up or down
#define STRING_TABLE_SIZE 30

// **** Declare any datatypes here ****
struct Timer {
    int8_t event;
    int16_t counter;
};
// **** Define global, module-level, or external variables here ****
static struct Timer TimerA;
// holds output of check event
static uint8_t currState;
static uint8_t lastEvent;


// **** Declare function prototypes ****
void printMessage(uint8_t event);

int main(void)
{
    BOARD_Init();
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 2; // 1:64 prescaler
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
    printf("Welcome to taltaffe's lab6 part4 (ButtonsTest).  Compiled on %s %s.\n", __TIME__, __DATE__);
    
    ButtonsInit();
    OledInit();
    TimerA.event = FALSE;
    TimerA.counter = 0;
    currState = 0x55;
    lastEvent = 0x55;    
    printf("Please press some buttons!\n");
    
    while(1) {
        if (currState != BUTTON_EVENT_NONE) {
            printMessage(currState);
            TimerA.event = FALSE;
        }
    }
    
    
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. It should check for button events and stores them in a
 * module-level variable.
 * 
 * You should not modify this function for ButtonsTest.c or bounce_buttons.c!
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    
    currState = ButtonsCheckEvents();
    if (TimerA.counter == BUTTONS_DEBOUNCE_PERIOD) {
        if (currState != BUTTON_EVENT_NONE) {
            TimerA.event = TRUE;
            TimerA.counter = 0;
        }
        
    } else {
        TimerA.counter++;
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}

void printMessage(uint8_t event)
{    
    if (event == 0x00) {
        return;
    }
    
// INCOMPLETE
    char* MessagesTable[STRING_TABLE_SIZE] = {
      // C(4,1)
      "\nEVENT:   4:---- 3:---- 2:---- 1:  UP", // 55
      "\nEVENT:   4:---- 3:---- 2:---- 1:DOWN", // 56
      
      "\nEVENT:   4:---- 3:---- 2:  UP 1:----", // 55
      "\nEVENT:   4:---- 3:---- 2:DOWN 1:----", // 5
      
      "\nEVENT:   4:---- 3:  UP 2:---- 1:----",
      "\nEVENT:   4:---- 3:DOWN 2:---- 1:----",
      
      "\nEVENT:   4:  UP 3:---- 2:---- 1:----",
      "\nEVENT:   4:DOWN 3:---- 2:---- 1:----",
      
      // C(4,2)
      "\nEVENT:   4:---- 3:---- 2:  UP 1: UP",
      "\nEVENT:   4:---- 3:---- 2:DOWN 1:DOWN",
      
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      
      // C(4,3)
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      
      // C(4,4)
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      "\nEVENT:   4:---- 3:---- 2:---- 1:----",
      
    };

    // print the event
    int i = 0;
    for (i = 0; i < STRING_TABLE_SIZE; i++)
    {
        if ((event  == i + 0x55) && (event != lastEvent)) {
            printf("%s", MessagesTable[i]);
            OledDrawString(MessagesTable[i]);
            OledUpdate();
            lastEvent = event;
        }
    }

}