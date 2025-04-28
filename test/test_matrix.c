#include <assert.h>
#include <stdio.h>
#include "matrix.h"
#include "test_matrix.h"

void testMatrix() {
    testCreateMatrix();
    testFreeMatrix();
    testFreeMatrixOnView();
}

void testCreateMatrix() {
    size_t numRows = 2u, numCols = 3u;
    MatrixDType dtype = DTYPE_INT;
    matrix_t *matrix = createMatrix(numRows, numCols, dtype);
    assert(matrix->numRows == numRows);
    assert(matrix->numCols == numCols);
    assert(matrix->strides[0] == numCols);
    assert(matrix->strides[1] == 1u);
    assert(matrix->dtype == dtype);
    assert(matrix->base == NULL);
    assert(matrix->refcount == 1u);
    assert(matrix->ownsData);
    assert(matrix->data != NULL);
    printf("[TEST: testCreateMatrix] PASSED\n");
    freeMatrix(matrix);
}

void testFreeMatrix() {
    size_t numRows = 2u, numCols = 3u;
    MatrixDType dtype = DTYPE_INT;
    matrix_t *matrix = createMatrix(numRows, numCols, dtype);
    freeMatrix(matrix);
    matrix = NULL;
    printf("[TEST: testFreeMatrix] PASSED\n");
}

void testFreeMatrixOnView() {
    size_t numRows = 2u, numCols = 3u;
    MatrixDType dtype = DTYPE_INT;
    matrix_t *matrix = createMatrix(numRows, numCols, dtype);
    matrix_t *matrixView1 = createView(matrix);
    matrix_t *matrixView2 = createView(matrix);
    assert(matrix->refcount==3u);
    freeMatrix(matrixView1);
    assert(matrix->refcount==2u);
    freeMatrix(matrixView2);
    assert(matrix->refcount==1u);
    freeMatrix(matrix);
    printf("[TEST: testFreeMatrixOnView] PASSED\n");
}