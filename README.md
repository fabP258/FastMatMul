# FastMatMul
Exploring how to multiply matrices fast.

## Performance overview

|            Algorithm          | GLOP/s |   %  |
|-------------------------------|--------|------|
| C / GCC: Naive                |  0.33  | 0.13 |
| C / GCC: Loop order optimized |  9.28  | 3.7  |
| Numpy                         |  250   | 100  |
| CBLAS (sGEMM)                 |  320   | 100  |

## Compilation

This project uses GNU Make to automate the compilation process. In order to use the full functionality it is required to have *OpenBLAS* installed in `.\lib`. If the library is not available only the parts of the program that do not depend on it will be compiled.


## Run the program

```shell
$ make run
```

## Run the tests

```shell
$ make test
```
