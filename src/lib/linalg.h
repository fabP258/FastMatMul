#pragma once

#include "matrix.h"

typedef enum {
    NAIVE,
    OPTIMIZED_LOOP_ORDER,
} EMatmulAlgorithm;

matrix_t matmul(matrix_t *A, matrix_t *B, EMatmulAlgorithm algorithm);
void matmulNaive(matrix_t *A, matrix_t *B, matrix_t *result);
void matmulLoopOrderOptimized(matrix_t *A, matrix_t *B, matrix_t *result);