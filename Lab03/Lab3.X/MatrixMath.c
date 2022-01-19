// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// User libraries:
#include "MatrixMath.h"

void MatrixPrint(float mat[3][3]){
    int row, columns;
for (row=0; row<3; row++)
{
    for(columns=0; columns<3; columns++)
    {
         printf("%d     ", mat[row][columns]);
    }
    printf("\n");
}
    printf("%4c %7c\n", mat, 'F');
}

int MatrixEquals(float mat1[3][3], float mat2[3][3]);

void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3]);

void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]);

void MatrixScalarAdd(float x, float mat[3][3], float result[3][3]);

void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3]);

float MatrixTrace(float mat[3][3]);

void MatrixTranspose(float mat[3][3], float result[3][3]);

void MatrixSubmatrix(int i, int j, float mat[3][3], float result[2][2]);

float MatrixDeterminant(float mat[3][3]);

void MatrixInverse(float mat[3][3], float result[3][3]);

