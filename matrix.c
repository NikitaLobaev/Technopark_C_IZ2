#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "matrix.h"

u_int8_t matrix_mirroring_(const int *source, size_t source_rows_count, size_t source_columns_count, int *destination,
		size_t source_row_start, size_t source_row_end) { //[source_row_start, source_row_end)
	if (source_row_start >= source_row_end) {
		return IZ2_ERROR_ARGUMENT;
	}
	size_t source_row_current = source_row_start, source_column_current = 0;
	for (source += source_columns_count * source_row_start; source_row_current < source_row_end; source++) {
		*(destination + source_rows_count * (source_columns_count - source_column_current - 1) + source_rows_count - source_row_current - 1) = *source;
		source_column_current++;
		if (source_column_current == source_columns_count) {
			source_row_current++;
			source_column_current = 0;
		}
	}
	return IZ2_OK;
}

u_int8_t matrix_mirroring(const int *source, size_t source_rows_count, size_t source_columns_count, int *destination) {
	if (!source || !destination) {
		return IZ2_ERROR_MEMORY;
	} else if (source_rows_count == 0 || source_columns_count == 0) {
		return IZ2_ERROR_ARGUMENT;
	}
	return matrix_mirroring_(source, source_rows_count, source_columns_count, destination, 0, source_rows_count);
}

u_int8_t matrix_mirroring_multiproc(const int *source, size_t source_rows_count, size_t source_columns_count, int *destination, size_t proc_max_count) {
	if (!source || !destination) {
		return IZ2_ERROR_MEMORY;
	} else if (source_rows_count == 0 || source_columns_count == 0 || proc_max_count == 0) {
		return IZ2_ERROR_ARGUMENT;
	}
	if (proc_max_count > source_rows_count) {
		proc_max_count = source_rows_count;
	}
	for (size_t proc_current = 0; proc_current < proc_max_count - 1; proc_current++) {
		pid_t pid = fork();
		if (pid == -1) {
			return IZ2_ERROR_FORK_FAILED;
		} else if (pid == 0) { //порождённый процесс
			exit(matrix_mirroring_(source, source_rows_count, source_columns_count, destination, (source_rows_count / proc_max_count) * proc_current, (source_rows_count / proc_max_count) * (proc_current + 1)));
		}
	}
	u_int8_t error = matrix_mirroring_(source, source_rows_count, source_columns_count, destination, (source_rows_count / proc_max_count) * (proc_max_count - 1), source_rows_count);
	for (size_t proc_current = 1; proc_current < proc_max_count; proc_current++) {
		int status;
		if (wait(&status) < 0) {
			return IZ2_ERROR_FORK_FAILED;
		}
		if (!WIFEXITED(status)) {
			error |= (u_int8_t) WEXITSTATUS(status);
		}
	}
	return error;
}

u_int8_t matrix_read(int *matrix, size_t rows_count, size_t columns_count) {
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

u_int8_t matrix_print(const int *matrix, size_t rows_count, size_t columns_count) {
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
