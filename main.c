#include <sys/sysinfo.h>
#include "run.c"

int main(int argc, char **argv) {
	if (argc != 3 && argc != 4) {
		printf(
				"Usage: ./matrix <количество строк> <количество столбцов> "
				"(<максимальное количество процессов>)");
		return IZ2_ERROR_ARGUMENT;
	}
	const size_t source_rows_count = (size_t)atoi(argv[1]),
			source_columns_count = (size_t)atoi(argv[2]);
	if (argc == 4) {
		return run(source_rows_count, source_columns_count, get_nprocs());
	} else {
		return run(source_rows_count, source_columns_count, (size_t)atoi(argv[3]));
	}
}
