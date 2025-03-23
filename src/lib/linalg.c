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

MatmulNaiveFunc matmulNaiveFuncTable[] = {
    matmulNaiveInt, // DTYPE_INT
    matmulNaiveFloat, // DTYPE_FLOAT
    matmulNaiveDouble // DTYPE_DOUBLE
};

void matmulNaive(matrix_t *A, matrix_t *B, matrix_t *result) {
    MatmulNaiveFunc func =  matmulNaiveFuncTable[A->dtype];
    func(A, B, result);
}

void matmulNaiveInt(matrix_t *A, matrix_t *B, matrix_t *result) {
    int *aData = (int *)A->data;
    int *bData = (int *)B->data;
    int *resData = (int *)result->data;
    int temp = 0;
    for (size_t i = 0; i < A->numRows; i++) {
        for (size_t j = 0; j < B->numCols; j++) {
            temp = 0;
            for (size_t k = 0; k < A->numCols; k++) {
                temp += aData[i * A->numCols + k] * bData[k * B->numCols + j];
            }
            resData[i * result->numCols + j] = temp;
        }
    }
}

void matmulNaiveFloat(matrix_t *A, matrix_t *B, matrix_t *result) {
    float *aData = (float *)A->data;
    float *bData = (float *)B->data;
    float *resData = (float *)result->data;
    float temp = 0;
    for (size_t i = 0; i < A->numRows; i++) {
        for (size_t j = 0; j < B->numCols; j++) {
            temp = 0;
            for (size_t k = 0; k < A->numCols; k++) {
                temp += aData[i * A->numCols + k] * bData[k * B->numCols + j];
            }
            resData[i * result->numCols + j] = temp;
        }
    }
}

void matmulNaiveDouble(matrix_t *A, matrix_t *B, matrix_t *result) {
    double *aData = (double *)A->data;
    double *bData = (double *)B->data;
    double *resData = (double *)result->data;
    double temp = 0;
    for (size_t i = 0; i < A->numRows; i++) {
        for (size_t j = 0; j < B->numCols; j++) {
            temp = 0;
            for (size_t k = 0; k < A->numCols; k++) {
                temp += aData[i * A->numCols + k] * bData[k * B->numCols + j];
            }
            resData[i * result->numCols + j] = temp;
        }
    }
}

MatmulLoopOrderOptimizedFunc matmulLoopOrderOptimizedFuncTable[] = {
    matmulLoopOrderOptimizedInt, // DTYPE_INT
    matmulLoopOrderOptimizedFloat, // DTYPE_FLOAT
    matmulLoopOrderOptimizedDouble // DTYPE_DOUBLE
};

void matmulLoopOrderOptimized(matrix_t *A, matrix_t *B, matrix_t *result) {
    MatmulLoopOrderOptimizedFunc func = matmulLoopOrderOptimizedFuncTable[A->dtype];
    func(A, B, result);
}

void matmulLoopOrderOptimizedInt(matrix_t *A, matrix_t *B, matrix_t *result) {
    int *aData = (int *)A->data;
    int *bData = (int *)B->data;
    int *resData = (int *)result->data;
    for (size_t i = 0; i < A->numRows; i++) {
        for (size_t k = 0; k < A->numCols; k++) {
            for (size_t j = 0; j < B->numCols; j++) {
                resData[i * result->numCols+j] += aData[i * A->numCols + k] * bData[k * B->numCols + j];
            }
        }
    }
}

void matmulLoopOrderOptimizedFloat(matrix_t *A, matrix_t *B, matrix_t *result) {
    float *aData = (float *)A->data;
    float *bData = (float *)B->data;
    float *resData = (float *)result->data;
    for (size_t i = 0; i < A->numRows; i++) {
        for (size_t k = 0; k < A->numCols; k++) {
            for (size_t j = 0; j < B->numCols; j++) {
                resData[i * result->numCols+j] += aData[i * A->numCols + k] * bData[k * B->numCols + j];
            }
        }
    }
}

void matmulLoopOrderOptimizedDouble(matrix_t *A, matrix_t *B, matrix_t *result) {
    double *aData = (double *)A->data;
    double *bData = (double *)B->data;
    double *resData = (double *)result->data;
    for (size_t i = 0; i < A->numRows; i++) {
        for (size_t k = 0; k < A->numCols; k++) {
            for (size_t j = 0; j < B->numCols; j++) {
                resData[i * result->numCols+j] += aData[i * A->numCols + k] * bData[k * B->numCols + j];
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