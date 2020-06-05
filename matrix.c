#include <stdio.h>
#include "matrix.h"

int matrix_mirroring(const int *matrix_source, size_t rows_count, size_t columns_count, int *matrix_destination) {
	if (!matrix_source || !matrix_destination) {
		return IZ2_ERROR_MEMORY;
	} else if (rows_count == 0 || columns_count == 0) {
		return IZ2_ERROR_ARGUMENT;
	}
	size_t current_column = rows_count - 1, current_row = columns_count - 1; //matrix_destination
	for (const int *matrix_source_end = matrix_source + rows_count * columns_count; matrix_source < matrix_source_end; matrix_source++) {
		*(matrix_destination + current_row * rows_count + current_column) = *matrix_source;
		if (current_row == 0) {
			current_column--;
			current_row = columns_count - 1;
		} else {
			current_row--;
		}
	}
	return IZ2_OK;
}

int matrix_mirroring_multiproc(const int *matrix_source, size_t rows_count, size_t columns_count, int *matrix_destination, size_t proc_count) {
	if (!matrix_source || !matrix_destination) {
		return IZ2_ERROR_MEMORY;
	} else if (rows_count == 0 || columns_count == 0 || proc_count == 0) {
		return IZ2_ERROR_ARGUMENT;
	}
	
	return IZ2_OK;
}

int matrix_read(int *matrix, size_t rows_count, size_t columns_count) {
	if (!matrix) {
		return IZ2_ERROR_MEMORY;
	} else if (rows_count == 0 || columns_count == 0) {
		return IZ2_ERROR_ARGUMENT;
	}
	for (const int *matrix_end = matrix + rows_count * columns_count; matrix < matrix_end; matrix++) {
		scanf("%d", matrix);
	}
	return IZ2_OK;
}

int matrix_print(const int *matrix, size_t rows_count, size_t columns_count) {
	if (!matrix) {
		return IZ2_ERROR_MEMORY;
	} else if (rows_count == 0 || columns_count == 0) {
		return IZ2_ERROR_ARGUMENT;
	}
	const int *matrix_end = matrix + rows_count * columns_count;
	for (size_t index = 1; matrix < matrix_end; index++, matrix++) {
		printf("%d ", *matrix);
		if (index % rows_count == 0) {
			printf("\n");
		}
	}
	return IZ2_OK;
}
