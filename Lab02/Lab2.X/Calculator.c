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
int Round(double operand1);
double Tangent(double operand);
//add more prototypes here

void CalculatorRun(void) {
    printf("\n\nWelcome to Luca's calculator program! Compiled at %s %s\n", __DATE__, __TIME__);

    // Your code here
    BOARD_End();
    while (1) {
        // Initializing the operator as a char
        char op = 'a';
        printf("Enter a mathematical operation to perform (*,/,+,-,m,a,c,f,t,r): ");
//        scanf("%c", &op);
        if (op == '*') {
            double operator1, operator2;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%lf", &operator1);

            // Getting the second operand
            printf("Enter the second operand: ");
            scanf("%lf", &operator2);

            // performing tbe math
            double answer = Multiply(operator1, operator2);

            //print result
            printf("Result of (%lf * %lf): %lf\n",operator1, operator2, answer);

        } else if (op == '/') {
            double operator1, operator2;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%lf", &operator1);

            // Getting the second operand
            printf("Enter the second operand: ");
            scanf("%lf", &operator2);

            // If the second operand is 0
            if (operator2 == 0) {
                printf("Error: Divide by 0!");
            } else {

                // performing tbe math
                double answer = Divide(operator1, operator2);

                //print result
                printf("Result of (%lf / %lf): %lf\n",operator1, operator2, answer);
            }

        } else if (op == '+') {
            double operator1, operator2;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%lf", &operator1);

            // Getting the second operand
            printf("Enter the second operand: ");
            scanf("%lf", &operator2);

            // performing tbe math
            double answer = Add(operator1, operator2);

            //print result
            printf("Result of (%lf + %lf): %lf\n",operator1, operator2, answer);

        } else if (op == '-') {
            double operator1, operator2;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%lf", &operator1);

            // Getting the second operand
            printf("Enter the second operand: ");
            scanf("%lf", &operator2);

            // performing tbe math
            double answer = Subtract(operator1, operator2);

            //print result
            printf("Result of (%lf - %lf): %lf\n",operator1, operator2, answer);

        } else if (op == 'm') {
            double operator1, operator2;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%lf", &operator1);

            // Getting the second operand
            printf("Enter the second operand: ");
            scanf("%lf", &operator2);

            // performing tbe math
            double answer = Average(operator1, operator2);

            //print result
            printf("Result of Averaging (%lf & %lf): %lf\n",operator1, operator2, answer);

        } else if (op == 'a') {
            double operator1;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%lf", &operator1);

            // performing tbe math
            double answer = AbsoluteValue(operator1);

            //print result
            printf("Result of |%lf|: %lf\n",operator1, answer);

        } else if (op == 'c') {
            double operator1;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%lf", &operator1);

            // performing tbe math
            double answer = FahrenheitToCelsius(operator1);

            //print result
            printf("Result of Fahrenheit %lf to Celsius %lf\n",operator1, answer);

        } else if (op == 'f') {
            double operator1;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%lf", &operator1);

            // performing tbe math
            double answer = CelsiusToFahrenheit(operator1);

            //print result
            printf("Result of Celsius %lf to Fahrenheit %lf\n",operator1, answer);

        } else if (op == 't') {
            double operator1;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%lf", &operator1);

            // performing tbe math
            double answer = Tangent(operator1);

            //print result
            printf("Result of tan(%lf): %lf\n",operator1, answer);

        } else if (op == 'q') {
            // exits out
            break;
            
        } else if (op == 'r') {
            double operator1;

            // Getting the first operand
            printf("Enter the first operand: ");
            scanf("%lf", &operator1);

            // performing tbe math
            double answer = Round(operator1);

            //print result
            printf("Result of Rounding(%lf): %lf\n",operator1, answer);

        }
        else{ 
            printf("Invalid operator");
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
    double result = 0;
    if(operand1 < 0) {
        result = -operand1;
    } else {
        result = operand1;
    }
    return result;
}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double FahrenheitToCelsius(double operand1) {
    double result = ((operand1 - 32) * 0.55555555555);
    return result;
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double CelsiusToFahrenheit(double operand1) {
    double result = ((operand1 + (1.8)) + 32);
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
int Round(double operand) {
    return operand < 0 ? operand - 0.5 : operand + 0.5;
}


