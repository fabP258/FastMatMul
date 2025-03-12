#include "linalg.h"

matrix_t matmul(matrix_t *A, matrix_t *B, EMatmulAlgorithm algorithm) {
    matrix_t result;
    if (A->data == NULL || B->data == NULL || (A->numCols != B->numRows)) {
        result.data = NULL;
        return result;
    }
    initMatrix(&result, A->numRows, B->numCols, (DATA_TYPE)0);
    if (result.data == NULL) {
        return result;
    }
    switch (algorithm) {
        case NAIVE:
            matmulNaive(A,B,&result);
            break;
        case OPTIMIZED_LOOP_ORDER:
            matmulLoopOrderOptimized(A,B,&result);
            break;
        default:
            // TODO: catch this error
            freeMatrix(&result);
            break;
    }
    return result;
}

void matmulNaive(matrix_t *A, matrix_t *B, matrix_t *result) {
    DATA_TYPE temp = (DATA_TYPE)0;
    for (size_t i = 0; i < A->numRows; i++) {
        for (size_t j = 0; j < B->numCols; j++) {
            temp = (DATA_TYPE)0;
            for (size_t k = 0; k < A->numCols; k++) {
                temp += A->data[i * A->numCols + k] * B->data[k * B->numCols + j];
            }
            result->data[i * result->numCols + j] = temp;
        }
    }
}

void matmulLoopOrderOptimized(matrix_t *A, matrix_t *B, matrix_t *result) {
    for (size_t i = 0; i < A->numRows; i++) {
        for (size_t k = 0; k < A->numCols; k++) {
            for (size_t j = 0; j < B->numCols; j++) {
                result->data[i * result->numCols+j] += A->data[i * A->numCols + k] * B->data[k * B->numCols + j];
            }
        }
    }
}