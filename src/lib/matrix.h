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
    size_t strides[2];
    MatrixDType dtype;
} matrix_t;

matrix_t createMatrix(size_t numRows, size_t numCols, MatrixDType dtype);
void initMatrix(matrix_t *matrix, size_t numRows, size_t numCols, MatrixDType dtype);
void freeMatrix(matrix_t *matrix);

// data type conversions
void castMatrixTo(matrix_t *matrix, MatrixDType dtype);
void castIntMatrixToFloatMatrix(matrix_t *srcMatrix, matrix_t *dstMatrix);
void castIntMatrixToDoubleMatrix(matrix_t *srcMatrix, matrix_t *dstMatrix);
void castFloatMatrixToIntMatrix(matrix_t *srcMatrix, matrix_t *dstMatrix);
void castFloatMatrixToDoubleMatrix(matrix_t *srcMatrix, matrix_t *dstMatrix);
void castDoubleMatrixToIntMatrix(matrix_t *srcMatrix, matrix_t *dstMatrix);
void castDoubleMatrixToFloatMatrix(matrix_t *srcMatrix, matrix_t *dstMatrix);

// helper
static inline size_t calculateIndex(matrix_t *matrix, size_t rowIdx, size_t colIdx) {
    return rowIdx * matrix->numCols + colIdx;
}