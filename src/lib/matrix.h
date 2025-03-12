#pragma once

#include <stdlib.h>

#define DATA_TYPE float

typedef struct {
    DATA_TYPE *data;
    size_t numRows;
    size_t numCols;
} matrix_t;

matrix_t createMatrix(size_t numRows, size_t numCols, DATA_TYPE value);
void initMatrix(matrix_t *matrix, size_t numRows, size_t numCols, DATA_TYPE value);
void freeMatrix(matrix_t *matrix);

// helper
size_t calculateIndex(matrix_t *matrix, size_t rowIdx, size_t colIdx);