// Standard libraries
#include <stdio.h>
#include <math.h>


//User libraries:
#include "BOARD.h"

#ifdef PIC32
// Microchip libraries
#include <xc.h>
#endif


// **** Declare function prototypes ****
double Add(double operand1, double operand2);
double Multiply(double operand1, double operand2);
double AbsoluteValue(double operand);
double FahrenheitToCelsius(double operand);
double Tangent(double operand);
//add more prototypes here


void CalculatorRun(void)
{
    printf("\n\nWelcome to Luca's calculator program! Compiled at %s %s\n",__DATE__, __TIME__);

    // Your code here
    BOARD_End();
    while (1);
}



/********************************************************************************
 * Define the Add function here.
 ********************************************************************************/
double Add(double operand1, double operand2){
    // conducting addition between operands
    double result = (operand1 + operand2);
    // return the result
    return result;
}

/********************************************************************************
 * Define the Subtract function here.
 ********************************************************************************/
double Subtract(double operand1, double operand2)
{
    double result = (operand1 - operand2);
    return result;
	
}

/********************************************************************************
 * Define the Multiply function here.
 ********************************************************************************/
double Multiply(double operand1, double operand2){
    double result = (operand1 * operand2);
    return result;
}

/********************************************************************************
 * Define the Divide function here.
 ********************************************************************************/
double Divide(double operand1, double operand2)
{
    double result = (operand1 / operand2);
    return result;
	
}

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
double Absolute(double operand1){
    double result = abs(operand1);
    return result;
}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double F2C(double operand1){
    double result = ((operand1 - 32) * (5 / 9));
    return result;
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double C2F(double operand1){
    double result = ((operand1 + (9 / 5)) + 32);
    return result;
}

/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
double Average(double operand1, double operand2){
    double result = ((operand1 + operand2) / 2);
    return result;
}

/*********************************************************************************
 * Define the Tangent function that takes input in degrees.
 ********************************************************************************/
double Tangent(double operand1){
    double result = tan((operand1 * (180 / M_PI )));
    return result;
}

/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/
double Round(double operand)
{
    double result = floor(operand);
    return result;
}


