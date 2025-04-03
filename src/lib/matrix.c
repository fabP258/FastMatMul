#include "matrix.h"
#include <stdio.h>
#include <assert.h>

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
            fprintf(stderr, "Unsupported dtype.");
            exit(1);
            break;
    }
    matrix->data = calloc(numRows * numCols, elementSize);
    if (matrix->data == NULL) {
        return;
    }
    matrix->numRows = numRows;
    matrix->numCols = numCols;
    matrix->strides[0] = numCols * elementSize;
    matrix->strides[1] = elementSize;
    matrix->dtype = dtype;
}

void freeMatrix(matrix_t *matrix) {
    if (matrix->data == NULL) {
        return;
    }
    free(matrix->data);
    matrix->data = NULL;
    matrix->numRows = 0U;
    matrix->numCols = 0U;
    matrix->strides[0] = 0U;
    matrix->strides[1] = 0U;
}

void castMatrixTo(matrix_t *srcMatrix, MatrixDType dstDtype) {
    assert(srcMatrix != NULL);
    if (srcMatrix->dtype == dstDtype) {
        return;
    }
    matrix_t dstMatrix = createMatrix(srcMatrix->numRows, srcMatrix->numCols, dstDtype);
    assert(dstMatrix.data != NULL);
    switch (srcMatrix->dtype) {
        case (DTYPE_INT):
            switch (dstDtype) {
                case (DTYPE_FLOAT):
                    castIntMatrixToFloatMatrix(srcMatrix, &dstMatrix);
                    break;
                case (DTYPE_DOUBLE):
                    castIntMatrixToDoubleMatrix(srcMatrix, &dstMatrix);
                    break;
                default:
                    fprintf(stderr, "Unsupported destination dtype for source dtype int.");
                    exit(1);
            }
            break;
        case (DTYPE_FLOAT):
            switch (dstDtype) {
                case (DTYPE_INT):
                    castFloatMatrixToIntMatrix(srcMatrix, &dstMatrix);
                    break;
                case (DTYPE_DOUBLE):
                    castFloatMatrixToDoubleMatrix(srcMatrix, &dstMatrix);
                    break;
                default:
                    fprintf(stderr, "Unsupported destination dtype for source dtype float.");
                    exit(1);
            }
            break;
        case (DTYPE_DOUBLE):
            switch (dstDtype) {
                case (DTYPE_INT):
                    castDoubleMatrixToIntMatrix(srcMatrix, &dstMatrix);
                    break;
                case (DTYPE_FLOAT):
                    castDoubleMatrixToFloatMatrix(srcMatrix, &dstMatrix);
                    break;
                default:
                    fprintf(stderr, "Unsupported destination dtype for source dtype double.");
                    exit(1);
            }
            break;
        default:
            fprintf(stderr, "Unsupported source dtype.");
            exit(1);
    }
    free(srcMatrix->data);
    srcMatrix->data = dstMatrix.data;
    srcMatrix->dtype = dstDtype;
}

void castIntMatrixToFloatMatrix(matrix_t *srcMatrix, matrix_t *dstMatrix) {
    assert(srcMatrix->dtype == DTYPE_INT);
    assert(dstMatrix->dtype == DTYPE_FLOAT);
    size_t numElements = srcMatrix->numRows * srcMatrix->numCols;
    for (size_t i = 0; i < numElements; i++) {
        ((float *)dstMatrix->data)[i] = (float)((int *)(srcMatrix->data))[i];
    }
}

void castIntMatrixToDoubleMatrix(matrix_t *srcMatrix, matrix_t *dstMatrix) {
    assert(srcMatrix->dtype == DTYPE_INT);
    assert(dstMatrix->dtype == DTYPE_DOUBLE);
    size_t numElements = srcMatrix->numRows * srcMatrix->numCols;
    for (size_t i = 0; i < numElements; i++) {
        ((double *)dstMatrix->data)[i] = (double)((int *)(srcMatrix->data))[i];
    }
}

void castFloatMatrixToIntMatrix(matrix_t *srcMatrix, matrix_t *dstMatrix) {
    assert(srcMatrix->dtype == DTYPE_FLOAT);
    assert(dstMatrix->dtype == DTYPE_INT);
    size_t numElements = srcMatrix->numRows * srcMatrix->numCols;
    for (size_t i = 0; i < numElements; i++) {
        ((int *)dstMatrix->data)[i] = (int)((float *)(srcMatrix->data))[i];
    }
}

void castFloatMatrixToDoubleMatrix(matrix_t *srcMatrix, matrix_t *dstMatrix) {
    assert(srcMatrix->dtype == DTYPE_FLOAT);
    assert(dstMatrix->dtype == DTYPE_DOUBLE);
    size_t numElements = srcMatrix->numRows * srcMatrix->numCols;
    for (size_t i = 0; i < numElements; i++) {
        ((double *)dstMatrix->data)[i] = (double)((float *)(srcMatrix->data))[i];
    }
}

void castDoubleMatrixToIntMatrix(matrix_t *srcMatrix, matrix_t *dstMatrix) {
    assert(srcMatrix->dtype == DTYPE_DOUBLE);
    assert(dstMatrix->dtype == DTYPE_INT);
    size_t numElements = srcMatrix->numRows * srcMatrix->numCols;
    for (size_t i = 0; i < numElements; i++) {
        ((int *)dstMatrix->data)[i] = (int)((double *)(srcMatrix->data))[i];
    }
}

void castDoubleMatrixToFloatMatrix(matrix_t *srcMatrix, matrix_t *dstMatrix) {
    assert(srcMatrix->dtype == DTYPE_DOUBLE);
    assert(dstMatrix->dtype == DTYPE_FLOAT);
    size_t numElements = srcMatrix->numRows * srcMatrix->numCols;
    for (size_t i = 0; i < numElements; i++) {
        ((float *)dstMatrix->data)[i] = (float)((double *)(srcMatrix->data))[i];
    }
}
