/*
 * File:   part0.c
 * Author: Luca (taltafe@ucsc.edu)
 *
 * Created on January 5, 2022, 5:31 PM
 */ 


#include <stdio.h>
#include <stdlib.h>

#include "BOARD.h"

int main(void) {
    BOARD_Init();
    printf("Hello World\n");
    
    BOARD_End();
    while(1);
}
