#include "linalg.h"

#ifdef USE_CBLAS
#include <cblas.h>
#endif

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
#ifdef USE_CBLAS
        case BLAS_GEMM:
            matmulBlas(A,B,&result);
            break;
#endif
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

#ifdef USE_CBLAS
void matmulBlas(matrix_t *A, matrix_t *B, matrix_t *result) {
    // TODO: Handle dtypes properly, this only works for float
    openblas_set_num_threads(4);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A->numRows, B->numCols, A->numCols, 1.0f, A->data, A->numCols, B->data, B->numCols, 0.0f, result->data, result->numCols);
}
#endif