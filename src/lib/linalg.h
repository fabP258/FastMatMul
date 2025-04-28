#pragma once

#include "matrix.h"

typedef enum {
    NAIVE,
    OPTIMIZED_LOOP_ORDER,
#ifdef CBLAS_AVAILABLE
    BLAS_GEMM
#endif
} EMatmulAlgorithm;

// Matrix multiplication
matrix_t *matmul(matrix_t *A, matrix_t *B, EMatmulAlgorithm algorithm);

typedef void (*MatmulFunc)(matrix_t *, matrix_t *, matrix_t *);

void matmulNaive(matrix_t *A, matrix_t *B, matrix_t *result);
void matmulNaiveInt(matrix_t *A, matrix_t *B, matrix_t *result);
void matmulNaiveFloat(matrix_t *A, matrix_t *B, matrix_t *result);
void matmulNaiveDouble(matrix_t *A, matrix_t *B, matrix_t *result);
extern MatmulFunc matmulNaiveFuncTable[];

void matmulLoopOrderOptimized(matrix_t *A, matrix_t *B, matrix_t *result);
void matmulLoopOrderOptimizedInt(matrix_t *A, matrix_t *B, matrix_t *result);
void matmulLoopOrderOptimizedFloat(matrix_t *A, matrix_t *B, matrix_t *result);
void matmulLoopOrderOptimizedDouble(matrix_t *A, matrix_t *B, matrix_t *result);
extern MatmulFunc matmulLoopOrderOptimizedFuncTable[];

#ifdef CBLAS_AVAILABLE
void matmulBlas(matrix_t *A, matrix_t *B, matrix_t *result);
#endif

matrix_t *transpose(matrix_t *matrix);