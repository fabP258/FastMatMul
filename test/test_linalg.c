#include <assert.h>
#include <stdio.h>
#include "test_linalg.h"
#include "test_helper.h"
#include "matrix.h"
#include "linalg.h"

void testLinalg() {
    testMatmul();
    testTranspose();
}

void testMatmulResult(matrix_t *A, matrix_t *B, matrix_t *expectedResult) {
    EMatmulAlgorithm algorithms[NUM_ALGORITHMS] = { NAIVE, OPTIMIZED_LOOP_ORDER, BLAS_GEMM };
    matrix_t *C;
    int srcDtype = A->dtype;
    for (size_t j = 0; j < NUM_DTYPES; j++) {
        castMatrixTo(A, j);
        castMatrixTo(B, j);
        castMatrixTo(expectedResult, j);
        for (size_t i = 0; i < NUM_ALGORITHMS; i++) {
            C = matmul(A, B, algorithms[i]);
            assert(C != NULL);
            assert(C->data != NULL);
            assert(testMatrixEquality(C, expectedResult, EPS));
            printf("[TEST: Matmul with algorithm %d and dtype %d] PASSED\n", algorithms[i], j);
            freeMatrix(C);
        }
    }
    castMatrixTo(A, srcDtype);
    castMatrixTo(B, srcDtype);
    castMatrixTo(expectedResult, srcDtype);
}

void testMatmul() {
    matrix_t *A = createMatrix(3U, 4U, DTYPE_INT);
    assert(A != NULL);
    assert(A->data != NULL);

    matrix_t *B = createMatrix(4U, 5U, DTYPE_INT);
    assert(B != NULL);
    assert(B->data != NULL);

    // Fill with dummy values
    int *aData = (int *)A->data;
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

    int *bData = (int *)B->data;
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
    matrix_t *expectedResult = createMatrix(3U, 5U, DTYPE_INT);
    assert(expectedResult != NULL);
    assert(expectedResult->data != NULL);

    int *expResData = (int *)expectedResult->data;
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

    testMatmulResult(A, B, expectedResult);

    // get A by transposing its transpose
    matrix_t *AT = createMatrix(4U, 3U, DTYPE_INT);
    assert(AT != NULL);
    assert(AT->data != NULL);
    int *atData = (int *)AT->data;
    atData[0] = 4;
    atData[1] = 0;
    atData[2] = 0;

    atData[3] = 2;
    atData[4] = 2;
    atData[5] = 1;

    atData[6] = 4;
    atData[7] = 1;
    atData[8] = 2;

    atData[9] = 0;
    atData[10] = 0;
    atData[11] = 1;

    matrix_t *A_view_transform = transpose(AT);
    assert(testMatrixEquality(A, A_view_transform, EPS));
    testMatmulResult(A_view_transform, B, expectedResult);
    freeMatrix(A);
    freeMatrix(AT);
    freeMatrix(A_view_transform);
    freeMatrix(B);
    freeMatrix(expectedResult);
}

void testTranspose() {
    matrix_t *matrix = createMatrix(2U, 3U, DTYPE_INT);
    assert(matrix != NULL);
    int *mData = (int *)matrix->data;
    mData[calculateIndex(matrix, 0u, 0u)] = 4;
    mData[calculateIndex(matrix, 0u, 1u)] = 2;
    mData[calculateIndex(matrix, 0u, 2u)] = 9;

    mData[calculateIndex(matrix, 1u, 0u)] = 1;
    mData[calculateIndex(matrix, 1u, 1u)] = 3;
    mData[calculateIndex(matrix, 1u, 2u)] = 8;

    matrix_t *expectedResult = createMatrix(3U, 2U, DTYPE_INT);
    assert(expectedResult != NULL);
    int *expData = (int *)expectedResult->data;
    expData[calculateIndex(expectedResult, 0u, 0u)] = 4;
    expData[calculateIndex(expectedResult, 0u, 1u)] = 1;

    expData[calculateIndex(expectedResult, 1u, 0u)] = 2;
    expData[calculateIndex(expectedResult, 1u, 1u)] = 3;

    expData[calculateIndex(expectedResult, 2u, 0u)] = 9;
    expData[calculateIndex(expectedResult, 2u, 1u)] = 8;

    matrix_t *transposedMatrix = transpose(matrix);
    assert(transposedMatrix != NULL);

    assert(matrix->numRows == transposedMatrix->numCols);
    assert(matrix->numCols == transposedMatrix->numRows);
    assert(matrix->strides[0] == transposedMatrix->strides[1]);
    assert(matrix->strides[1] == transposedMatrix->strides[0]);
    assert(testMatrixEquality(transposedMatrix, expectedResult, EPS));
    printf("[TEST: testTranspose] PASSED\n");
}