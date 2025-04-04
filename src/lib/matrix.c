#include "matrix.h"
#include <stdio.h>
#include <assert.h>

matrix_t *createMatrix(size_t numRows, size_t numCols, MatrixDType dtype) {
    matrix_t *matrix = (matrix_t *)malloc(sizeof(matrix_t));
    if (!matrix) {
        return NULL;
    }
    initMatrix(matrix, numRows, numCols, dtype);
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
    matrix->strides[0] = numCols;
    matrix->strides[1] = 1u;
    matrix->dtype = dtype;
    matrix->refcount = 1;
    matrix->base = NULL;
    matrix->ownsData = 1;
}

matrix_t *createView(matrix_t *const matrix) {
    matrix_t *view = (matrix_t *)malloc(sizeof(matrix_t));
    if (!view) {
        return NULL;
    }
    *view = *matrix;
    view->refcount = 1u;
    view->base = matrix;
    view->ownsData = 0;
    matrix->refcount++;
    return view;
}

void freeMatrix(matrix_t *matrix) {
    matrix->refcount--;
    if (matrix->refcount == 0) {
        if (matrix->ownsData && matrix->data != NULL) {
            free(matrix->data);
            matrix->data = NULL;
            matrix->numRows = 0u;
            matrix->numCols = 0u;
            matrix->strides[0] = 0u;
            matrix->strides[1] = 0u;
        }
        if (matrix->base) {
            freeMatrix(matrix->base);
        }
        free(matrix);
    }
}

void castMatrixTo(matrix_t *srcMatrix, MatrixDType dstDtype) {
    assert(srcMatrix != NULL);
    if (srcMatrix->dtype == dstDtype) {
        return;
    }
    matrix_t *dstMatrix = createMatrix(srcMatrix->numRows, srcMatrix->numCols, dstDtype);
    assert(dstMatrix != NULL);
    assert(dstMatrix->data != NULL);
    switch (srcMatrix->dtype) {
        case (DTYPE_INT):
            switch (dstDtype) {
                case (DTYPE_FLOAT):
                    castIntMatrixToFloatMatrix(srcMatrix, dstMatrix);
                    break;
                case (DTYPE_DOUBLE):
                    castIntMatrixToDoubleMatrix(srcMatrix, dstMatrix);
                    break;
                default:
                    fprintf(stderr, "Unsupported destination dtype for source dtype int.");
                    exit(1);
            }
            break;
        case (DTYPE_FLOAT):
            switch (dstDtype) {
                case (DTYPE_INT):
                    castFloatMatrixToIntMatrix(srcMatrix, dstMatrix);
                    break;
                case (DTYPE_DOUBLE):
                    castFloatMatrixToDoubleMatrix(srcMatrix, dstMatrix);
                    break;
                default:
                    fprintf(stderr, "Unsupported destination dtype for source dtype float.");
                    exit(1);
            }
            break;
        case (DTYPE_DOUBLE):
            switch (dstDtype) {
                case (DTYPE_INT):
                    castDoubleMatrixToIntMatrix(srcMatrix, dstMatrix);
                    break;
                case (DTYPE_FLOAT):
                    castDoubleMatrixToFloatMatrix(srcMatrix, dstMatrix);
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
    // transfer ownership to src
    srcMatrix->data = dstMatrix->data;
    srcMatrix->dtype = dstDtype;
    free(dstMatrix);
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
