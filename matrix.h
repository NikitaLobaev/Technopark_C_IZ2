#ifndef TECHNOPARK_C_IZ2_MATRIX_H
#define TECHNOPARK_C_IZ2_MATRIX_H

#define IZ2_OK 0
#define IZ2_ERROR_MEMORY 1
#define IZ2_ERROR_ARGUMENT 2

int matrix_read(int*, size_t, size_t);

int matrix_print(const int*, size_t, size_t);

int matrix_mirroring(const int*, size_t, size_t, int*);

#endif //TECHNOPARK_C_IZ2_MATRIX_H
