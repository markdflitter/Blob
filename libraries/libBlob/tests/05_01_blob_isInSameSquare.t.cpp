#include <gtest/gtest.h>
#include <blob.h>

TEST (test_05_01_isInSameSquare_t, different_squares)
{
	Blob b1 = CreateBlob ().position (make_pt (10.0, 20.0));
	Blob b2 = CreateBlob ().position (make_pt (20.0, 10.0));

	EXPECT_FALSE (b1.isInSameSquare (b2));
	EXPECT_FALSE (b2.isInSameSquare (b1));
}

TEST (test_05_01_isInSameSquare_t, identical_position)
{
	Blob b1 = CreateBlob ().position (make_pt (10.0, 20.0));
	Blob b2 = CreateBlob ().position (make_pt (10.0, 20.0));

	EXPECT_TRUE (b1.isInSameSquare (b2));
	EXPECT_TRUE (b2.isInSameSquare (b1));
}

TEST (test_05_01_isInSameSquare_t, different_position_in_square)
{
	Blob b1 = CreateBlob ().position (make_pt (10.1, 20.1));
	Blob b2 = CreateBlob ().position (make_pt (10.9, 20.9));

	EXPECT_TRUE (b1.isInSameSquare (b2));
	EXPECT_TRUE (b2.isInSameSquare (b1));
}

TEST (test_05_01_isInSameSquare_t, minus_different_position_in_square)
{
	Blob b1 = CreateBlob ().position (make_pt (-10.1, -20.1));
	Blob b2 = CreateBlob ().position (make_pt (-10.9, -20.9));

	EXPECT_TRUE (b1.isInSameSquare (b2));
	EXPECT_TRUE (b2.isInSameSquare (b1));
}

TEST (test_05_01_isInSameSquare_t, different_squares_but_still_in_range)
{
	Blob b1 = CreateBlob ().position (make_pt (10.6, 20.7));
	Blob b2 = CreateBlob ().position (make_pt (11.3, 21.2));

	EXPECT_TRUE (b1.isInSameSquare (b2));
	EXPECT_TRUE (b2.isInSameSquare (b1));
}

