#include <stdio.h>
#include <time.h>
#include "matrix.h"
#include "linalg.h"

#ifdef USE_CBLAS
#define NUM_MATMUL_ALGOS 3
#else
#define NUM_MATMUL_ALGOS 2
#endif
#define NUM_DTYPES 3

double evaluateMatmulAlgorithmFlops(matrix_t *A, matrix_t *B, EMatmulAlgorithm algorithm) {
    matrix_t C;
    struct timespec start, end;

    double numOps = 2.0 * (double)A->numRows * (double)B->numCols * (double)A->numCols;
    clock_gettime(CLOCK_MONOTONIC, &start);
    C = matmul(A, B, algorithm);
    clock_gettime(CLOCK_MONOTONIC, &end);
    if (C.data == NULL) {
        printf("Algorithm %d: Matmul failed.", algorithm);
    }
    double timeTaken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    double flops = numOps / timeTaken;
    freeMatrix(&C);

    return flops;
}

int main() {
#ifdef USE_CBLAS
    EMatmulAlgorithm algorithms[NUM_MATMUL_ALGOS] = { NAIVE, OPTIMIZED_LOOP_ORDER, BLAS_GEMM };
    const char *algo_header[NUM_MATMUL_ALGOS] = {"Naive", "Optimized_Loop_Order", "BLAS_GEMM" };
#else 
    EMatmulAlgorithm algorithms[NUM_MATMUL_ALGOS] = { NAIVE, OPTIMIZED_LOOP_ORDER };
    const char *algo_header[NUM_MATMUL_ALGOS] = {"Naive", "Optimized_Loop_Order" };
#endif
    const char *dtype_header[NUM_DTYPES] = {"int", "fp32", "fp64"};
    size_t matrixSizes[8] = { 100U, 200U, 300U, 400U, 500U, 750U, 1000U, 1500U };

    FILE *file = fopen("results.csv", "w");
    if (file == NULL) {
        printf("Failed to open csv file.");
        return -1;
    }
    fprintf(file, "matrix_size");
    printf("%-15s", "Matrix size N");
    for (size_t j = 0; j < NUM_DTYPES; j++) {
        for (size_t i = 0; i < NUM_MATMUL_ALGOS; i++) {
            printf("%s_%s\t", algo_header[i], dtype_header[j]);
            fprintf(file, ",%s_%s", algo_header[i], dtype_header[j]);
        }
    }
    printf("\n");
    fprintf(file, "\n");

    matrix_t A, B;
    for (size_t i = 0; i < 8; i++) {
        printf("%-15lu", matrixSizes[i]);
        fprintf(file, "%lu", matrixSizes[i]);
        for (size_t j = 0; j < NUM_DTYPES; j++) {
            A = createMatrix(matrixSizes[i], matrixSizes[i], j);
            B = createMatrix(matrixSizes[i], matrixSizes[i], j);
            if ((A.data == NULL) || (B.data == NULL)) {
                printf("Failed to create matrix.");
                return -1;
            }
            for (size_t k = 0; k < NUM_MATMUL_ALGOS; k++) {
                double flops = evaluateMatmulAlgorithmFlops(&A, &B, algorithms[k]);
                printf("%.2f \t", flops / 1e9);
                fprintf(file, ",%.2f", flops);
            }
            freeMatrix(&A);
            freeMatrix(&B);
        }
        printf("\n");
        fprintf(file, "\n");
    }

    fclose(file);

    return 0;
}