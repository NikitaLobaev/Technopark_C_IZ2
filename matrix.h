#ifndef TECHNOPARK_C_IZ2_MATRIX_H
#define TECHNOPARK_C_IZ2_MATRIX_H

#define IZ2_OK 0
#define IZ2_ERROR_MEMORY 1u
#define IZ2_ERROR_ARGUMENT 2u
#define IZ2_ERROR_FORK_FAILED 4u

u_int8_t matrix_read(int*, size_t, size_t);

u_int8_t matrix_print(const int*, size_t, size_t);

u_int8_t matrix_mirroring(const int*, size_t, size_t, int*);

u_int8_t matrix_mirroring_multiproc(const int*, size_t, size_t, int*, size_t);

#endif //TECHNOPARK_C_IZ2_MATRIX_H
