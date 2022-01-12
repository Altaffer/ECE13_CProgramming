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
double Divide(double operand1, double operand2);
double Subtract(double operand1, double operand2);
double Average(double operand1, double operand2);
double AbsoluteValue(double operand);
double FahrenheitToCelsius(double operand);
double CelsiusToFahrenheit(double operand1);
double Round(double operand1);
double Tangent(double operand);
//add more prototypes here

void CalculatorRun(void) {
    printf("\n\nWelcome to Luca's calculator program! Compiled at %s %s\n", __DATE__, __TIME__);

    // Your code here
    BOARD_End();
    while (1) {
        // Initializing the operator as a char
        char op[1];
        printf("Enter a mathematical operation to perform (*,/,+.-.v.a.c.f.t.r): ");
        scanf("%c", op);
        if (op == "*") {
            double operator1, operator2;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%f", &operator1);

            // Getting the second operand
            printf("Enter the second operand: ");
            scanf("%f", &operator2);

            // performing tbe math
            double answer = Multiply(operator1, operator2);

            //print result
            printf("Result of (", operator1, " * ", operator2, "): ", answer);

        } else if (op == "/") {
            double operator1, operator2;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%f", &operator1);

            // Getting the second operand
            printf("Enter the second operand: ");
            scanf("%f", &operator2);

            // If the second operand is 0
            if (operator2 == 0) {
                printf("Error: Divide by 0!");
            } else {

                // performing tbe math
                double answer = Divide(operator1, operator2);

                //print result
                printf("Result of (", operator1, " / ", operator2, "): ", answer);
            }

        } else if (op == "+") {
            double operator1, operator2;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%f", &operator1);

            // Getting the second operand
            printf("Enter the second operand: ");
            scanf("%f", &operator2);

            // performing tbe math
            double answer = Add(operator1, operator2);

            //print result
            printf("Result of (", operator1, " + ", operator2, "): ", answer);

        } else if (op == "-") {
            double operator1, operator2;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%f", &operator1);

            // Getting the second operand
            printf("Enter the second operand: ");
            scanf("%f", &operator2);

            // performing tbe math
            double answer = Subtract(operator1, operator2);

            //print result
            printf("Result of (", operator1, " - ", operator2, "): ", answer);

        } else if (op == "m") {
            double operator1, operator2;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%f", &operator1);

            // Getting the second operand
            printf("Enter the second operand: ");
            scanf("%f", &operator2);

            // performing tbe math
            double answer = Average(operator1, operator2);

            //print result
            printf("Result of Averaging(", operator1, " & ", operator2, "): ", answer);

        } else if (op == "a") {
            double operator1;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%f", &operator1);

            // performing tbe math
            double answer = AbsoluteValue(operator1);

            //print result
            printf("Result of |", operator1, "|: ", answer);

        } else if (op == "c") {
            double operator1;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%f", &operator1);

            // performing tbe math
            double answer = FahrenheitToCelsius(operator1);

            //print result
            printf("Result of Fahrenheit", operator1, " to Celsius ", answer);

        } else if (op == "f") {
            double operator1;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%f", &operator1);

            // performing tbe math
            double answer = CelsiusToFahrenheit(operator1);

            //print result
            printf("Result of Celsius", operator1, " to Fahrenheit ", answer);

        } else if (op == "t") {
            double operator1;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%f", &operator1);

            // performing tbe math
            double answer = Tangent(operator1);

            //print result
            printf("Result of tan(", operator1, "): ", answer);

        } else if (op == "q") {
            // exits out
            break;
        } else if (op == "r") {
            double operator1;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%f", &operator1);

            // performing tbe math
            double answer = Round(operator1);

            //print result
            printf("Result of Rounding(", operator1, "): ", answer);

        }
    }
}

/********************************************************************************
 * Define the Add function here.
 ********************************************************************************/
double Add(double operand1, double operand2) {
    // conducting addition between operands
    double result = (operand1 + operand2);
    // return the result
    return result;
}

/********************************************************************************
 * Define the Subtract function here.
 ********************************************************************************/
double Subtract(double operand1, double operand2) {
    double result = (operand1 - operand2);
    return result;

}

/********************************************************************************
 * Define the Multiply function here.
 ********************************************************************************/
double Multiply(double operand1, double operand2) {
    double result = (operand1 * operand2);
    return result;
}

/********************************************************************************
 * Define the Divide function here.
 ********************************************************************************/
double Divide(double operand1, double operand2) {
    double result = (operand1 / operand2);
    return result;

}

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
double AbsoluteValue(double operand1) {
    double result = abs(operand1);
    return result;
}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double FahrenheitToCelsius(double operand1) {
    double result = ((operand1 - 32) * (5 / 9));
    return result;
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double CelsiusToFahrenheit(double operand1) {
    double result = ((operand1 + (9 / 5)) + 32);
    return result;
}

/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
double Average(double operand1, double operand2) {
    double result = ((operand1 + operand2) / 2);
    return result;
}

/*********************************************************************************
 * Define the Tangent function that takes input in degrees.
 ********************************************************************************/
double Tangent(double operand1) {
    double result = tan((operand1 * (180 / M_PI)));
    return result;
}

/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/
double Round(double operand) {
    double result = floor(operand);
    return result;
}


