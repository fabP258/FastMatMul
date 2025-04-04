#include "linalg.h"

#ifdef CBLAS_AVAILABLE
#include <cblas.h>
#endif

matrix_t matmul(matrix_t *A, matrix_t *B, EMatmulAlgorithm algorithm) {
    matrix_t result;
    if (A->data == NULL || B->data == NULL || (A->numCols != B->numRows) || (A->dtype != B->dtype)) {
        result.data = NULL;
        // TODO: Report error properly to caller
        return result;
    }
    initMatrix(&result, A->numRows, B->numCols, A->dtype);
    if (result.data == NULL) {
        // TODO: Report error properly to caller
        return result;
    }
    switch (algorithm) {
        case NAIVE:
            matmulNaive(A,B,&result);
            break;
        case OPTIMIZED_LOOP_ORDER:
            matmulLoopOrderOptimized(A,B,&result);
            break;
#ifdef CBLAS_AVAILABLE
        case BLAS_GEMM:
            matmulBlas(A,B,&result);
            break;
#endif
        default:
            fprintf(stderr, "Unsupported algorithm.");
            exit(1);
    }
    return result;
}

MatmulFunc matmulNaiveFuncTable[] = {
    matmulNaiveInt,   // DTYPE_INT
    matmulNaiveFloat, // DTYPE_FLOAT
    matmulNaiveDouble // DTYPE_DOUBLE
};

void matmulNaive(matrix_t *A, matrix_t *B, matrix_t *result) {
    MatmulFunc func =  matmulNaiveFuncTable[A->dtype];
    func(A, B, result);
}

void matmulNaiveInt(matrix_t *A, matrix_t *B, matrix_t *result) {
    int *aData = (int *)A->data;
    int *bData = (int *)B->data;
    int *resData = (int *)result->data;
    size_t aStrides[2] = { A->strides[0], A->strides[1] };
    size_t bStrides[2] = { B->strides[0], B->strides[1] };
    size_t resStrides[2] = { result->strides[0], result->strides[1] };
    for (size_t i = 0; i < A->numRows; i++) {
        size_t resIdx = i * resStrides[0];
        for (size_t j = 0; j < B->numCols; j++) {
            int temp = 0;
            size_t aIdx = i * aStrides[0];
            size_t bIdx = j * bStrides[1];
            for (size_t k = 0; k < A->numCols; k++) {
                temp += aData[aIdx] * bData[bIdx];
                aIdx += aStrides[1];
                bIdx += bStrides[0];
            }
            resData[resIdx] = temp;
            resIdx += resStrides[1];
        }
    }
}

void matmulNaiveFloat(matrix_t *A, matrix_t *B, matrix_t *result) {
    float *aData = (float *)A->data;
    float *bData = (float *)B->data;
    float *resData = (float *)result->data;
    for (size_t i = 0; i < A->numRows; i++) {
        size_t resIdx = i * result->strides[0];
        for (size_t j = 0; j < B->numCols; j++) {
            size_t aIdx = i * A->strides[0];
            size_t bIdx = j * B->strides[1];
            float temp = 0;
            for (size_t k = 0; k < A->numCols; k++) {
                temp += aData[aIdx] * bData[bIdx];
                aIdx += A->strides[1];
                bIdx += B->strides[0];
            }
            resData[resIdx] = temp;
            resIdx += result->strides[1];
        }
    }
}

void matmulNaiveDouble(matrix_t *A, matrix_t *B, matrix_t *result) {
    double *aData = (double *)A->data;
    double *bData = (double *)B->data;
    double *resData = (double *)result->data;
    for (size_t i = 0; i < A->numRows; i++) {
        size_t resIdx = i * result->strides[0];
        for (size_t j = 0; j < B->numCols; j++) {
            size_t aIdx = i * A->strides[0];
            size_t bIdx = j * B->strides[1];
            double temp = 0;
            for (size_t k = 0; k < A->numCols; k++) {
                temp += aData[aIdx] * bData[bIdx];
                aIdx += A->strides[1];
                bIdx += B->strides[0];
            }
            resData[resIdx] = temp;
            resIdx += result->strides[1];
        }
    }
}

