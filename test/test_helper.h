#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include "matrix.h"

int testMatrixEqualityInt(matrix_t *A, matrix_t *B);
int testMatrixEqualityFloat(matrix_t *A, matrix_t *B, double eps);
int testMatrixEqualityDouble(matrix_t *A, matrix_t *B, double eps);
int testMatrixEquality(matrix_t *A, matrix_t *B, double eps);

void printMatrix(matrix_t *matrix);
void printIntMatrix(matrix_t *matrix);
void printFloatMatrix(matrix_t *matrix);
void printDoubleMatrix(matrix_t *matrix);

#endif