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

// HELPER FUNCTIONS
int FloatEquals(float x, float y) {
    if (x > (y + FP_DELTA) || x < (y - FP_DELTA)) {
        return FALSE;
    }
    return TRUE;
}
int MatrixEquals2x2(float mat1[2][2], float mat2[2][2]) {
    int i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            if (mat1[i][j]>(mat2[i][j] + FP_DELTA) ||
                    mat1[i][j]<(mat2[i][j] - FP_DELTA)) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

int main()
{
    BOARD_Init();
    TestsPassed = 0;
            
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
    
    // Scalars for functions 
    float g1 = 1;
    float g2 = 5;
    float g3 = 10;
    float g4 = 100;

    // i and j values for submatrix tests
    int i1 = 1;
    int j1 = 2;
    int i2 = 0;
    int j2 = 1;
    int i3 = 2;
    int j3 = 0;


    // Expected results for Determinant tests
    float x1 = 24;
    float x2 = 324;
    float x3 = 10;
    float x4 = 1;

    // Expected results for Trace tests
    float t1 = 82.5; //trace value for matrix5
    float t2 = 0.165; //trace value for matrixH
    float t3 = 5.297; //trace value for matrixL
    float t4 = -15; //trace value for matrix

    printf("Beginning taltaffe's mml test harness, compiled on %s %s\n", __DATE__, __TIME__);
    
    // Start Testing
    
    // TESTING MATRIX PRINT
    printf("\nExpected Output of MatrixPrint():\n");
    printf(" [0][0]:    1.1000  [0][1]:    2.2000  [0][2]:    3.3000 \n");
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

    // TESTING MATRIX ADD
    PassedNumber = 0;
    // Test 1 
    MatrixAdd(matrixA, matrixB, result3);
    if (MatrixEquals(result3, matrix1)) PassedNumber++;
    // Test 2
    MatrixAdd(matrixD, matrixE, result3);
    if (MatrixEquals(result3, matrix2)) PassedNumber++;
    // Test 3
    MatrixAdd(matrixA, matrixH, result3);
    if (MatrixEquals(result3, matrix10)) PassedNumber++;
    // Test 4 - Testing property A+B == B+A;
    MatrixAdd(matrixE, matrixD, result3);
    if (MatrixEquals(result3, matrix2)) PassedNumber++;


    if (PassedNumber >= 4) {
        printf("PASSED (%d/4) : MatrixAdd\n", PassedNumber);
        TestsPassed++;
    } else {
        printf("FAILED (%d/4) : MatrixAdd\n", PassedNumber);
    }

    // TESTING MATRIX MULTIPLY
    PassedNumber = 0;

    // Test 1 
    MatrixMultiply(matrixD, matrixE, result3);
    if (MatrixEquals(result3, matrix11)) PassedNumber++;

    // Test 2 
    MatrixMultiply(matrixM, matrixN, result3);
    if (MatrixEquals(result3, matrix12)) PassedNumber++;

    // Test 3
    MatrixMultiply(matrixD, matrixO, result3);
    if (MatrixEquals(result3, matrix13)) PassedNumber++;

    // Test 4 - Test to confirm that A*B != B*A
    MatrixMultiply(matrixE, matrixD, result3);
    if (!MatrixEquals(result3, matrix11)) PassedNumber++;

    if (PassedNumber >= 4) {
        printf("PASSED (%d/4) : MatrixMultiply\n", PassedNumber);
        TestsPassed++;
    } else {
        printf("FAILED (%d/4) : MatrixMultiply\n", PassedNumber);
    }
    
    // TESTING SCALR ADD
    PassedNumber = 0;
    // Test 1
    MatrixScalarAdd(g1, matrixD, result3);
    if (MatrixEquals(result3, matrix3)) PassedNumber++;

    // Test 2
    MatrixScalarAdd(g2, matrixF, result3);
    if (!MatrixEquals(result3, matrixE)) PassedNumber++;

    // Test 3
    MatrixScalarAdd(g3, matrixF, result3);
    if (MatrixEquals(result3, matrixE)) PassedNumber++;

    // Test 4
    MatrixScalarAdd(g4, matrixM, result3);
    if (MatrixEquals(result3, matrix20)) PassedNumber++;


    if (PassedNumber >= 3) {
        printf("PASSED (%d/4) : MatrixScalarAdd\n", PassedNumber);
        TestsPassed++;
    } else {

        printf("FAILED (%d/4) : MatrixScalarAdd\n", PassedNumber);
    }

    // TESTING SCALAR MULTIPLY
    PassedNumber = 0;
    // Test 1
    MatrixScalarMultiply(g3, matrixF, result3);
    if (MatrixEquals(result3, matrix4)) PassedNumber++;

    // Test 2
    MatrixScalarMultiply(g2, matrixG, result3);
    if (MatrixEquals(result3, matrix5)) PassedNumber++;

    // Test 3
    MatrixScalarMultiply(g4, matrixH, result3);
    if (MatrixEquals(result3, matrix6)) PassedNumber++;

    // Test 4
    MatrixScalarMultiply(g2, matrix4, result3);
    if (MatrixEquals(result3, matrix21)) PassedNumber++;

    if (PassedNumber >= 4) {
        printf("PASSED (%d/4) : MatrixScalarMultiply\n", PassedNumber);
        TestsPassed++;
    } else {
        printf("FAILED (%d/4) : MatrixScalarMultiply\n", PassedNumber);
    }

    // TESTING TRACE
    PassedNumber = 0;
    // Test 1
    float t = 0;
    t = MatrixTrace(matrix5);
    if (FloatEquals(t, t1)) PassedNumber++;

    // Test 2
    float s = 0;
    s = MatrixTrace(matrixH);
    if (FloatEquals(s, t2)) PassedNumber++;

    // Test 3
    float r = 0;
    r = MatrixTrace(matrixL);
    if (FloatEquals(r, t3)) PassedNumber++;

    // Test 4
    float q = 0;
    q = MatrixTrace(matrixF);
    if (FloatEquals(q, t4)) PassedNumber++;


    if (PassedNumber >= 4) {
        printf("PASSED (%d/4) : MatrixTrace\n", PassedNumber);
        TestsPassed++;
    } else {
        printf("FAILED (%d/4) : MatrixTrace\n", PassedNumber);
    }
    
    
    // TESTING TRANSPOSE
    PassedNumber = 0;
    // Test 1
    MatrixTranspose(matrixD, result3);
    if (MatrixEquals(result3, matrix7)) PassedNumber++;

    // Test 2
    MatrixTranspose(matrixG, result3);
    if (MatrixEquals(result3, matrix16)) PassedNumber++;

    // Test 3
    MatrixTranspose(matrixJ, result3);
    if (MatrixEquals(result3, matrix17)) PassedNumber++;
    
    // Test 4
    MatrixTranspose(matrixF, result3);
    if (MatrixEquals(result3, matrix22)) PassedNumber++;

    if (PassedNumber >= 4) {
        printf("PASSED (%d/4) : MatrixTranspose\n", PassedNumber);
        TestsPassed++;
    } else {
        printf("FAILED (%d/4) : MatrixTranspose\n", PassedNumber);
    }
    
    
    // TESTING SUBMATRIX 
    PassedNumber = 0;
    // Test 1
    MatrixSubmatrix(i1, j1, matrixD, result2);
    if (MatrixEquals2x2(result2, matrix8)) PassedNumber++;

    // Test 2
    MatrixSubmatrix(i2, j2, matrixL, result2);
    if (MatrixEquals2x2(result2, matrix18)) PassedNumber++;

    // Test 3
    MatrixSubmatrix(i3, j3, matrixH, result2);
    if (MatrixEquals2x2(result2, matrix19)) PassedNumber++;
    
    // Test 4
    MatrixSubmatrix(i2, j2, matrixQ, result2);
    if (MatrixEquals2x2(result2, matrix23)) PassedNumber++;

    if (PassedNumber >= 4) {
        printf("PASSED (%d/4) : MatrixSubmatrix\n", PassedNumber);
        TestsPassed++;
    } else {
        printf("FAILED (%d/4) : MatrixSubmatrix\n", PassedNumber);
    }
    
    // TESTING DETERMINANT
    PassedNumber = 0;
    // Test 1
    int y = MatrixDeterminant(matrixI);
    if (FloatEquals(x1, y)) PassedNumber++;
    // Test 2
    y = MatrixDeterminant(matrixJ);
    if (FloatEquals(x2, y)) PassedNumber++;

    // Test 3
    y = MatrixDeterminant(matrixQ);
    if (FloatEquals(x3, y)) PassedNumber++;
    
    // Test 4
    y = MatrixDeterminant(matrixP);
    if (FloatEquals(x4, y)) PassedNumber++;

    if (PassedNumber >= 4) {
        printf("PASSED (%d/4) : MatrixDeterminant\n", PassedNumber);
        TestsPassed++;
    } else {
        printf("FAILED (%d/4) : MatrixDeterminant\n", PassedNumber);
    }
    
    // TESTING INVERSE
    PassedNumber = 0;
    //Test 1
    MatrixInverse(matrixK, result3);
    if (MatrixEquals(result3, matrix9)) PassedNumber++;
    // Test 2
    MatrixInverse(matrixP, result3);
    if (MatrixEquals(result3, matrix14)) PassedNumber++;

    // Test 3
    MatrixInverse(matrixQ, result3);
    if (MatrixEquals(result3, matrix15)) PassedNumber++;

    // Test 4
    MatrixInverse(matrixR, result3);
    if (MatrixEquals(result3, matrixR)) PassedNumber++;


    if (PassedNumber >= 4) {
        printf("PASSED (%d/4) : MatrixInverse\n", PassedNumber);
        TestsPassed++;
    } else {
        printf("FAILED (%d/4) : MatrixInverse\n", PassedNumber);
    }
    
    
    // PRINTING RESULTS
    printf("-------------------------------------\n");
    printf("%d out of 10 functions passed (%.2f%%)\n", TestsPassed,
            (double) ((TestsPassed * 100) / 10));
    
    // Add more tests here!
    BOARD_End();
    while (1);
}

