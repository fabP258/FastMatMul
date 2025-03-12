#include "matrix.h"

matrix_t createMatrix(size_t numRows, size_t numCols, DATA_TYPE value) {
    matrix_t matrix;
    initMatrix(&matrix, numRows, numCols, value);
    return matrix;
}

void initMatrix(matrix_t *matrix, size_t numRows, size_t numCols, DATA_TYPE value) {
    matrix->data = (DATA_TYPE *)malloc(numRows * numCols * sizeof(DATA_TYPE));
    if (matrix->data == NULL) {
        return;
    }
    matrix->numRows = numRows;
    matrix->numCols = numCols;
    for (size_t i = 0; i < (numRows * numCols); i++) {
        matrix->data[i] = value;
    }
}

void freeMatrix(matrix_t *matrix) {
    if (matrix->data == NULL) {
        return;
    }
    free(matrix->data);
    matrix->data = NULL;
    matrix->numRows = 0U;
    matrix->numCols = 0U;
}

size_t calculateIndex(matrix_t *matrix, size_t rowIdx, size_t colIdx) {
    return rowIdx * matrix->numCols + colIdx;
}