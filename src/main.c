#include <stdio.h>
#include "matrix.h"
#include "linalg.h"

int main() {

    matrix_t A = createMatrix((size_t)3U, (size_t)5U, (DATA_TYPE)1);
    if (A.data == NULL) {
        printf("Failed to create matrix.");
        return -1;
    }
    matrix_t B = createMatrix((size_t)5U, (size_t)6U, (DATA_TYPE)2);
    if (B.data == NULL) {
        printf("Failed to create matrix.");
        return -1;
    }

    matrix_t C = matmul(&A, &B);

    printf("Result matrix has shape: [%d,%d]", C.num_rows, C.num_cols);

    // TODO: check if matmul is correct
    // TODO: calc flops
    // TODO: print matrix

    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
    return 0;
}