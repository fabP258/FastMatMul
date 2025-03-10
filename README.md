# FastMatMul
Exploring how to multiply matrices fast.

## Performance overview

|            Algorithm          | GLOP/s |   %  |
|-------------------------------|--------|------|
| C / GCC: Naive                |  0.33  | 0.13 |
| C / GCC: Loop order optimized |  9.28  | 3.7  |
| Numpy                         |  250   | 100  |
| CBLAS (sGEMM)                 |  320   | 100  |

## Run the program

```shell
$ make run
```

## Run the tests

```shell
$ make test
```
