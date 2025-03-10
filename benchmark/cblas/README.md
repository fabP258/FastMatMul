# Fast Matrix Multiplication using (C)BLAS

## Getting started

### Dependencies

The implementation depends on the BLAS package wrapped in C. Therefore a installation of BLAS is required.

### Compile

```shell
$ gcc -o matmul_blas matmul_blas.c -I <lopenblas-path>/include -L <lopenblas-path>/lib -lopenblas
```