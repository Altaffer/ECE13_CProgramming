// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "BOARD.h"
#include "Ascii.h"
#include "Oled.h"
#include "OledDriver.h"

// **** Set macros and preprocessor directives ****
#define WIN_SIZE 5
#define STR_SIZE 5
#define POT_MAX 1023
#define POT_MIN 0

// **** Declare any datatypes here ****

struct AdcResult {
    int8_t event;
    int16_t voltage;
};

// **** Define global, module-level, or external variables here ****
static struct AdcResult adc;
int16_t last_max = 0;

// **** Declare function prototypes ****

int main(void) {
    BOARD_Init();

    // Enable interrupts for the ADC
    IPC6bits.AD1IP = 2;
    IPC6bits.AD1IS = 0;
    IEC1bits.AD1IE = 1;

    // Set B2 to an input so AN2 can be used by the ADC.
    TRISBbits.TRISB2 = 1;

    // Configure and start the ADC
    AD1CHSbits.CH0SA = 2; // add B2 to the mux
    AD1PCFGbits.PCFG2 = 0; // add b2 to the ADC
    AD1CSSLbits.CSSL2 = 1; // and add b2 to the scanner

    AD1CON1 = 0; // start with 0
    AD1CON1bits.SSRC = 0b111; // auto conversion
    AD1CON1bits.ASAM = 1; // auto sample

    AD1CON2 = 0; // start with 0
    AD1CON2bits.SMPI = 7; // one interrupt per 8 samples

    AD1CON3 = 0; // start with 0
    AD1CON3bits.SAMC = 29; // long sample time
    AD1CON3bits.ADCS = 50; // long conversion time

    AD1CON1bits.ADON = 1; // and finally turn it on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome taltaffe's lab6 part3 (bounce_adc).  Compiled on %s %s.\n", __TIME__, __DATE__);
    OledInit();
    adc.event = FALSE;
    adc.voltage = 0;

    while (1) {
        if (adc.event) {
            // print to oled
            char voltageString[STR_SIZE];
            char percentString[STR_SIZE];
            sprintf(voltageString, "%d", adc.voltage);
            sprintf(percentString, "%.0f", (100 * ((double) adc.voltage / 1023)));

            char str[512] = "\nPotentiometer value:\n  ";
            strcat(str, voltageString);
            strcat(str, "\n  ");
            strcat(str, percentString);
            strcat(str, "%\n");
            //          //  isnt flushing string 
            //            strcat(str, "\r\n"); 
            printf("%s", str);
            // flickers too much 
            OledClear(OLED_COLOR_BLACK);
            OledUpdate();
            printf("%s\n", str);
            OledDrawString(str);
            OledUpdate();
            adc.event = FALSE;
        }
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the ADC1 peripheral. It will trigger whenever a new ADC reading is available
 * in the ADC buffer SFRs, ADC1BUF0 - ADC1BUF7.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_ADC_VECTOR, ipl2auto) AdcHandler(void) {
    // Clear the interrupt flag.
    IFS1bits.AD1IF = 0;
    last_max = (ADC1BUF0 + ADC1BUF1
            + ADC1BUF2 + ADC1BUF3
            + ADC1BUF4 + ADC1BUF5
            + ADC1BUF6 + ADC1BUF7) / 8;

    if ((last_max >= POT_MAX - WIN_SIZE) || (last_max <= POT_MIN - WIN_SIZE)) {
        adc.event = TRUE;
        adc.voltage = last_max;
    } else if (adc.voltage < (last_max - WIN_SIZE) || adc.voltage > (last_max + WIN_SIZE)) {
        adc.event = TRUE;
        adc.voltage = last_max;
    }
}