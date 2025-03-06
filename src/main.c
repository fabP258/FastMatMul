#include <stdio.h>
#include <time.h>
#include "matrix.h"
#include "linalg.h"

int main() {

    // Matrix sizes
    size_t M = 300U;
    size_t N = 250U;
    size_t P = 200U;

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

    // Perform matmul
    clock_t start = clock();
    matrix_t C = naiveMatmul(&A, &B);
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Result matrix has shape: [%d,%d]\n", C.num_rows, C.num_cols);

    // TODO: check if matmul is correct

    unsigned long num_ops = 2 * M * N * P;
    double flops = (double)num_ops / time_taken;
    double gflops = flops / 1e9;
    printf("Number of operations (adds & multiplies): %d\n", num_ops);
    printf("Performing matmul took: %.12f s\n", time_taken);
    printf("FLOP/s: %.2f\n", flops);
    printf("GFLOP/s: %.2f\n", gflops);

    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
    return 0;
}