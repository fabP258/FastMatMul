#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"
#include "linalg.h"

#define EPS 1e-6

void test_matmul() {
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

    // TODO: Iterate over all algorithms
    EMatmulAlgorithm algorithm = NAIVE;
    matrix_t C = matmul(&A, &B, algorithm);
    assert(C.data != NULL);

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

    // Test shape
    assert(C.num_rows == expectedResult.num_rows);
    assert(C.num_cols == expectedResult.num_cols);

    // Check result
    for (size_t i = 0; i < C.num_rows; i++) {
        for (size_t j = 0; j < C.num_cols; j++) {
            assert(fabs(C.data[i][j] - expectedResult.data[i][j]) < EPS);
        }
    }

    printf("[TEST: Matmul] PASSED\n");

    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
}

int main() {
    test_matmul();
    return 0;
}