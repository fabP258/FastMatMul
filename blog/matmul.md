# How to multiply matrices (fast) in C

* Relevance of matrix multiplications: Machine Learning, Scientific computing (e.g. linear regression, rotations)
* Motivation:
  * Understand performance optimization of programs
  * Understand and exploit SIMD on CPU
  * Understand memory influence

## Matrix multiplication

Problem statement.
* Explain how matrices are multiplied mathematically.
* Explain computational complexity of matmul --> O(2n^3)

## The matrix datastructure

A matrix is a collection of elements on a two-dimensional grid having a shared data type. Representing a matrix in C while fullfilling requirements for enabling advanced use-cases is not as straightforward as one might think. Therefore, we will build it from scratch in an iterative manner in this section. We will begin with the possibly simplest approach and then iteratively reduce the list of drawbacks in order to converge to the final matrix datastructure.

### The naive approach
For now the discussion will be restricted to matrices with a specific data type of each element, e.g. single-precision floating point numbers (float). This assumption will be relaxed after building the foundations for representing this subset of general matrices.

Since a matrix is nothing else than a collection of numbers on a 2D grid the straightfoward way for representing it in C is a 2D array.

```C
float matrix[5][8];
```
This is easily implemented and also comes with strightforward element access via the bracket operator. Additionally there is no need for dynamic memory management. But this is also a major drawback. Since the size of the stack is constraint we might run out of memory when allocating a very large matrix. Furthermore, the dimensions of the matrix need to be known at compile time which might not be possible in some use-cases.

### Dynamic memory allocation
In order to avoid these drawbacks we consider a more advanced definition of a matrix. Therefore a struct is defined which will contain important meta data and most importantly a pointer to the first element of the matrix.

```C
typedef struct {
    float *data;
    size_t numRows;
    size_t numCols;
} matrix_t;
```

Let us now break down each field of the struct. The `data` field is a float pointer to the first element of a single contiguous array. In order to be able to represent a matrix by a single array we need to perform flattening. This can be done either in row-major or column-major order. Here we will only consider row-major order. Flattening a matrix in row-major order can be simply achived by concatenation of the rows like illustrated in the figure below.
![matrix flatten](matrix_flatten.png)

The remaining fields `numRows` and `numCols` are required for bounds checks and also index calculation. In order to access an element `(i,j)` the index in the `data` array can be computed by
$$k=i * numCols + j$$

Now we need to address the second drawback of the naive matrix representation, which is allocating the memory on the stack. This can be avoided by the following matrix creation function.

```C
matrix_t createMatrix(size_t numRows, size_t numCols) {
    matrix_t matrix;
    matrix.data = calloc(numRows * numCols, sizeof(float));
    matrix.numRows = numRows;
    matrix.numCols = numCols;
    return matrix;
}
```
By using the system call `calloc` the function requests the allocation of the number of bytes required for representing a matrix with $numRows * numCols$ elements of type float. Using `calloc` instead of `malloc` ensures that all bytes are initialized with zeros. Since this allocates memory on the heap we also need to free the memory manually if the object is not needed anymore. This can be done by calling the following function.

```C
void freeMatrix(matrix_t *matrix) {
  if (matrix->data == NULL) {
    return;
  }
  free(matrix->data);
  matrix->data = NULL;
  matrix->numRows = 0;
  matrix->numCols = 0;
}
```
### Adressing different data types
Now let us address the third drawback of the naive approach, which is that we are currently only able to represent float matrices. We start by defining an enum representing the data type.

```C
typedef enum {
  DTYPE_INT,
  DTYPE_FLOAT,
  DTYPE_DOUBLE
} MatrixDType;
```

Now we also modify the matrix struct definition by changing the float pointer to a generic void pointer and extending the struct by a field specifying the data type.

```C
typedef struct {
  void *data;
  size_t numRows;
  size_t numCols;
  MatrixDType dtype;
} matrix_t;
```

