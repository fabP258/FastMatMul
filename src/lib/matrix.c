#include "matrix.h"
#include <stdio.h>

matrix_t createMatrix(size_t numRows, size_t numCols, MatrixDType dtype) {
    matrix_t matrix;
    initMatrix(&matrix, numRows, numCols, dtype);
    return matrix;
}

void initMatrix(matrix_t *matrix, size_t numRows, size_t numCols, MatrixDType dtype) {
    size_t elementSize;
    switch (dtype) {
        case DTYPE_INT:
            elementSize = sizeof(int);
            break;
        case DTYPE_FLOAT:
            elementSize = sizeof(float);
            break;
        case DTYPE_DOUBLE:
            elementSize = sizeof(double);
            break;
        default:
            printf("Error: dtype not allowed.");
            // TODO: Crash properly
            break;
    }
    matrix->stride = elementSize;
    matrix->data = malloc(numRows * numCols * elementSize);
    if (matrix->data == NULL) {
        return;
    }
    matrix->numRows = numRows;
    matrix->numCols = numCols;
}

void freeMatrix(matrix_t *matrix) {
    if (matrix->data == NULL) {
        return;
    }
    free(matrix->data);
    matrix->data = NULL;
    matrix->numRows = 0U;
    matrix->numCols = 0U;
    matrix->stride = 0U;
}

size_t calculateIndex(matrix_t *matrix, size_t rowIdx, size_t colIdx) {
    return rowIdx * matrix->numCols + colIdx;
}