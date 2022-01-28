#include "stack.h"

// Creating function to inialize the Stack
void StackInit(struct Stack *stack) {
    stack->currentItemIndex = -1; 
    stack->initialized = TRUE;
}

// Function to push a value onto teh stack
int StackPush(struct Stack *stack, double value){
    &stack->currentItemIndex += 1;
    &stack->stackItems[stack->currentItemIndex] = value;
}

// function to remove element from stack
int StackPop(struct Stack *stack, double *value){
    &value = stack->stackItems[stack->currentItemIndex];
    &stack->stackItems[stack->currentItemIndex] = NULL;
    &stack->currentItemIndex -= 1;
}

// Checks whether stack is empy or not
int StackIsEmpty(const struct Stack *stack){
    if(&stack->currentItemIndex == -1 ){
        return SUCCESS;
    }
    else{
        return NO_SUCCESS
    }
}

// Checks if Stack is full or not 
int StackIsFull(const struct Stack *stack){
    
}

// Getter for the zie of the stack
int StackGetSize(const struct Stack *stack){
    
}