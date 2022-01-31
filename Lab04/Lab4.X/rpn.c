#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "BOARD.h"
#include "rpn.h"
#include "stack.h"

int RPN_Evaluate(char * rpn_string, double * result) {
    int len = strlen(rpn_string);
    char* token = strtok(rpn_string, " ");
    struct Stack stack = {};
    StackInit(&stack);
    if ((token[0] == '+') || (token[0] == '-') || (token[0] == '*') ||
                (token[0] == '/')) {
            printf("Error: Cannot start with an operator");
            return STANDARD_ERROR;
        }
        if ((token[0] < 0x30) && (token[0] > 0x39)) {
            printf("Error: Invalid character in first term");
            return STANDARD_ERROR;
        }
        if ((token[1] < 0x30) && (token[1] > 0x39)) {
            printf("Error: Invalid character in second term");
            return STANDARD_ERROR;
        }
    while (token != NULL) {
        double counter = 1;
        double counter2 = 0;
        double z = 0;
        printf("token %c\n", token);
//        for (int i = 0; i <= len; i++) {
            if ((token[0] >= 0x30) && (token[0] <= 0x39)) {
                StackPush(&stack, token[0]);
            }
            if (token[0] == '+') {
                double a = StackPop(&stack, &counter2);
                double b = StackPop(&stack, &counter);
                printf("a: %fl, b: %fl\n", a, b);
                counter++;
                counter2++;
                z = a + b;
                StackPush(&stack, z);
            }
            if (token[0] == '-') {
                double a = StackPop(&stack, (&counter2));
                double b = StackPop(&stack, &counter);
                printf("a: %fl, b:%fl\n", a, b);
                counter++;
                counter2++;
                z = a - b;
                StackPush(&stack, z);
            }
            if (token[0] == '*') {
                double a = StackPop(&stack, (&counter2));
                double b = StackPop(&stack, &counter);
                printf("a: %fl, b:%fl\n", a, b);
                counter++;
                counter2++;
                z = a * b;
                StackPush(&stack, z);
            }
            if (token[0] == '/') {
                double a = StackPop(&stack, (&counter2));
                double b = StackPop(&stack, &counter);
                printf("a: %fl, b:%fl\n", a, b);
                counter++;
                counter2++;
                z = a / b;
                StackPush(&stack, z);
            }
//        }
        token = strtok(NULL, " ");
        *result = z;
    }
}

int ProcessBackspaces(char *rpn_sentence) {
    char *i, *j;

    for (i = j = rpn_sentence; *i != '\0'; i++) {
        if (*i == '\b') {
            if (j != rpn_sentence) {
                j--;
            }
            continue;
        }
        *j++ = *i;
    }
    *j = '\0';
    return strlen(rpn_sentence);
}