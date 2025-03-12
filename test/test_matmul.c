#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"
#include "linalg.h"

#define EPS 1e-6
#define NUM_ALGORITHMS 2

int testMatrixEquality(matrix_t *A, matrix_t *B, DATA_TYPE eps) {
    if (A->numRows != B->numRows || A->numCols != B->numCols) {
        return 0;
    }
    for (size_t i = 0; i < A->numRows; i++) {
        for (size_t j = 0; j < A->numCols; j++) {
            if (fabs(A->data[calculateIndex(A,i,j)] - B->data[calculateIndex(B,i,j)]) > eps) {
                return 0;
            }
        }
    }
    return 1;
}

void printMatrix(matrix_t *matrix) {
    for (int i = 0; i < matrix->numRows; i++) {
        for (int j = 0; j < matrix->numCols; j++) {
            printf("%.2f ", matrix->data[i * matrix->numCols + j]);
        }
        printf("\n");
    }
}

void testMatmul() {
    matrix_t A = createMatrix(3U, 4U, (DATA_TYPE)0);
    assert(A.data != NULL);
    matrix_t B = createMatrix(4U, 5U, (DATA_TYPE)0);
    assert(B.data != NULL);

    // Fill with dummy values
    A.data[0] = (DATA_TYPE)4;
    A.data[1] = (DATA_TYPE)2;
    A.data[2] = (DATA_TYPE)4;
    A.data[3] = (DATA_TYPE)0;

    A.data[4] = (DATA_TYPE)0;
    A.data[5] = (DATA_TYPE)2;
    A.data[6] = (DATA_TYPE)1;
    A.data[7] = (DATA_TYPE)0;

    A.data[8] = (DATA_TYPE)0;
    A.data[9] = (DATA_TYPE)1;
    A.data[10] = (DATA_TYPE)2;
    A.data[11] = (DATA_TYPE)1;

    printf("Input matrix A with shape [%lu,%lu]: \n", A.numRows, A.numCols);
    printMatrix(&A);

    B.data[0] = (DATA_TYPE)4;
    B.data[1] = (DATA_TYPE)2;
    B.data[2] = (DATA_TYPE)2;
    B.data[3] = (DATA_TYPE)3;
    B.data[4] = (DATA_TYPE)2;
    B.data[5] = (DATA_TYPE)1;
    B.data[6] = (DATA_TYPE)2;
    B.data[7] = (DATA_TYPE)1;
    B.data[8] = (DATA_TYPE)0;
    B.data[9] = (DATA_TYPE)0;
    B.data[10] = (DATA_TYPE)1;
    B.data[11] = (DATA_TYPE)2;
    B.data[12] = (DATA_TYPE)3;
    B.data[13] = (DATA_TYPE)1;
    B.data[14] = (DATA_TYPE)2;
    B.data[15] = (DATA_TYPE)3;
    B.data[16] = (DATA_TYPE)1;
    B.data[17] = (DATA_TYPE)1;
    B.data[18] = (DATA_TYPE)2;
    B.data[19] = (DATA_TYPE)4;

    printf("Input matrix B with shape [%lu,%lu]: \n", B.numRows, B.numCols);
    printMatrix(&B);

    // Expected matrix
    matrix_t expectedResult = createMatrix(3U, 5U, (DATA_TYPE)0);
    assert(expectedResult.data != NULL);
    expectedResult.data[0] = (DATA_TYPE)22;
    expectedResult.data[1] = (DATA_TYPE)20;
    expectedResult.data[2] = (DATA_TYPE)22;
    expectedResult.data[3] = (DATA_TYPE)16;
    expectedResult.data[4] = (DATA_TYPE)16;

    expectedResult.data[5] = (DATA_TYPE)3;
    expectedResult.data[6] = (DATA_TYPE)6;
    expectedResult.data[7] = (DATA_TYPE)5;
    expectedResult.data[8] = (DATA_TYPE)1;
    expectedResult.data[9] = (DATA_TYPE)2;

    expectedResult.data[10] = (DATA_TYPE)6;
    expectedResult.data[11] = (DATA_TYPE)7;
    expectedResult.data[12] = (DATA_TYPE)8;
    expectedResult.data[13] = (DATA_TYPE)4;
    expectedResult.data[14] = (DATA_TYPE)8;

    EMatmulAlgorithm algorithms[NUM_ALGORITHMS] = { NAIVE, OPTIMIZED_LOOP_ORDER };
    
    matrix_t C;
    for (size_t i = 0; i < NUM_ALGORITHMS; i++) {
        C = matmul(&A, &B, algorithms[i]);
        assert(C.data != NULL);

        printf("Calculated GEMM: \n");
        printMatrix(&C);

        printf("\n");
        printf("Expected result: \n");
        printMatrix(&expectedResult);

        assert(testMatrixEquality(&C, &expectedResult, (DATA_TYPE)EPS));
        printf("[TEST: Matmul with algorithm %d] PASSED\n", algorithms[i]);
    }

    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
}

int main() {
    testMatmul();
    return 0;
}