// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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
            printf("%fl     ", mat[row][columns]);
        }
        printf("\n");
    }
}

int MatrixEquals(float mat1[3][3], float mat2[3][3]) {
    // initializing the rows, columns, and truth detector
    for (int row = 0; row < 3; row++) {
        for (int column = 0; column < 3; column++) {
            if (mat1[row][column]>(mat2[row][column] + FP_DELTA) ||
                    mat1[row][column]<(mat2[row][column] - FP_DELTA)) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    for (int row = 0; row < 3; row++) {
        for (int column = 0; column < 3; column++) {
            result[row][column] = mat1[row][column] + mat2[row][column];
        }
    }
}

void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    for (int row = 0; row < 3; row++) {
        for (int column = 0; column < 3; column++) {
            // initialize the result
            result[row][column] = 0;
            for (int c = 0; c < 3; c++)
                result[row][column] += mat1[row][c] * mat2[c][column];
        }
    }
}

void MatrixScalarAdd(float x, float mat[3][3], float result[3][3]) {
    for (int row = 0; row < 3; row++) {
        for (int column = 0; column < 3; column++) {
            result[row][column] = mat[row][column] + x;
        }
    }
}

void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3]) {
    for (int row = 0; row < 3; row++) {
        for (int column = 0; column < 3; column++) {
            result[row][column] = mat[row][column] * x;
        }
    }
}

float MatrixTrace(float mat[3][3]) {
    float trace = 0;
    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < 3; column++) {
            if (row == column) {
                trace = trace + mat[row][column];
            }
        }
    }
    return trace;
}

void MatrixTranspose(float mat[3][3], float result[3][3]) {
    // switches the order of the values in the matrix
    for (int row = 0; row < 3; row++) {
        for (int column = 0; column < 3; column++) {
            result[row][column] = mat[column][row];
        }
    }
}

void MatrixSubmatrix(int i, int j, float mat[3][3], float result[2][2]) {
    int k, m, column;
    int row = 0;
    for (k = 0; k < 3; k++) {
        if (k == i) {
            continue;
        }
        column = 0;
        for (m = 0; m < 3; m++) {
            if (m == j) {
                continue;
            }
            result[row][column] = mat[k][m];
            column++;
        }
        row++;
    }
}

float MatrixDeterminant(float mat[3][3]) {
    float deter = mat[0][0] * ((mat[1][1] * mat[2][2]) - (mat[2][1] * mat[1][2]))
            - mat[0][1] * (mat[1][0]
            * mat[2][2] - mat[2][0] * mat[1][2]) + mat[0][2] * (mat[1][0] *
            mat[2][1] - mat[2][0] * mat[1][1]);
    return deter;
}

void MatrixInverse(float mat[3][3], float result[3][3]) {
    float deter = MatrixDeterminant(mat);
    float cofac[3][3] = {
        {},
        {},
        {}
    };
    float ressywessy[2][2] = {
        {},
        {}
    };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            MatrixSubmatrix(i, j, mat, ressywessy);
            float deter2 = ressywessy[0][0] * ressywessy[1][1] - ressywessy[1][0] * ressywessy[0][1];
            if ((i + j) % 2 == 0) {
                cofac[i][j] = deter2;
            } else {
                cofac[i][j] = -deter2;
            }
        }
    }
    float trans[3][3] = {
        {},
        {},
        {}
    };
    MatrixTranspose(cofac, trans);
    MatrixScalarMultiply(1 / deter, trans, result);
}
