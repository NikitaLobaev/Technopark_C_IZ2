#include "gtest/gtest.h"

extern "C" {
#include "matrix.h"
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(matrix_mirroring, test1) {
	auto matrix_source = new int[1]{123};
	int *matrix_destination = new int[1]{-1};
	ASSERT_EQ(matrix_mirroring(nullptr, 1, 1, matrix_destination), IZ2_ERROR_MEMORY);
	ASSERT_EQ(matrix_mirroring(matrix_source, 1, 1, nullptr), IZ2_ERROR_MEMORY);
	ASSERT_EQ(matrix_mirroring(matrix_source, 0, 1, matrix_destination), IZ2_ERROR_ARGUMENT);
	ASSERT_EQ(matrix_mirroring(matrix_source, 1, 0, matrix_destination), IZ2_ERROR_ARGUMENT);
	ASSERT_EQ(matrix_mirroring(matrix_source, 1, 1, matrix_destination), IZ2_OK);
	ASSERT_EQ(matrix_destination[0], 123);
	delete[] matrix_source;
	delete[] matrix_destination;
}

TEST(matrix_mirroring, test2) {
	const size_t rows_count = 3, columns_count = 3;
	auto matrix_source = new int[rows_count * columns_count];
	for (size_t index = 0; index < rows_count * columns_count; index++) {
		matrix_source[index] = index;
	}
	int *matrix_destination = new int[rows_count * columns_count];
	ASSERT_EQ(matrix_mirroring(matrix_source, rows_count, columns_count, matrix_destination), IZ2_OK);
	const int *matrix_destination_correct = new int[rows_count * columns_count]{8, 5, 2, 7, 4, 1, 6, 3, 0};
	ASSERT_TRUE(std::equal(matrix_destination_correct, matrix_destination_correct + rows_count * columns_count,
	                       matrix_destination));
	delete[] matrix_source;
	delete[] matrix_destination;
}

TEST(matrix_mirroring, test3) {
	const size_t rows_count = 10, columns_count = 5;
	auto matrix_source = new int[rows_count * columns_count];
	for (size_t index = 0; index < rows_count * columns_count; index++) {
		matrix_source[index] = index;
	}
	int *matrix_destination = new int[rows_count * columns_count];
	ASSERT_EQ(matrix_mirroring(matrix_source, rows_count, columns_count, matrix_destination), IZ2_OK);
	const int *matrix_destination_correct = new int[rows_count * columns_count]{49, 44, 39, 34, 29, 24, 19, 14, 9, 4,
																			 48, 43, 38, 33, 28, 23, 18, 13, 8, 3,
																			 47, 42, 37, 32, 27, 22, 17, 12, 7, 2,
																			 46, 41, 36, 31, 26, 21, 16, 11, 6, 1,
																			 45, 40, 35, 30, 25, 20, 15, 10, 5, 0};
	ASSERT_TRUE(std::equal(matrix_destination_correct, matrix_destination_correct + rows_count * columns_count,
			matrix_destination));
	delete[] matrix_source;
	delete[] matrix_destination;
}
