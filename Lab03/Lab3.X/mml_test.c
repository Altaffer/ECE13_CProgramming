// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// User libraries:
#include "MatrixMath.h"


int TestsPassed, PassedNumber;

int main()
{
    BOARD_Init();
    TestsPassed = 0
            
    // Empty matrices for function results
    float result2[2][2] = {
        {},
        {}
    };
    float result3[3][3] = {
        {},
        {},
        {}
    };
    
    
    
    // Matrix inputs for tests
    float matrixA[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    float matrixB[3][3] = {
        {2, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    float matrixC[3][3] = {
        {1.00005, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    float matrixD[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    float matrixE[3][3] = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };
    float matrixF[3][3] = {
        {-1, -2, -3},
        {-4, -5, -6},
        {-7, -8, -9}
    };
    float matrixG[3][3] = {
        {1.5, 2.5, 3.5},
        {4.5, 5.5, 6.5},
        {7.5, 8.5, 9.5}
    };
    float matrixH[3][3] = {
        {0.011, 0.022, 0.033},
        {0.044, 0.055, 0.066},
        {0.077, 0.088, 0.099}
    };
    float matrixI[3][3] = {
        {1, 2, 3},
        {6, 6, 6},
        {2, 4, 2}
    };
    float matrixJ[3][3] = {
        {1.5, 3, 6},
        {6, 1.5, 6},
        {8, 10.5, 2}
    };
    float matrixK[3][3] = {
        {1, 2, 3},
        {2, 3, 4},
        {4, 2, 1}
    };
    float matrixL[3][3] = {
        {1.099, 2, 3},
        {2, 3.099, 4},
        {4, 2, 1.099}
    };
    float matrixM[3][3] = {
        {10, 20, 10},
        {4, 5, 6},
        {2, 3, 5}
    };
    float matrixN[3][3] = {
        {3, 2, 4},
        {3, 3, 9},
        {4, 4, 2}
    };
    float matrixO[3][3] = {
        {1, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    float matrixP[3][3] = {
        {1, 2, 3},
        {0, 1, 4},
        {5, 6, 0}
    };
    float matrixQ[3][3] = {
        {3, 0, 2},
        {2, 0, -2},
        {0, 1, 1}
    };
    float matrixR[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    
    
    // Expected Results for matrices 
    float matrix1[3][3] = {
        {3, 2, 2},
        {2, 2, 2},
        {2, 2, 2}
    };
    float matrix2[3][3] = {
        {10, 10, 10},
        {10, 10, 10},
        {10, 10, 10}
    };
    float matrix3[3][3] = {
        {2, 3, 4},
        {5, 6, 7},
        {8, 9, 10}
    };
    float matrix4[3][3] = {
        {-10, -20, -30},
        {-40, -50, -60},
        {-70, -80, -90}
    };
    float matrix5[3][3] = {
        {7.5, 12.5, 17.5},
        {22.5, 27.5, 32.5},
        {37.5, 42.5, 47.5}
    };
    float matrix6[3][3] = {
        {1.1, 2.2, 3.3},
        {4.4, 5.5, 6.6},
        {7.7, 8.8, 9.9}
    };
    float matrix7[3][3] = {
        {1, 4, 7},
        {2, 5, 8},
        {3, 6, 9}
    };
    float matrix8[2][2] = {
        {1, 2},
        {7, 8}
    };
    float matrix9[3][3] = {
        {5, -4, 1},
        {-14, 11, -2},
        {8, -6, 1}
    };
    float matrix10[3][3] = {
        {1.011, 1.022, 1.033},
        {1.044, 1.055, 1.066},
        {1.077, 1.088, 1.099}
    };
    float matrix11[3][3] = {
        {30, 24, 18},
        {84, 69, 54},
        {138, 114, 90}
    };
    float matrix12[3][3] = {
        {130, 120, 240},
        {51, 47, 73},
        {35, 33, 45}
    };
    float matrix13[3][3] = {
        {1, 0, 0},
        {4, 0, 0},
        {7, 0, 0}
    };
    float matrix14[3][3] = {
        {-24, 18, 5},
        {20, -15, -4},
        {-5, 4, 1}
    };
    float matrix15[3][3] = {
        {0.2, 0.2, 0},
        {-0.2, 0.3, 1},
        {0.2, -0.3, 0}
    };
    float matrix16[3][3] = {
        {1.5, 4.5, 7.5},
        {2.5, 5.5, 8.5},
        {3.5, 6.5, 9.5}
    };
    float matrix17[3][3] = {
        {1.5, 6, 8},
        {3, 1.5, 10.5},
        {6, 6, 2}
    };
    float matrix18[2][2] = {
        {2, 4},
        {4, 1.099}
    };
    float matrix19[2][2] = {
        {0.022, 0.033},
        {0.055, 0.066}
    };
    float matrix20[3][3] = {
        {110, 120, 110},
        {104, 105, 106},
        {102, 103, 105}
    };
    float matrix21[3][3] = {
        {-50, -100, -150},
        {-200, -250, -300},
        {-350, -400, -450}
    };
    float matrix22[3][3] = {
        {-1, -4, -7},
        {-2, -5, -8},
        {-3, -6, -9}
    };
    float matrix23[2][2] = {
        {2, -2},
        {0, 1}
    };

    printf("Beginning taltaffe's mml test harness, compiled on %s %s\n", __DATE__, __TIME__);
    
    // Start Testing
    
    // TESTING MATRIX PRINT
    printf("\nExpected Output of MatrixPrint():\n");
    printf(" [0][0]:    1.1000  [0][1]:    2.2000  [0][2]:    3.3000 \n|");
    printf(" [1][0]:    4.4000  [1][1]:    5.5000  [1][2]:    6.6000 \n");
    printf(" [2][0]:    7.7000  [2][1]:    8.8000  [2][2]:    9.9000 \n");

    // Calling and printing result of MatrixPrint function
    printf("\nOutput of MatrixPrint():\n");
    MatrixPrint(matrix6);
    printf("\n-------------------------------------\n");
    
    // TESTING MATRIX EQUALS
    PassedNumber = 0;
    // Test 1
    if (MatrixEquals(matrixA, matrixA)) PassedNumber++;
    // Test 2
    if (!MatrixEquals(matrixB, matrixA)) PassedNumber++;
    // Test 3
    if (MatrixEquals(matrixA, matrixC)) PassedNumber++;
    // Test 4
    if (!MatrixEquals(matrixD, matrixE)) PassedNumber++;

    if (PassedNumber >= 4) {
        printf("PASSED (%d/4) : MatrixEquals\n", PassedNumber);
        TestsPassed++;
    } else {
        printf("FAILED (%d/4) : MatrixEquals\n", PassedNumber);
    }

    // Add more tests here!
    BOARD_End();
    while (1);
}

