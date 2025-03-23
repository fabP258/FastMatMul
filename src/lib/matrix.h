#pragma once

#include <stdlib.h>

#define DATA_TYPE float

typedef enum {
    DTYPE_INT,
    DTYPE_FLOAT,
    DTYPE_DOUBLE,
} MatrixDType;

typedef struct {
    void *data;
    size_t numRows;
    size_t numCols;
    size_t stride;
    MatrixDType dtype;
} matrix_t;

matrix_t createMatrix(size_t numRows, size_t numCols, MatrixDType dtype);
void initMatrix(matrix_t *matrix, size_t numRows, size_t numCols, MatrixDType dtype);
void freeMatrix(matrix_t *matrix);

// helper
size_t calculateIndex(matrix_t *matrix, size_t rowIdx, size_t colIdx);