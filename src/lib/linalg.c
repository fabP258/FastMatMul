#include "linalg.h"

matrix_t matmul(matrix_t *A, matrix_t *B, EMatmulAlgorithm algorithm) {
    matrix_t result;
    if (A->data == NULL || B->data == NULL || (A->num_cols != B->num_rows)) {
        result.data = NULL;
        return result;
    }
    initMatrix(&result, A->num_rows, B->num_cols, (DATA_TYPE)0);
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
    for (size_t i = 0; i < A->num_rows; i++) {
        DATA_TYPE *row = A->data[i];
        for (size_t j = 0; j < B->num_cols; j++) {
            for (size_t k = 0; k < A->num_cols; k++) {
                result->data[i][j] += row[k] * B->data[k][j];
            }
        }
    }
}

void matmulLoopOrderOptimized(matrix_t *A, matrix_t *B, matrix_t *result) {
    for (size_t i = 0; i < A->num_rows; i++) {
        DATA_TYPE *row = A->data[i];
        for (size_t k = 0; k < A->num_cols; k++) {
            for (size_t j = 0; j < B->num_cols; j++) {
                result->data[i][j] += row[k] * B->data[k][j];
            }
        }
    }
}