Respectively, the creation of this struct also needs to be modified.
```C
matrix_t createMatrix(size_t numRows, size_t numCols, MatrixDType dtype) {
  matrix_t matrix;
  size_t elementSize;
  switch (dtype) {
    case DTYPE_INT:
      elementSize = sizeof(int);
      break;
    case DTYPE_FLOAT:
      elementSize = sizeof(float);
      break;
    case DTYPE_DOUBLE:
      elementSize = sizeof(double);
      break;
    default:
      matrix.data = NULL;
      // TODO: Assert here?
      return matrix;
  }
  matrix->data = calloc(numRows * numCols, elementSize);
  if (matrix->data == NULL) {
    return matrix;
  }
  matrix->numRows = numRows;
  matrix->numCols = numCols;
  matrix->dtype = dtype;
  return matrix;
}
```

Recall that the `sizeof()` function used here returns the number of bytes that are required to store a specific type. Finally we are able to represent matrices of different data types. But this also comes at a cost. Accessing an element of a matrix requires now to cast the void pointer first to a pointer of the respective data type and calculating the index int the flattened array. This can be seen in the example below for a matrix object having data type float.

```C
// create a matrix
matrix_t matrix = createMatrix(2U, 3U, DTYPE_FLOAT);

// set element (1,2) of the matrix
((float *)matrix.data)[1 * matrix.numCols + 2] = 0.1f;
```
Since accessing elements of the matrix is a quite common use case and the operations required are prone to error it makes sense to abstract it into a function.
```C
// TODO: Setter and getter
```
We will revisit this issue once we implement the matrix multiplication.

### Enabling zero cost shape operations
The last extension of the data structure is the introduction of *column* and *row strides*. These encode by which value an element pointer needs to be increased in order to move to the next element along one of the two matrix dimensions. Using strides for element access enables shape operations to have basically zero cost. For example a matrix transpose can be achived by modifying the strides and the shape of the matrix while keeping the data buffer unmodified. The final datastructure is then given by
```
typedef struct matrix {
    void *data;
    size_t numRows;
    size_t numCols;
    size_t strides[2];
    MatrixDType dtype;
    size_t refcount;
    struct matrix *base;
    int ownsData;
} matrix_t;
```

### The matrix view
As you might noticed there is one more thing. The final version of the matrix datastructure includes several new fields: the `refcount`, a pointer to a base matrix object (`base`) and a flag indicating data ownership (`ownsData`). But why do we need it?
These are necessary to introduce the concept of a *view*. A view is a standard matrix object with the distinction that it does not own the data buffer but inherited it from some base matrix. Since shape operations do not modify the data itself but only the meta-data we do not want to allocate new memory for the results of these operations. We want that the result of shape operations *share* a data buffer with the source operand. Therefore, a view can be returned instead.

Freeing the allocated memory gets now more involved since we need to keep track of which matrix objects still need to access the data buffer. This can be seen best in the `freeMatrix` function
```
void freeMatrix(matrix_t *matrix) {
    matrix->refcount--;
    if (matrix->refcount == 0) {
        if (matrix->ownsData && matrix->data != NULL) {
            free(matrix->data);
            matrix->data = NULL;
            matrix->numRows = 0u;
            matrix->numCols = 0u;
            matrix->strides[0] = 0u;
            matrix->strides[1] = 0u;
        }
        if (matrix->base) {
            freeMatrix(matrix->base);
        }
        free(matrix);
    }
}
```
As can be seen the memory is now only freed if there are no references to it by external views indicated by the `refcount` and if the object owns the data buffer. 

### Alternative representation of the data buffer
Instead of having the data buffer be represented as a single contiguous array it would also have been possible to use an array of pointers for the matrix elements. This would avoid index calculations when accessing elements but at the cost of a potential heap fragmentation and making movement operations like the matrix transpose more involved. Furthermore having a single contiguous array enables scaling the matrix datastructure beyond two dimensions to a multi-dimensional array (a *tensor*) straight forward.