// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"


// User libraries
#include "stack.h"

// User Variables
int testsPassed = 0;

// Helper Test functions

void testStackInit() {
    printf("Does StackInit() set the currentItemIndex appropriately?\n");
    struct Stack stack = {};
    StackInit(&stack);
    if (stack.currentItemIndex == -1) {
        printf("passed\n");
        testsPassed++;
    } else {
        printf("failed\n");
    }
    printf("\nPassed %i/1 StackInit tests\n\n", testsPassed);
    testsPassed = 0;
}

void testStackPush() {
    printf("Does StackPush() push a value onto the stack?\n");
    struct Stack stack = {};
    StackInit(&stack);
    StackPush(&stack, 4);
    printf("The Stack has a size: %i\n", StackGetSize(&stack));
    if (stack.stackItems[0] == 4) {
        printf("passed\n");
        testsPassed++;
    } else {
        printf("failed\n");
    }
    printf("Does StackPush() push a value when its full?\n");
    struct Stack stack2 = {};
    StackInit(&stack2);
    StackPush(&stack2, 1);
    StackPush(&stack2, 2);
    StackPush(&stack2, 3);
    StackPush(&stack2, 4);
    StackPush(&stack2, 5);
    StackPush(&stack2, 6);
    StackPush(&stack2, 7);
    StackPush(&stack2, 8);
    StackPush(&stack2, 9);
    StackPush(&stack2, 10);
    StackPush(&stack2, 11);
    StackPush(&stack2, 12);
    StackPush(&stack2, 13);
    StackPush(&stack2, 14);
    StackPush(&stack2, 15);
    StackPush(&stack2, 16);
    StackPush(&stack2, 17);
    StackPush(&stack2, 18);
    StackPush(&stack2, 19);
    StackPush(&stack2, 20);
    printf("The Stack has a size: %i\n", StackGetSize(&stack2));
    if (StackPush(&stack2, 21) == STANDARD_ERROR) {
        printf("passed\n");
        testsPassed++;
    } else {
        printf("failed\n");
    }
    printf("\nPassed %i/2 StackPush tests\n\n", testsPassed);
    testsPassed = 0;
}

void testStackPop() {
    printf("Does StackPop() pop a value onto the stack?\n");
    double x = 1;
    struct Stack stack = {};
    StackInit(&stack);
    StackPush(&stack, 1);
    StackPush(&stack, 2);
    int k = StackGetSize(&stack);
    printf("The stack has a size: %i before poping\n", k);
    StackPop(&stack, &x);
    int j = StackGetSize(&stack);
    printf("The stack has a size: %i after popping\n", j);
    if ((x == 2) && (k - j == 1)) {
        printf("passed\n");
        testsPassed++;
    } else {
        printf("failed\n");
    }
    printf("Does StackPop() pop when struct is empty?\n");
    struct Stack stack2 = {};
    StackInit(&stack2);
    int p = StackGetSize(&stack2);
    printf("The stack has a size: %i before popping\n", p);
    StackPop(&stack2, &x);
    if (StackPop(&stack2, &x) == STANDARD_ERROR) {
        printf("passed\n");
        testsPassed++;
    } else {

        printf("failed\n");
    }
    printf("\nPassed %i/2 StackPop tests\n\n", testsPassed);
    testsPassed = 0;
}

void testStackIsEmpty() {
    printf("Does StackIsEmpty() identify an empty stack?\n");
    struct Stack stack = {};
    StackInit(&stack);
    int q = StackGetSize(&stack);
    printf("The stack has a size %i\n", q);
    StackIsEmpty(&stack);
    if (StackIsEmpty(&stack) == TRUE) {
        printf("passed\n");
        testsPassed++;
    } else {
        printf("failed\n");
    }
    printf("Does StackIsEmpty() identify false case?\n");
    struct Stack stack2 = {};
    StackInit(&stack2);
    StackPush(&stack2, 1);
    int r = StackGetSize(&stack2);
    printf("The stack has a size %i\n", r);
    StackIsEmpty(&stack2);
    if (StackIsEmpty(&stack2) == FALSE) {
        printf("passed\n");
        testsPassed++;
    } else {

        printf("failed\n");
    }

    printf("\nPassed %i/2 StackIsEmpty tests\n\n", testsPassed);
    testsPassed = 0;
}

void testStackIsFull() {
    printf("Does StackIsFull() identify a full stack?\n");
    struct Stack stack = {};
    StackInit(&stack);
    StackPush(&stack, 1);
    StackPush(&stack, 2);
    StackPush(&stack, 3);
    StackPush(&stack, 4);
    StackPush(&stack, 5);
    StackPush(&stack, 6);
    StackPush(&stack, 7);
    StackPush(&stack, 8);
    StackPush(&stack, 9);
    StackPush(&stack, 10);
    StackPush(&stack, 11);
    StackPush(&stack, 12);
    StackPush(&stack, 13);
    StackPush(&stack, 14);
    StackPush(&stack, 15);
    StackPush(&stack, 16);
    StackPush(&stack, 17);
    StackPush(&stack, 18);
    StackPush(&stack, 19);
    StackPush(&stack, 20);
    printf("The Stack has a size: %i\n", StackGetSize(&stack));
    StackIsFull(&stack);
    if (StackIsFull(&stack) == TRUE) {
        printf("passed\n");
        testsPassed++;
    } else {
        printf("failed\n");
    }
    printf("Does StackIsFull() identify false case?\n");
    struct Stack stack2 = {};
    StackInit(&stack2);
    printf("The Stack has a size: %i\n", StackGetSize(&stack2));
    StackIsFull(&stack2);
    if (StackIsFull(&stack2) == FALSE) {
        printf("passed\n");
        testsPassed++;
    } else {

        printf("failed\n");
    }

    printf("\nPassed %i/2 StackIsFull tests\n\n", testsPassed);
    testsPassed = 0;
}

void testGetStackSize() {
    printf("Does GetStackSize() identify stack size?\n");
    struct Stack stack = {};
    StackInit(&stack);
    StackPush(&stack, 2);
    StackPush(&stack, 3.1);
    StackGetSize(&stack);
    printf("The stack size is %i\n", StackGetSize(&stack));
    if (StackGetSize(&stack) == 2) {
        printf("passed\n");
        testsPassed++;
    } else {
        printf("failed\n");
    }
    printf("Does GetStackSize() identify -1 case?\n");
    struct Stack stack2 = {};
    StackInit(&stack2);
    StackGetSize(&stack2);
    printf("The stack size is %i\n", StackGetSize(&stack2));
    if (StackGetSize(&stack2) == 0) {
        printf("passed\n");
        testsPassed++;
    } else {

        printf("failed\n");
    }

    printf("\nPassed %i/2 GetStackSize tests\n\n", testsPassed);
    testsPassed = 0;
}

int main() {
    BOARD_Init();

    printf("\n###### Beginning taltaffe's stack test harness: ####\n\n");

    //What follows is starter code.  You will need to modify it!
    testStackInit();
    testStackPush();
    testStackPop();
    testStackIsEmpty();
    testStackIsFull();
    testGetStackSize();

    BOARD_End();
    //test stackInit:
    while (1);
    return 0;
}


