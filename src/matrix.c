#include "matrix.h"

DATA_TYPE *createArray(size_t size, DATA_TYPE value) {
    DATA_TYPE *array = (DATA_TYPE *)malloc(size * sizeof(DATA_TYPE));
    if (array == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < size; i++) {
        array[i] = value;
    }
    return array;
}

DATA_TYPE *freeArray(DATA_TYPE *array) {
    if (array != NULL) {
        free(array);
    }
    return NULL;
}

matrix_t createMatrix(size_t num_rows, size_t num_cols, DATA_TYPE value) {
    matrix_t matrix;
    initMatrix(&matrix, num_rows, num_cols, value);
    return matrix;
}

void initMatrix(matrix_t *matrix, size_t num_rows, size_t num_cols, DATA_TYPE value) {
    matrix->data = (DATA_TYPE **)malloc(num_rows * sizeof(DATA_TYPE *));
    if (matrix->data == NULL) {
        return;
    }
    for (size_t i = 0; i < num_rows; i++) {
        matrix->data[i] = createArray(num_cols, value);
        if (matrix->data[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                matrix->data[i] = freeArray(matrix->data[i]);
            }
            free(matrix->data);
            matrix->data = NULL;
            return;
        }
    }
    matrix->num_rows = num_rows;
    matrix->num_cols = num_cols;
}

void freeMatrix(matrix_t *matrix) {
    if (matrix->data == NULL) {
        return;
    }
    for (size_t i = 0; i < matrix->num_rows; i++) {
        matrix->data[i] = freeArray(matrix->data[i]);
    }
    free(matrix->data);
    matrix->data = NULL;
}