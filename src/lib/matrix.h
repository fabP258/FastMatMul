#pragma once

#include <stdlib.h>

#define DATA_TYPE float

typedef struct {
    DATA_TYPE **data;
    size_t num_rows;
    size_t num_cols;
} matrix_t;

DATA_TYPE *createArray(size_t size, DATA_TYPE value);
DATA_TYPE *freeArray(DATA_TYPE *array);
matrix_t createMatrix(size_t num_rows, size_t num_cols, DATA_TYPE value);
void initMatrix(matrix_t *matrix, size_t num_rows, size_t num_cols, DATA_TYPE value);
void freeMatrix(matrix_t *matrix);