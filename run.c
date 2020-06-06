#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "matrix.h"

int run(int argc, char **argv) {
	if (argc != 3 && argc != 4) {
		printf("Usage: ./matrix <количество строк> <количество столбцов> (<максимальное количество процессов>)");
		return IZ2_ERROR_ARGUMENT;
	}
	u_int8_t error = IZ2_OK;
	const size_t source_rows_count = (size_t) atoi(argv[1]), source_columns_count = (size_t) atoi(argv[2]);
	const size_t proc_count = (argc == 4 ? (size_t) atoi(argv[3]) : 1);
	if (proc_count == 0) {
		return IZ2_ERROR_ARGUMENT;
	}
	int *source, *destination;
	if (proc_count == 1) {
		source = (int*) malloc(source_rows_count * source_columns_count * sizeof(int));
		destination = (int*) malloc(source_columns_count * source_rows_count * sizeof(int));
	} else {
		source = (int*) mmap(NULL, source_rows_count * source_columns_count * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
		destination = (int*) mmap(NULL, source_columns_count * source_rows_count * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	}
	if (!source || !destination) {
		error |= IZ2_ERROR_MEMORY;
	}
	error |= matrix_read(source, source_rows_count, source_columns_count);
	if (proc_count == 1) {
		error |= matrix_mirroring(source, source_rows_count, source_columns_count, destination);
	} else {
		error |= matrix_mirroring_multiproc(source, source_rows_count, source_columns_count, destination, proc_count);
	}
	error |= matrix_print(destination, source_rows_count, source_columns_count);
	if (proc_count == 1) {
		free(source);
		free(destination);
	} else {
		munmap(source, source_rows_count * source_columns_count * sizeof(int));
		munmap(destination, source_columns_count * source_rows_count * sizeof(int));
	}
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
