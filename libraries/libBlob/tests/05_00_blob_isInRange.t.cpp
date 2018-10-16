#include <gtest/gtest.h>
#include <blob.h>

TEST (test_05_01_blob_isInRange_t, out_of_range_x)
{
	Blob b1 = CreateBlob ().position (make_pt (0.0, 0.0));
	Blob b2 = CreateBlob ().position (make_pt (10.0, 0.0));

	EXPECT_FALSE (b1.isInRange (b2, 9.0));
	EXPECT_FALSE (b2.isInRange (b1, 9.0));
}

TEST (test_05_01_blob_isInRange_t, in_range_x)
{
	Blob b1 = CreateBlob ().position (make_pt (0.0, 0.0));
	Blob b2 = CreateBlob ().position (make_pt (10.0, 0.0));

	EXPECT_TRUE (b1.isInRange (b2, 10.0));
	EXPECT_TRUE (b2.isInRange (b1, 10.0));
}

TEST (test_05_01_blob_isInRange_t, out_of_range_y)
{
	Blob b1 = CreateBlob ().position (make_pt (0.0, 0.0));
	Blob b2 = CreateBlob ().position (make_pt (10.0, 10.0));

	EXPECT_FALSE (b1.isInRange (b2, 9.0));
	EXPECT_FALSE (b2.isInRange (b1, 9.0));
}

TEST (test_05_01_blob_isInRange_t, in_range_y)
{
	Blob b1 = CreateBlob ().position (make_pt (0.0, 0.0));
	Blob b2 = CreateBlob ().position (make_pt (0.0, 10.0));

	EXPECT_TRUE (b1.isInRange (b2, 10.0));
	EXPECT_TRUE (b2.isInRange (b1, 10.0));
}

TEST (test_05_01_blob_isInRange_t, out_of_range_xy)
{
	Blob b1 = CreateBlob ().position (make_pt (3.0, 4.0));
	Blob b2 = CreateBlob ().position (make_pt (6.0, 8.0));

	EXPECT_FALSE (b1.isInRange (b2, 4.0));
	EXPECT_FALSE (b2.isInRange (b1, 4.0));
}

TEST (test_05_01_blob_isInRange_t, in_range_xy)
{
	Blob b1 = CreateBlob ().position (make_pt (3.0, 4.0));
	Blob b2 = CreateBlob ().position (make_pt (6.0, 8.0));

	EXPECT_TRUE (b1.isInRange (b2, 5.0));
	EXPECT_TRUE (b2.isInRange (b1, 5.0));
}

