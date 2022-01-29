// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"


// User libraries
#include "stack.h"

int testsPassed = 0;

int main() {
    BOARD_Init();

    printf("\n###### Beginning taltaffe's stack test harness: ####\n\n");

    //What follows is starter code.  You will need to modify it!

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
        struct Stack stack = {1, 2, 3};
        StackPush(&stack, 4);
        printf("%i", stack.stackItems);
        if (stack.stackItems[3] == 4) {
            printf("passed\n");
            testsPassed++;
        } else {
            printf("failed\n");
        }
        printf("Does StackPush() push a value when its full?\n");
        struct Stack stack = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
        StackPush(&stack, 21);
        printf("%i", stack.stackItems);
        if (StackPush(&stack, 21) == STANDARD_ERROR) {
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
        struct Stack stack = {1, 2, 3};
        StackPop(&stack, 1);
        printf("%i", stack.stackItems);
        if (stack.stackItems[0] == 2) {
            printf("passed\n");
            testsPassed++;
        } else {
            printf("failed\n");
        }
        printf("Does StackPush() pop when struct is empty?\n");
        struct Stack stack = {};
        StackPush(&stack, 1);
        printf("%i", stack.stackItems);
        if (StackPop(&stack, 1) == STANDARD_ERROR) {
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
        StackIsEmpty(&stack);
        if (StackIsEmpty(&stack) == TRUE) {
            printf("passed\n");
            testsPassed++;
        } else {
            printf("failed\n");
        }
        printf("Does StackIsEmpty() identify false case?\n");
        struct Stack stack = {1};
        StackIsEmpty(&stack);
        if (StackIsEmpty(&stack) == FALSE) {
            printf("passed\n");
            testsPassed++;
        } else {
            printf("failed\n");
        }
        
        printf("\nPassed %i/2 StackIsEmpty tests\n\n", testsPassed);
        testsPassed = 0;
    }
    
    void testStackIsFull() {
        
    }
    
    void testGetStackSize(){
        
    }

    BOARD_End();
    //test stackInit:
    while (1);
    return 0;
}


