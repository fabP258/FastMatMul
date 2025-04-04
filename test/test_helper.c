#include <math.h>
#include <stdio.h>
#include "test_helper.h"

int testMatrixEqualityInt(matrix_t *A, matrix_t *B) {
    if (A->numRows != B->numRows || A->numCols != B->numCols) {
        return 0;
    }
    int *aData = (int *)A->data;
    int *bData = (int *)B->data;
    for (size_t i = 0; i < A->numRows; i++) {
        for (size_t j = 0; j < A->numCols; j++) {
            if (!(aData[calculateIndex(A,i,j)] == bData[calculateIndex(B,i,j)])) {
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
    float *aData = (float *)A->data;
    float *bData = (float *)B->data;
    for (size_t i = 0; i < A->numRows; i++) {
        for (size_t j = 0; j < A->numCols; j++) {
            float diff = aData[calculateIndex(A,i,j)] - bData[calculateIndex(B,i,j)];
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
    double *aData = (double *)A->data;
    double *bData = (double *)B->data;
    for (size_t i = 0; i < A->numRows; i++) {
        for (size_t j = 0; j < A->numCols; j++) {
            double diff = aData[calculateIndex(A,i,j)] - bData[calculateIndex(B,i,j)];
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