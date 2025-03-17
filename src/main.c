#include <stdio.h>
#include <time.h>
#include "matrix.h"
#include "linalg.h"

#ifdef USE_CBLAS
#define NUM_MATMUL_ALGOS 3
#else
#define NUM_MATMUL_ALGOS 2
#endif

double evaluateMatmulAlgorithmFlops(matrix_t *A, matrix_t *B, EMatmulAlgorithm algorithm) {
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
    freeMatrix(&C);

    return flops;
}

int main() {
#ifdef USE_CBLAS
    EMatmulAlgorithm algorithms[NUM_MATMUL_ALGOS] = { NAIVE, OPTIMIZED_LOOP_ORDER, BLAS_GEMM };
#else 
    EMatmulAlgorithm algorithms[NUM_MATMUL_ALGOS] = { NAIVE, OPTIMIZED_LOOP_ORDER };
#endif
    size_t matrixSizes[4] = { 256U, 512U, 1024U, 2048U };

    printf("%-15s", "Matrix size N");
    for (size_t i = 0; i < NUM_MATMUL_ALGOS; i++) {
        printf("Algorithm %lu\t", i);
    }
    printf("\n");

    matrix_t A, B;
    for (size_t i = 0; i < 4; i++) {
        A = createMatrix(matrixSizes[i], matrixSizes[i], (DATA_TYPE)1);
        B = createMatrix(matrixSizes[i], matrixSizes[i], (DATA_TYPE)2);
        if ((A.data == NULL) || (B.data == NULL)) {
            printf("Failed to create matrix.");
            return -1;
        }
        printf("%-15lu", matrixSizes[i]);
        for (size_t i = 0; i < NUM_MATMUL_ALGOS; i++) {
            double flops = evaluateMatmulAlgorithmFlops(&A, &B, algorithms[i]);
            printf("%.2f \t", flops / 1e9);
        }
        printf("\n");
        freeMatrix(&A);
        freeMatrix(&B);
    }

    return 0;
}