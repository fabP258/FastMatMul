#include "linalg.h"

matrix_t matmul(matrix_t *A, matrix_t *B) {
    matrix_t result;
    if (A->data == NULL || B->data == NULL || (A->num_cols != B->num_rows)) {
        result.data = NULL;
        return result;
    }
    initMatrix(&result, A->num_rows, B->num_cols, (DATA_TYPE)0);
    if (result.data == NULL) {
        return result;
    }
    for (size_t i = 0; i < A->num_rows; i++) {
        for (size_t j = 0; j < B->num_cols; j++) {
            for (size_t k = 0; k < A->num_cols; k++) {
                result.data[i][j] += A->data[i][k] * B->data[k][j];
            }
        }
    }
    return result;
}