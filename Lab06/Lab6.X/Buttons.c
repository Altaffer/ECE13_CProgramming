#include <stdio.h>
#include <stdint.h>
#include <genericTypeDefs.h>
#include <xc.h>
#include <math.h>

// We rely on this file to define various macros for working with the hardware buttons.
#include "BOARD.h"
#include "Buttons.h"


//defines
#define HIGH 1
#define LOW 0
#define NUM_DEBOUNCES 5

// Macros
#define BUTTON_STATES() (((PORTD >> 4) & 0x0E) | ((PORTF >> 1) & 0x01))


/**
 * The ButtonStateFlags enum provides a bitmask for use with the BUTTON_STATES macro. By bitwise-
 * ANDing any of these enum values with the return value from BUTTON_STATES, the current state of
 * the buttons can be tested.
 *
 * For example:
 *
 * uint8_t buttonsState = BUTTON_STATES();
 * if (buttonsState & BUTTON_STATE_3) {
 *   // Buttons 3 is pressed down.
 * }
 *
 * @see BUTTON_STATES()
 */
enum ButtonStateFlags {
    BUTTON_STATE_1 = 0x1,
    BUTTON_STATE_2 = 0x2,
    BUTTON_STATE_3 = 0x4,
    BUTTON_STATE_4 = 0x8
};
enum bitsmask_ind{
    xFE = 0,
    xFD,
    xFB,
    xF7,
    xEF,
    xDF,
    xBF,
    x7F,
    xFC
};



// static variable to hold prev state of buttons
static uint8_t prevState;
// used to calculate the current state of the buttons
static uint8_t currState;
// buffer to debounce
static uint8_t buffer_b1[NUM_DEBOUNCES];
static uint8_t buffer_b2[NUM_DEBOUNCES];
static uint8_t buffer_b3[NUM_DEBOUNCES];
static uint8_t buffer_b4[NUM_DEBOUNCES];
// bitMask at i masks every bit exept the ith bit
uint8_t bitMask[] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F, 0xFC};

/**
 * This function initializes the proper pins such that the buttons 1-4 may be used by modifying
 * the necessary bits in TRISD/TRISF. Only the bits necessary to enable the 1-4 buttons are
 * modified, so that this library does not interfere with other libraries.
 */
void ButtonsInit(void)
{
    // sets button 1 as an input
    TRISFbits.TRISF1 = 0x01;
    // sets buttons 2,3,4 as inputs
    TRISDbits.TRISD5 = 0x01;
    TRISDbits.TRISD6 = 0x01;
    TRISDbits.TRISD7 = 0x01;
    // all buttons are up 
    prevState = BUTTON_EVENT_NONE;//0x55;
    currState = BUTTON_EVENT_NONE;//0x55;
}

/**
 * ButtonsCheckEvents function checks the current button states and returns
 *  any events that have occured since its last
 * call.  This function should be called repeatedly in a Timer ISR, though it can
 * be called in main() during testing.
 *
 * In normal use, this function should only be used after ButtonsInit().
 * 
 * This function should assume that the buttons start in an off state with 
 * value 0. Therefore if no buttons are
 * pressed when ButtonsCheckEvents() is first called, 
 * BUTTONS_EVENT_NONE should be returned. 
 * 
 * @return  Each bit of the return value corresponds to one ButtonEvent flag,
 *          as described in Buttons.h.  If no events are detected, BUTTONS_EVENT_NONE is returned.
 * 
 * Note that more than one event can occur simultaneously, though this situation is rare.
 * To handle this, the output should be a bitwise OR of all applicable event flags. 
 * For example, if button 1 was released at the same time that button 2 was pressed,
 * this function should return (BUTTON_EVENT_1UP | BUTTON_EVENT_2DOWN) 
 * 
 */
