#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main(int argc, char **argv) {
	if (argc != 3) {
		printf("Usage: matrix <rows_count> <columns_count>");
		return IZ2_ERROR_ARGUMENT;
	}
	int result;
	const size_t rows_count = (size_t) argv[1], columns_count = (size_t) argv[2];
	int *matrix_source = (int*) malloc(rows_count * columns_count * sizeof(int));
	int *matrix_destination = (int*) malloc(columns_count * rows_count * sizeof(int));
	if (!matrix_source || !matrix_destination) {
		return IZ2_ERROR_MEMORY;
	}
	result = matrix_read(matrix_source, rows_count, columns_count);
	if (!result) {
		return result;
	}
	result = matrix_mirroring(matrix_source, rows_count, columns_count, matrix_destination);
	if (!result) {
		return result;
	}
	result = matrix_print(matrix_destination, rows_count, columns_count);
	if (!result) {
		return result;
	}
	return 0;
}
