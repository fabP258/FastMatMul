#include <stdio.h>
#include <time.h>
#include "matrix.h"
#include "linalg.h"

double evaluateMatmulAlgorithm(matrix_t *A, matrix_t *B, EMatmulAlgorithm algorithm) {
    matrix_t C;
    clock_t start, end;

    double numOps = 2.0 * (double)A->numRows * (double)B->numCols * (double)A->numCols;
    start = clock();
    C = matmul(A, B, algorithm);
    end = clock();
    if (C.data == NULL) {
        printf("Algorithm %d: Matmul failed.", algorithm);
    }
    double timeTaken = ((double)(end - start)) / CLOCKS_PER_SEC;
    double flops = numOps / timeTaken;
    double gFlops = flops / 1e9;
    printf("Algorithm %d, GLOP/s: %.2f\n", algorithm, gFlops);
    freeMatrix(&C);
}

int main() {

    // Matrix sizes
    size_t M = 1000U;
    size_t N = 1000U;
    size_t P = 1000U;

    matrix_t A = createMatrix(M, N, (DATA_TYPE)1);
    if (A.data == NULL) {
        printf("Failed to create matrix.");
        return -1;
    }
    matrix_t B = createMatrix(N, P, (DATA_TYPE)2);
    if (B.data == NULL) {
        printf("Failed to create matrix.");
        return -1;
    }

    EMatmulAlgorithm algorithms[2] = { NAIVE, OPTIMIZED_LOOP_ORDER };
    for (size_t i = 0; i < 2; i++) {
        evaluateMatmulAlgorithm(&A, &B, algorithms[i]);
    }

    freeMatrix(&A);
    freeMatrix(&B);
    return 0;
}