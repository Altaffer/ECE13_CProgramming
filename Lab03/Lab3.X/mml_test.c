// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// User libraries:
#include "MatrixMath.h"


// Module-level variables:
float zero_matrix[3][3] = {{},{},{}};

int main()
{
    BOARD_Init();

    printf("Beginning CRUZID's mml test harness, compiled on %s %s\n", __DATE__, __TIME__);
    
    printf("Demonstrating MatrixPrint():\n");
    MatrixPrint(zero_matrix);

    // Add more tests here!
    BOARD_End();
    while (1);
}