MatmulFunc matmulLoopOrderOptimizedFuncTable[] = {
    matmulLoopOrderOptimizedInt,   // DTYPE_INT
    matmulLoopOrderOptimizedFloat, // DTYPE_FLOAT
    matmulLoopOrderOptimizedDouble // DTYPE_DOUBLE
};

void matmulLoopOrderOptimized(matrix_t *A, matrix_t *B, matrix_t *result) {
    MatmulFunc func = matmulLoopOrderOptimizedFuncTable[A->dtype];
    func(A, B, result);
}

void matmulLoopOrderOptimizedInt(matrix_t *A, matrix_t *B, matrix_t *result) {
    int *aData = (int *)A->data;
    int *bData = (int *)B->data;
    int *resData = (int *)result->data;
    for (size_t i = 0; i < A->numRows; i++) {
        size_t aIdx = i * A->strides[0];
        for (size_t k = 0; k < A->numCols; k++) {
            size_t resIdx = i * result->strides[0];
            size_t bIdx = k * B->strides[0];
            for (size_t j = 0; j < B->numCols; j++) {
                resData[resIdx] += aData[aIdx] * bData[bIdx];
                resIdx += result->strides[1];
                bIdx += B->strides[1];
            }
            aIdx += A->strides[1];
        }
    }
}

void matmulLoopOrderOptimizedFloat(matrix_t *A, matrix_t *B, matrix_t *result) {
    float *aData = (float *)A->data;
    float *bData = (float *)B->data;
    float *resData = (float *)result->data;
    for (size_t i = 0; i < A->numRows; i++) {
        size_t aIdx = i * A->strides[0];
        for (size_t k = 0; k < A->numCols; k++) {
            size_t resIdx = i * result->strides[0];
            size_t bIdx = k * B->strides[0];
            for (size_t j = 0; j < B->numCols; j++) {
                resData[resIdx] += aData[aIdx] * bData[bIdx];
                resIdx += result->strides[1];
                bIdx += B->strides[1];
            }
            aIdx += A->strides[1];
        }
    }
}

void matmulLoopOrderOptimizedDouble(matrix_t *A, matrix_t *B, matrix_t *result) {
    double *aData = (double *)A->data;
    double *bData = (double *)B->data;
    double *resData = (double *)result->data;
    for (size_t i = 0; i < A->numRows; i++) {
        size_t aIdx = i * A->strides[0];
        for (size_t k = 0; k < A->numCols; k++) {
            size_t resIdx = i * result->strides[0];
            size_t bIdx = k * B->strides[0];
            for (size_t j = 0; j < B->numCols; j++) {
                resData[resIdx] += aData[aIdx] * bData[bIdx];
                resIdx += result->strides[1];
                bIdx += B->strides[1];
            }
            aIdx += A->strides[1];
        }
    }
}

#ifdef CBLAS_AVAILABLE
void matmulBlas(matrix_t *A, matrix_t *B, matrix_t *result) {
    openblas_set_num_threads(4);
    switch (A->dtype) {
        case DTYPE_INT:
            matmulLoopOrderOptimized(A, B, result);
            break;
        case DTYPE_FLOAT:
            cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A->numRows, B->numCols, A->numCols, 1.0f, (float *)A->data, A->numCols, (float *)B->data, B->numCols, 0.0f, (float *)result->data, result->numCols);
            break;
        case DTYPE_DOUBLE:
            cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A->numRows, B->numCols, A->numCols, 1.0f, (double *)A->data, A->numCols, (double *)B->data, B->numCols, 0.0f, (double *)result->data, result->numCols);
            break;
        default:
            fprintf(stderr, "Unsupported dtype.");
            exit(1);
    }
}
#endif