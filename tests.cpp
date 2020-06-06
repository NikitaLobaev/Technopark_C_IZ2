#include "gtest/gtest.h"

extern "C" {
#include <sys/mman.h>
#include <sys/sysinfo.h>
#include "matrix.h"
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(matrix_mirroring, test1) {
	auto source = new int[1]{123};
	auto destination = new int[1]{-1};
	EXPECT_EQ(matrix_mirroring(nullptr, 1, 1, destination), IZ2_ERROR_MEMORY);
	EXPECT_EQ(matrix_mirroring(source, 1, 1, nullptr), IZ2_ERROR_MEMORY);
	EXPECT_EQ(matrix_mirroring(source, 0, 1, destination), IZ2_ERROR_ARGUMENT);
	EXPECT_EQ(matrix_mirroring(source, 1, 0, destination), IZ2_ERROR_ARGUMENT);
	EXPECT_EQ(matrix_mirroring(source, 1, 1, destination), IZ2_OK);
	EXPECT_EQ(destination[0], 123);
	delete[] source;
	delete[] destination;
}

TEST(matrix_mirroring, test2) {
	const size_t source_rows_count = 3, source_columns_count = 3;
	auto source = new int[source_rows_count * source_columns_count];
	for (size_t index = 0; index < source_rows_count * source_columns_count; index++) {
		source[index] = index;
	}
	auto destination = new int[source_rows_count * source_columns_count];
	EXPECT_EQ(matrix_mirroring(source, source_rows_count, source_columns_count, destination), IZ2_OK);
	const int *destination_correct = new int[source_rows_count * source_columns_count]{8, 5, 2, 7, 4, 1, 6, 3, 0};
	EXPECT_TRUE(std::equal(destination_correct, destination_correct + source_rows_count * source_columns_count,
	                       destination));
	delete[] source;
	delete[] destination;
	delete[] destination_correct;
}

TEST(matrix_mirroring, test3) {
	const size_t source_rows_count = 10, source_columns_count = 5;
	auto source = new int[source_rows_count * source_columns_count];
	for (size_t index = 0; index < source_rows_count * source_columns_count; index++) {
		source[index] = index;
	}
	auto destination = new int[source_rows_count * source_columns_count];
	EXPECT_EQ(matrix_mirroring(source, source_rows_count, source_columns_count, destination), IZ2_OK);
	const int *destination_correct = new int[source_rows_count * source_columns_count]{49, 44, 39, 34, 29, 24, 19, 14, 9, 4,
	                                                                                   48, 43, 38, 33, 28, 23, 18, 13, 8, 3,
	                                                                                   47, 42, 37, 32, 27, 22, 17, 12, 7, 2,
	                                                                                   46, 41, 36, 31, 26, 21, 16, 11, 6, 1,
	                                                                                   45, 40, 35, 30, 25, 20, 15, 10, 5, 0};
	EXPECT_TRUE(std::equal(destination_correct, destination_correct + source_rows_count * source_columns_count,
			destination));
	delete[] source;
	delete[] destination;
	delete[] destination_correct;
}

TEST(matrix_mirroring_multiproc, test1) {
	auto source = new int[1]{123};
	auto destination = new int[1]{-1};
	EXPECT_EQ(matrix_mirroring_multiproc(nullptr, 1, 1, destination, 1), IZ2_ERROR_MEMORY);
	EXPECT_EQ(matrix_mirroring_multiproc(source, 1, 1, nullptr, 1), IZ2_ERROR_MEMORY);
	EXPECT_EQ(matrix_mirroring_multiproc(source, 0, 1, destination, 1), IZ2_ERROR_ARGUMENT);
	EXPECT_EQ(matrix_mirroring_multiproc(source, 1, 0, destination, 1), IZ2_ERROR_ARGUMENT);
	EXPECT_EQ(matrix_mirroring_multiproc(source, 1, 1, destination, 0), IZ2_ERROR_ARGUMENT);
	EXPECT_EQ(matrix_mirroring_multiproc(source, 1, 1, destination, 1), IZ2_OK);
	EXPECT_EQ(destination[0], 123);
	EXPECT_EQ(matrix_mirroring_multiproc(source, 1, 1, destination, 3), IZ2_OK);
	EXPECT_EQ(destination[0], 123);
	EXPECT_EQ(matrix_mirroring_multiproc(source, 1, 1, destination, 10), IZ2_OK);
	EXPECT_EQ(destination[0], 123);
	delete[] source;
	delete[] destination;
}

TEST(matrix_mirroring_multiproc, test2) {
	const size_t source_rows_count = 3, source_columns_count = 3;
	auto source = (int*) mmap(nullptr, source_rows_count * source_columns_count * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	for (size_t index = 0; index < source_rows_count * source_columns_count; index++) {
		source[index] = index;
	}
	auto destination = (int*) mmap(nullptr, source_columns_count * source_rows_count * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	EXPECT_EQ(matrix_mirroring_multiproc(source, source_rows_count, source_columns_count, destination, 2), IZ2_OK);
	const int *destination_correct = new int[source_rows_count * source_columns_count]{8, 5, 2, 7, 4, 1, 6, 3, 0};
	EXPECT_TRUE(std::equal(destination_correct, destination_correct + source_rows_count * source_columns_count, destination));
	munmap(source, source_rows_count * source_columns_count * sizeof(int));
	munmap(destination, source_columns_count * source_rows_count * sizeof(int));
	delete[] destination_correct;
}

TEST(matrix_mirroring_multiproc, test3) {
	const size_t source_rows_count = 10, source_columns_count = 5;
	auto source = (int*) mmap(nullptr, source_rows_count * source_columns_count * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	for (size_t index = 0; index < source_rows_count * source_columns_count; index++) {
		source[index] = index;
	}
	auto destination = (int*) mmap(nullptr, source_columns_count * source_rows_count * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	const int *destination_correct = new int[source_rows_count * source_columns_count]{49, 44, 39, 34, 29, 24, 19, 14, 9, 4,
	                                                                     48, 43, 38, 33, 28, 23, 18, 13, 8, 3,
	                                                                     47, 42, 37, 32, 27, 22, 17, 12, 7, 2,
	                                                                     46, 41, 36, 31, 26, 21, 16, 11, 6, 1,
	                                                                     45, 40, 35, 30, 25, 20, 15, 10, 5, 0};
	for (size_t max_proc_count = 1; max_proc_count <= source_rows_count; max_proc_count++) {
		EXPECT_EQ(matrix_mirroring_multiproc(source, source_rows_count, source_columns_count, destination, 4), IZ2_OK);
		EXPECT_TRUE(std::equal(destination_correct, destination_correct + source_rows_count * source_columns_count, destination));
		munmap(destination, source_columns_count * source_rows_count * sizeof(int));
		destination = (int*) mmap(nullptr, source_columns_count * source_rows_count * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	}
	munmap(source, source_rows_count * source_columns_count * sizeof(int));
	munmap(destination, source_columns_count * source_rows_count * sizeof(int));
	delete[] destination_correct;
}

TEST(matrix_mirroring_multiproc, test4) {
	const size_t source_rows_count = 10000, source_columns_count = 5000;
	auto source = (int*) mmap(nullptr, source_rows_count * source_columns_count * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	for (size_t index = 0; index < source_rows_count * source_columns_count; index++) {
		source[index] = index;
	}
	auto destination = new int[source_rows_count * source_columns_count];
	EXPECT_EQ(matrix_mirroring(source, source_rows_count, source_columns_count, destination), IZ2_OK);
	auto destination_multiproc = (int*) mmap(nullptr, source_columns_count * source_rows_count * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	const size_t nprocs = get_nprocs();
	for (size_t max_proc_count = 1; max_proc_count <= nprocs; max_proc_count++) {
		EXPECT_EQ(matrix_mirroring_multiproc(source, source_rows_count, source_columns_count, destination_multiproc, 4), IZ2_OK);
		EXPECT_TRUE(std::equal(destination, destination + source_rows_count * source_columns_count, destination_multiproc));
		munmap(destination_multiproc, source_columns_count * source_rows_count * sizeof(int));
		destination_multiproc = (int*) mmap(nullptr, source_columns_count * source_rows_count * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	}
	munmap(source, source_rows_count * source_columns_count * sizeof(int));
	munmap(destination_multiproc, source_columns_count * source_rows_count * sizeof(int));
	delete[] destination;
}
