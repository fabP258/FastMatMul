#pragma once

#include "matrix.h"

matrix_t matmul(matrix_t *A, matrix_t *B);
matrix_t matmulNaive(matrix_t *A, matrix_t *B);
matrix_t matmulLoopOrderOptimized(matrix_t *A, matrix_t *B);