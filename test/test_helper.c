#include <math.h>
#include <stdio.h>
#include "test_helper.h"

int testMatrixEqualityInt(matrix_t *A, matrix_t *B) {
    if (A->numRows != B->numRows || A->numCols != B->numCols) {
        return 0;
    }
    char *aData = (char *)A->data;
    char *bData = (char *)B->data;
    for (size_t i = 0; i < A->numRows; i++) {
        for (size_t j = 0; j < A->numCols; j++) {
            if (!(*((int *)(aData + calculateOffset(A,i,j))) == *((int *)(aData + calculateOffset(B,i,j))))) {
                return 0;
            }
        }
    }
    return 1;
}

int testMatrixEqualityFloat(matrix_t *A, matrix_t *B, double eps) {
    if (A->numRows != B->numRows || A->numCols != B->numCols) {
        return 0;
    }
    char *aData = (char *)A->data;
    char *bData = (char *)B->data;
    for (size_t i = 0; i < A->numRows; i++) {
        for (size_t j = 0; j < A->numCols; j++) {
            float diff = *((float *)(aData + calculateOffset(A,i,j))) - *((float *)(aData + calculateOffset(B,i,j)));
            if (fabs(diff) > (float)eps) {
                return 0;
            }
        }
    }
    return 1;
}

int testMatrixEqualityDouble(matrix_t *A, matrix_t *B, double eps) {
    if (A->numRows != B->numRows || A->numCols != B->numCols) {
        return 0;
    }
    char *aData = (char *)A->data;
    char *bData = (char *)B->data;
    for (size_t i = 0; i < A->numRows; i++) {
        for (size_t j = 0; j < A->numCols; j++) {
            double diff = *((double *)(aData + calculateOffset(A,i,j))) - *((double *)(aData + calculateOffset(B,i,j)));
            if (fabs(diff) > eps) {
                return 0;
            }
        }
    }
    return 1;
}

int testMatrixEquality(matrix_t *A, matrix_t *B, double eps) {
    if (A->numRows != B->numRows || A->numCols != B->numCols) {
        return 0;
    }
    if (A->dtype != B->dtype) {
        fprintf(stderr, "Dytpes of inputs do not match.");
        exit(1);
    }
    switch (A->dtype) {
        case DTYPE_INT:
            return testMatrixEqualityInt(A, B);
        case DTYPE_FLOAT:
            return testMatrixEqualityFloat(A, B, eps);
        case DTYPE_DOUBLE:
            return testMatrixEqualityDouble(A, B, eps);
        default:
            fprintf(stderr, "Unsupported dtype.");
            exit(1);
    }
}