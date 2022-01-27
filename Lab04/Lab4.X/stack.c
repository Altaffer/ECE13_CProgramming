#include "stack.h"

void StackInit(struct Stack *stack) {
    stack->currentItemIndex = -1; 
    stack->initialized = TRUE;
}

