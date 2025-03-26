#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"
#include "linalg.h"

#define EPS 1e-6
#define NUM_ALGORITHMS 3
#define NUM_DTYPES 3

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
            if (fabs(aData[calculateIndex(A,i,j)] - bData[calculateIndex(B,i,j)]) > (float)eps) {
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
            if (fabs(aData[calculateIndex(A,i,j)] - bData[calculateIndex(B,i,j)]) > eps) {
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

void testMatmul() {
    matrix_t A = createMatrix(3U, 4U, DTYPE_INT);
    assert(A.data != NULL);
    matrix_t B = createMatrix(4U, 5U, DTYPE_INT);
    assert(B.data != NULL);

    // Fill with dummy values
    int *aData = (int *)A.data;
    aData[0] = 4;
    aData[1] = 2;
    aData[2] = 4;
    aData[3] = 0;

    aData[4] = 0;
    aData[5] = 2;
    aData[6] = 1;
    aData[7] = 0;

    aData[8] = 0;
    aData[9] = 1;
    aData[10] = 2;
    aData[11] = 1;

    int *bData = (int *)B.data;
    bData[0] = 4;
    bData[1] = 2;
    bData[2] = 2;
    bData[3] = 3;
    bData[4] = 2;
    bData[5] = 1;
    bData[6] = 2;
    bData[7] = 1;
    bData[8] = 0;
    bData[9] = 0;
    bData[10] = 1;
    bData[11] = 2;
    bData[12] = 3;
    bData[13] = 1;
    bData[14] = 2;
    bData[15] = 3;
    bData[16] = 1;
    bData[17] = 1;
    bData[18] = 2;
    bData[19] = 4;

    // Expected matrix
    matrix_t expectedResult = createMatrix(3U, 5U, DTYPE_INT);
    assert(expectedResult.data != NULL);

    int *expResData = (int *)expectedResult.data;
    expResData[0] = 22;
    expResData[1] = 20;
    expResData[2] = 22;
    expResData[3] = 16;
    expResData[4] = 16;

    expResData[5] = 3;
    expResData[6] = 6;
    expResData[7] = 5;
    expResData[8] = 1;
    expResData[9] = 2;

    expResData[10] = 6;
    expResData[11] = 7;
    expResData[12] = 8;
    expResData[13] = 4;
    expResData[14] = 8;
    EMatmulAlgorithm algorithms[NUM_ALGORITHMS] = { NAIVE, OPTIMIZED_LOOP_ORDER, BLAS_GEMM };
    MatrixDType dtypes[3] = { DTYPE_INT, DTYPE_FLOAT, DTYPE_DOUBLE };
    
    matrix_t C;
    for (size_t j = 0; j < NUM_DTYPES; j++) {
        castMatrixTo(&A, j);
        castMatrixTo(&B, j);
        castMatrixTo(&expectedResult, j);
        for (size_t i = 0; i < NUM_ALGORITHMS; i++) {
            C = matmul(&A, &B, algorithms[i]);
            assert(C.data != NULL);
            assert(testMatrixEquality(&C, &expectedResult, EPS));
            printf("[TEST: Matmul with algorithm %d and dtype %d] PASSED\n", algorithms[i], j);
            freeMatrix(&C);
        }
    }
    freeMatrix(&A);
    freeMatrix(&B);
}

int main() {
    testMatmul();
    return 0;
}