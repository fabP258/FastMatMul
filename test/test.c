#include <stdio.h>
#include "matrix.h"
#include "test_linalg.h"
#include "test_matrix.h"

void printIntMatrix(matrix_t *matrix) {
    for (int i = 0; i < matrix->numRows; i++) {
        for (int j = 0; j < matrix->numCols; j++) {
            printf("%d ", ((int *)matrix->data)[i * matrix->numCols + j]);
        }
        printf("\n");
    }
}

void printFloatMatrix(matrix_t *matrix) {
    for (int i = 0; i < matrix->numRows; i++) {
        for (int j = 0; j < matrix->numCols; j++) {
            printf("%.2f ", ((float *)matrix->data)[i * matrix->numCols + j]);
        }
        printf("\n");
    }
}

void printDoubleMatrix(matrix_t *matrix) {
    for (int i = 0; i < matrix->numRows; i++) {
        for (int j = 0; j < matrix->numCols; j++) {
            printf("%.2f ", ((double *)matrix->data)[i * matrix->numCols + j]);
        }
        printf("\n");
    }
}

void printMatrix(matrix_t *matrix) {
    switch (matrix->dtype) {
        case DTYPE_INT:
            printIntMatrix(matrix);
            break;
        case DTYPE_FLOAT:
            printFloatMatrix(matrix);
            break;
        case DTYPE_DOUBLE:
            printDoubleMatrix(matrix);
            break;
        default:
            fprintf(stderr, "Unsupported dtype.");
            exit(1);
    }
}

int main() {
    testMatrix();
    testLinalg();
    return 0;
}