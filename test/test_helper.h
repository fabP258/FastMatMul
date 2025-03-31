#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include "matrix.h"

int testMatrixEqualityInt(matrix_t *A, matrix_t *B);
int testMatrixEqualityFloat(matrix_t *A, matrix_t *B, double eps);
int testMatrixEqualityDouble(matrix_t *A, matrix_t *B, double eps);
int testMatrixEquality(matrix_t *A, matrix_t *B, double eps);

#endif