#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main(int argc, char **argv) {
	if (argc != 3 && argc != 4) {
		printf("Usage: matrix <rows_count> <columns_count> (<proc_count>)");
		return IZ2_ERROR_ARGUMENT;
	}
	u_int8_t error = IZ2_OK;
	const size_t rows_count = (size_t) atoi(argv[1]), columns_count = (size_t) atoi(argv[2]);
	const size_t proc_count = (argc == 4 ? (size_t) atoi(argv[3]) : 1);
	int *matrix_source = (int*) malloc(rows_count * columns_count * sizeof(int));
	int *matrix_destination = (int*) malloc(columns_count * rows_count * sizeof(int));
	if (!matrix_source || !matrix_destination) {
		error |= IZ2_ERROR_MEMORY;
	}
	error |= matrix_read(matrix_source, rows_count, columns_count);
	if (proc_count == 1) {
		//malloc
		error |= matrix_mirroring(matrix_source, rows_count, columns_count, matrix_destination);
	} else {
		//mmap
		error |= matrix_mirroring_multiproc(matrix_source, rows_count, columns_count, matrix_destination, proc_count);
	}
	error |= matrix_print(matrix_destination, rows_count, columns_count);
	free(matrix_source);
	free(matrix_destination);
	if ((error & 1u) == 1u) {
		printf("Ошибка. Не удалось выделить память для работы алгоритма.\n");
	}
	if ((error & 2u) == 2u) {
		printf("Ошибка. Некорректные аргументы функций.\n");
	}
	if ((error & 4u) == 4u) {
		printf("Ошибка. Не удалось создать процесс для работы параллельного алгоритма.\n");
	}
	return error;
}
