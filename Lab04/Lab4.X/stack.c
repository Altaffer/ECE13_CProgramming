#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "BOARD.h"
#include "stack.h"

// Creating function to inialize the Stack

void StackInit(struct Stack *stack) {
    stack->currentItemIndex = -1;
    stack->initialized = TRUE;
}

// Function to push a value onto the stack

int StackPush(struct Stack *stack, double value) {
    if (stack->currentItemIndex == 19) {
        printf("The stack is full");
        return STANDARD_ERROR;
    } else {
        stack->currentItemIndex += 1;
        stack->stackItems[stack->currentItemIndex] = value;
        return SUCCESS;
    }
}

// function to remove element from stack

int StackPop(struct Stack *stack, double *value) {
    if (stack->currentItemIndex == -1) {
        printf("The stack is empty");
        return STANDARD_ERROR;
    } else {
        value = &stack->stackItems[stack->currentItemIndex];
        stack->stackItems[stack->currentItemIndex--];
        return SUCCESS;
    }
}

// Checks whether stack is empty or not

int StackIsEmpty(const struct Stack *stack) {
    if (stack->currentItemIndex == -1) {
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

// Checks if Stack is full or not 

int StackIsFull(const struct Stack *stack) {
 if (stack->currentItemIndex == 19) {
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

// Getter for the size of the stack

int StackGetSize(const struct Stack *stack) {
    return (stack->currentItemIndex + 1); 
}