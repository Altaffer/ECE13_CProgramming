// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"



// User libraries
#include "rpn.h"

// Test Functions

void testRPN_Eval() {
    char test0[] = "1 1 +";
    double result0;
    double expected0 = 2;
    printf("Testing RPN_Evaluate with \"%s\"... \n ", test0);
    int error;
    error = RPN_Evaluate(test0, &result0);
    if (error) {
        printf("   Failed, RPN_Evaluate produced an error\n");
    } else if (result0 != expected0) {
        printf("   Failed, expected = %f , result = %f\n", expected0, result0);
    } else {
        printf("   Success!\n");
    }
    printf("%lf\n", result0);
}

void testProcessBack() {
    int testsPassed = 0;
    printf("Testing ProcessBackspaces:\n");
    char test_pb1[] = "123\b34";
    ProcessBackspaces(test_pb1);
    printf("result    : %s\n", test_pb1);
    printf("should be : 1234\n");
    printf("passed\n");
    testsPassed++;
    char test_pb2[] = "1\b\b234";
    ProcessBackspaces(test_pb2);
    printf("result    : %s\n", test_pb2);
    printf("should be : 234\n");
    printf("passed\n");
    testsPassed++;
    char test_pb3[] = "1\b\b";
    ProcessBackspaces(test_pb3);
    printf("result    : %s\n", test_pb3);
    printf("should be : \n");
    printf("passed\n");
    testsPassed++;

    printf("\nPassed %i/3 ProcessBackspace tests\n\n", testsPassed);
    testsPassed = 0;
}

int main() {
    BOARD_Init();

    printf("\n###### Beginning taltaffe's rpn test harness: ####\n\n");

    //What follows is starter code.  You will need to modify it!
    testRPN_Eval();
//    testProcessBack();

    BOARD_End();
    while (1);
}


