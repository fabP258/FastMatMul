#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cblas.h>

double diff_ms(struct timespec start, struct timespec end)
{
        struct timespec temp;
        if ((end.tv_nsec - start.tv_nsec) < 0)
        {
                temp.tv_sec = end.tv_sec - start.tv_sec - 1;
                temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
        }
        else
        {
                temp.tv_sec = end.tv_sec - start.tv_sec;
                temp.tv_nsec = end.tv_nsec - start.tv_nsec;
        }
        double ms = (temp.tv_sec * 1E9 + temp.tv_nsec) * 0.000001;
        return ms;
}

float *createMatrix(size_t num_rows, size_t num_cols, float init_val) {
    float *matrix = (float *)malloc(num_rows * num_cols * sizeof(float));
    if (matrix == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < (num_rows * num_cols); i++) {
        matrix[i] = init_val;
    }

    return matrix;
}

float *freeMatrix(float *matrix) {
    if (matrix != NULL) {
        free(matrix);
    }
    return NULL;
}

void printMatrix(float *matrix, int num_rows, int num_cols) {
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            printf("%.2f ", matrix[i * num_cols + j]);
        }
        printf("\n");
    }
}

int main() {
    openblas_set_num_threads(4);

    // Matrix allocation
    unsigned int N = 5000;
    float *A = createMatrix(N, N, 1.0f);
    float *B = createMatrix(N, N, 0.5f);
    float *C = createMatrix(N, N, 0.0f);

    // GEMM
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0f, A, N, B, N, 0.0f, C, N);
    clock_gettime(CLOCK_REALTIME, &end);
    double time_taken = diff_ms(start, end) * 1e-3;

    // Calculate FLOP/s
    double num_ops = 2 * (double)N * (double)N * (double)N;
    double flops = num_ops / time_taken;
    double gflops = flops / 1e9;
    printf("Number of operations (adds & multiplies): %lu\n", num_ops);
    printf("Performing matmul took: %.12f s\n", time_taken);
    printf("GFLOP/s: %.2f\n", gflops);

    // Cleanup
    A = freeMatrix(A);
    B = freeMatrix(B);
    C = freeMatrix(C);

    return 0;
}