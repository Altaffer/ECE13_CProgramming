// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// User libraries:
#include "MatrixMath.h"

void MatrixPrint(float mat[3][3]) {
    // initializing the rows and columns
    int row, columns;
    // loops to identify both the rows and columns
    for (row = 0; row < 3; row++) {
        for (columns = 0; columns < 3; columns++) {
            // printing the values in the matrix
            printf("%d     ", mat[row][columns]);
        }
        printf("\n");
    }
}

int MatrixEquals(float mat1[3][3], float mat2[3][3]) {
    // initializing the rows, columns, and truth detector
    int row, column, result;
    result = 1;
    for (row = 0; row < 3; row++) {
        for (column = 0; column < 3; column++) {
            if (mat1[row][column] != mat2[row][column]) {
                result = 0;
                break;
            }
        }
    }
    return result;
}

void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    for (int a = 0; a < 3; a++) {
        for (int b = 0; b < 3; b++) {
            result[a][b] = mat1[a][b] + mat2[a][b];
        }
    }
    return result;
}

void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    for (int a = 0; a < 3; a++) {
        for (int b = 0; b < 3; b++) {
            // initialize the result
            result[a][b] = 0;
            for (int c = 0; c < 3; c++)
                result[a][b] += mat1[a][c] * mat2[c][b];
        }
    }
    return result;
}

void MatrixScalarAdd(float x, float mat[3][3], float result[3][3]){
    for (int a = 0; a < 3; a++) {
        for (int b = 0; b < 3; b++) {
            result[a][b] = mat[a][b] + x;
        }
    }
    return result;
}

void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3]){
    for (int a = 0; a < 3; a++) {
        for (int b = 0; b < 3; b++) {
            // initialize the result
            result[a][b] = 0;
            for (int c = 0; c < 3; c++)
                result[a][b] += mat[a][c] * x;
        }
    }
    return result;
}

float MatrixTrace(float mat[3][3]);

void MatrixTranspose(float mat[3][3], float result[3][3]) {
    // switches the order of the values in the matrix
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = mat[j][i];
        }
    }
}

void MatrixSubmatrix(int i, int j, float mat[3][3], float result[2][2]);

float MatrixDeterminant(float mat[3][3]);

void MatrixInverse(float mat[3][3], float result[3][3]);

