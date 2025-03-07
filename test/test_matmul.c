#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"
#include "linalg.h"

#define EPS 1e-6
#define NUM_ALGORITHMS 2

int testMatrixEquality(matrix_t *A, matrix_t *B, DATA_TYPE eps) {
    if (A->num_rows != B->num_rows || A->num_cols != B->num_cols) {
        return 0;
    }
    for (size_t i = 0; i < A->num_rows; i++) {
        for (size_t j = 0; j < A->num_cols; j++) {
            if (fabs(A->data[i][j] - B->data[i][j]) > eps) {
                return 0;
            }
        }
    }
    return 1;
}

void testMatmul() {
    matrix_t A = createMatrix(3U, 4U, (DATA_TYPE)0);
    assert(A.data != NULL);
    matrix_t B = createMatrix(4U, 5U, (DATA_TYPE)0);
    assert(B.data != NULL);

    // Fill with dummy values
    A.data[0][0] = (DATA_TYPE)4;
    A.data[0][1] = (DATA_TYPE)2;
    A.data[0][2] = (DATA_TYPE)4;
    A.data[0][3] = (DATA_TYPE)0;

    A.data[1][0] = (DATA_TYPE)0;
    A.data[1][1] = (DATA_TYPE)2;
    A.data[1][2] = (DATA_TYPE)1;
    A.data[1][3] = (DATA_TYPE)0;

    A.data[2][0] = (DATA_TYPE)0;
    A.data[2][1] = (DATA_TYPE)1;
    A.data[2][2] = (DATA_TYPE)2;
    A.data[2][3] = (DATA_TYPE)1;

    B.data[0][0] = (DATA_TYPE)4;
    B.data[0][1] = (DATA_TYPE)2;
    B.data[0][2] = (DATA_TYPE)2;
    B.data[0][3] = (DATA_TYPE)3;
    B.data[0][4] = (DATA_TYPE)2;

    B.data[1][0] = (DATA_TYPE)1;
    B.data[1][1] = (DATA_TYPE)2;
    B.data[1][2] = (DATA_TYPE)1;
    B.data[1][3] = (DATA_TYPE)0;
    B.data[1][4] = (DATA_TYPE)0;

    B.data[2][0] = (DATA_TYPE)1;
    B.data[2][1] = (DATA_TYPE)2;
    B.data[2][2] = (DATA_TYPE)3;
    B.data[2][3] = (DATA_TYPE)1;
    B.data[2][4] = (DATA_TYPE)2;

    B.data[3][0] = (DATA_TYPE)3;
    B.data[3][1] = (DATA_TYPE)1;
    B.data[3][2] = (DATA_TYPE)1;
    B.data[3][3] = (DATA_TYPE)2;
    B.data[3][4] = (DATA_TYPE)4;

    // Expected matrix
    matrix_t expectedResult = createMatrix(3U, 5U, (DATA_TYPE)0);
    assert(expectedResult.data != NULL);
    expectedResult.data[0][0] = (DATA_TYPE)22;
    expectedResult.data[0][1] = (DATA_TYPE)20;
    expectedResult.data[0][2] = (DATA_TYPE)22;
    expectedResult.data[0][3] = (DATA_TYPE)16;
    expectedResult.data[0][4] = (DATA_TYPE)16;

    expectedResult.data[1][0] = (DATA_TYPE)3;
    expectedResult.data[1][1] = (DATA_TYPE)6;
    expectedResult.data[1][2] = (DATA_TYPE)5;
    expectedResult.data[1][3] = (DATA_TYPE)1;
    expectedResult.data[1][4] = (DATA_TYPE)2;

    expectedResult.data[2][0] = (DATA_TYPE)6;
    expectedResult.data[2][1] = (DATA_TYPE)7;
    expectedResult.data[2][2] = (DATA_TYPE)8;
    expectedResult.data[2][3] = (DATA_TYPE)4;
    expectedResult.data[2][4] = (DATA_TYPE)8;

    EMatmulAlgorithm algorithms[NUM_ALGORITHMS] = { NAIVE, OPTIMIZED_LOOP_ORDER };
    
    matrix_t C;
    for (size_t i = 0; i < NUM_ALGORITHMS; i++) {
        C = matmul(&A, &B, algorithms[i]);
        assert(C.data != NULL);
        assert(testMatrixEquality(&C, &expectedResult, (DATA_TYPE)EPS));
    }
    printf("[TEST: Matmul] PASSED\n");

    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
}

int main() {
    testMatmul();
    return 0;
}