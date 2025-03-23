#pragma once

#include "matrix.h"

typedef enum {
    NAIVE,
    OPTIMIZED_LOOP_ORDER,
#ifdef USE_CBLAS
    BLAS_GEMM
#endif
} EMatmulAlgorithm;

matrix_t matmul(matrix_t *A, matrix_t *B, EMatmulAlgorithm algorithm);

void matmulNaive(matrix_t *A, matrix_t *B, matrix_t *result);
void matmulNaiveInt(matrix_t *A, matrix_t *B, matrix_t *result);
void matmulNaiveFloat(matrix_t *A, matrix_t *B, matrix_t *result);
void matmulNaiveDouble(matrix_t *A, matrix_t *B, matrix_t *result);
typedef void (*MatmulNaiveFunc)(matrix_t *, matrix_t *, matrix_t*);
extern MatmulNaiveFunc matmulNaiveFuncTable[];

void matmulLoopOrderOptimized(matrix_t *A, matrix_t *B, matrix_t *result);
void matmulLoopOrderOptimizedInt(matrix_t *A, matrix_t *B, matrix_t *result);
void matmulLoopOrderOptimizedFloat(matrix_t *A, matrix_t *B, matrix_t *result);
void matmulLoopOrderOptimizedDouble(matrix_t *A, matrix_t *B, matrix_t *result);
typedef void (*MatmulLoopOrderOptimizedFunc)(matrix_t *, matrix_t *, matrix_t*);
extern MatmulLoopOrderOptimizedFunc matmulLoopOrderOptimizedFuncTable[];

#ifdef USE_CBLAS
// TODO: dispatch dtype properly
void matmulBlas(matrix_t *A, matrix_t *B, matrix_t *result);
#endif