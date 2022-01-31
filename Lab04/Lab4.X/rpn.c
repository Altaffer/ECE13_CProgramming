#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "BOARD.h"
#include "stack.h"
#include "rpn.h"

int RPN_Evaluate(char * rpn_string, double * result) {
        int len = strlen(rpn_string);
        char* token = strtok(rpn_string, " ");
        struct Stack stack = {};
        StackInit(&stack);
        if ((token[0] == '+') || (token[0] == '*') ||
                (token[0] == '/')) {
            printf("Error: Cannot start with an operator");
            return RPN_ERROR_INVALID_TOKEN;
        }
        if ((token[0] < 0x30) && (token[0] > 0x39)) {
            printf("Error: Invalid character in first term");
            return RPN_ERROR_INVALID_TOKEN;
        }
        if ((token[2] < 0x30) && (token[2] > 0x39)) {
            printf("Error: Invalid character in second term");
            return RPN_ERROR_INVALID_TOKEN;
        }
        if (len > 40) {
            printf("too many characters");
            RPN_ERROR_STACK_OVERFLOW;
        }
        double counter = 1;
        double counter2 = 0;
        double z = 0;
        double a, b;
//        int decCounter;
        while (token != NULL) {
//            decCounter = 0;
//            for (int i =0; i < len; i++){
//                 if (((token[0] == '-') && (len > 1)) || (isdigit(token[i])) || 
//                         ((token[i] == '.') && (decCounter < 1))) {
//                     if (token[i] == '.') {
//                         decCounter++;
//                     }
//                     continue;
//                 }
//                 else{
//                     return RPN_ERROR_INVALID_TOKEN;
//                 }
//            }
            if (((token[0] >= 0x30) && (token[0] <= 0x39))) {
                StackPush(&stack, atof(token));
            }
            if (token[0] == '+') {
                StackPop(&stack, &a);
                StackPop(&stack, &b);
                z = b + a;
                StackPush(&stack, z);
            }
            if (token[0] == '-') {
                StackPop(&stack, &a);
                StackPop(&stack, &b);
                z = b - a;
                StackPush(&stack, z);
            }
            if (token[0] == '*') {
                StackPop(&stack, &a);
                StackPop(&stack, &b);
                z = b * a;
                StackPush(&stack, z);
            }
            if (token[0] == '/') {
                StackPop(&stack, &a);
                StackPop(&stack, &b);
                if (a == 0) {
                    printf("Cannot divide by 0\n");
                    return RPN_ERROR_DIVIDE_BY_ZERO;
                }
                z = b / a;
                StackPush(&stack, z);
            }
            
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