uint8_t ButtonsCheckEvents(void)
{
    int i, j;
    uint8_t b1_high_flag = TRUE;
    uint8_t b1_low_flag = TRUE;
    uint8_t b2_high_flag = TRUE;
    uint8_t b2_low_flag = TRUE;
    uint8_t b3_high_flag = TRUE;
    uint8_t b3_low_flag = TRUE;
    uint8_t b4_high_flag = TRUE;
    uint8_t b4_low_flag = TRUE;
    
    for (i = 0; i < NUM_DEBOUNCES; i++) {
        // DEBOUNCE BTN1
        buffer_b1[4] = buffer_b1[3];
        buffer_b1[3] = buffer_b1[2];
        buffer_b1[2] = buffer_b1[1];
        buffer_b1[1] = buffer_b1[0];
        buffer_b1[0] = PORTFbits.RF1;
        
        // DEBOUNCE BTN2
        buffer_b2[4] = buffer_b2[3];
        buffer_b2[3] = buffer_b2[2];
        buffer_b2[2] = buffer_b2[1];
        buffer_b2[1] = buffer_b2[0];
        buffer_b2[0] = PORTDbits.RD5;
        
        // DEBOUNCE BTN3
        buffer_b3[4] = buffer_b3[3];
        buffer_b3[3] = buffer_b3[2];
        buffer_b3[2] = buffer_b3[1];
        buffer_b3[1] = buffer_b3[0];
        buffer_b3[0] = PORTDbits.RD6;
        
        // DEBOUNCE BTN4
        buffer_b4[4] = buffer_b4[3];
        buffer_b4[3] = buffer_b4[2];
        buffer_b4[2] = buffer_b4[1];
        buffer_b4[1] = buffer_b4[0];
        buffer_b4[0] = PORTDbits.RD7;
    }
    
    // if the buffer is low at any point in the array then button is not down yet
    for (j = 0; j < NUM_DEBOUNCES; j++) {
        if (buffer_b1[j] == LOW) {
            b1_high_flag = FALSE;
        }
        if (buffer_b2[j] == LOW) {
            b2_high_flag = FALSE;
        }
        if (buffer_b3[j] == LOW) {
            b3_high_flag = FALSE;
        }
        if (buffer_b4[j] == LOW) {
            b4_high_flag = FALSE;
        }
    }
    // if the buffer is high at any point in the array then button is not up yet
    for (j = 0; j < NUM_DEBOUNCES; j++) {
        if (buffer_b1[j] == HIGH) {
            b1_low_flag = FALSE;
        }
        if (buffer_b2[j] == HIGH) {
            b2_low_flag = FALSE;
        }
        if (buffer_b3[j] == HIGH) {
            b3_low_flag = FALSE;
        }
        if (buffer_b4[j] == HIGH) {
            b4_low_flag = FALSE;
        }
    }
    // BUTTON 1 UP OR DOWN LOGIC
    if (b1_high_flag && !(currState & BUTTON_EVENT_1DOWN)) {
        currState |= BUTTON_EVENT_1DOWN;
        currState &= bitMask[xFE]; // and with FE
    } else if(b1_low_flag && !(currState & BUTTON_EVENT_1UP) && (prevState & BUTTON_EVENT_1DOWN)){
        currState |= BUTTON_EVENT_1UP;
        currState &= bitMask[xFD]; // and with FD
    }
    
    // BUTTON 2 UP OR DOWN LOGIC
    if (b2_high_flag && !(currState & BUTTON_EVENT_2DOWN)) {
        currState |= BUTTON_EVENT_2DOWN;
        currState &= bitMask[xFB]; // and with FE
    } else if(b2_low_flag && !(currState & BUTTON_EVENT_2UP) && (prevState & BUTTON_EVENT_2DOWN)){
        currState |= BUTTON_EVENT_2UP;
        currState &= bitMask[xF7]; // and with FD
    }
    
    // BUTTON 3 UP OR DOWN LOGIC
    if (b3_high_flag && !(currState & BUTTON_EVENT_3DOWN)) {
        currState |= BUTTON_EVENT_3DOWN;
        currState &= bitMask[xFE]; // and with FE
    } else if(b3_low_flag && !(currState & BUTTON_EVENT_3UP) && (prevState & BUTTON_EVENT_3DOWN)){
        currState |= BUTTON_EVENT_3UP;
        currState &= bitMask[xFD]; // and with FD
    }
    
    // BUTTON 4 UP OR DOWN LOGIC
    if (b4_high_flag && !(currState & BUTTON_EVENT_4DOWN)) {
        currState |= BUTTON_EVENT_4DOWN;
        currState &= bitMask[xFB]; // and with FE
    } else if(b4_low_flag && !(currState & BUTTON_EVENT_4UP) && (prevState & BUTTON_EVENT_4DOWN)){
        currState |= BUTTON_EVENT_4UP;
        currState &= bitMask[xF7]; // and with FD
    }
    

    if (currState != prevState) {        
        prevState = currState;
        
    } else {
        currState = BUTTON_EVENT_NONE;
    }
    
    return currState;

    }