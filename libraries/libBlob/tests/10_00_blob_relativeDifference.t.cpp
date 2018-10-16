#include <gtest/gtest.h>
#include <blob.h>

TEST (test_10_00_blob_relativeDifference_t, relativeDifference_zero)
{
	Blob b = CreateBlob ();
	EXPECT_DOUBLE_EQ (b.relativeDifference (1.0, 0.0), 0.5);
	EXPECT_DOUBLE_EQ (b.relativeDifference (0.0, 1.0), -0.5);
}

TEST (test_10_00_blob_relativeDifference_t, relativeDifference_non_zero)
{
	Blob b = CreateBlob ();
	EXPECT_DOUBLE_EQ (b.relativeDifference (50.0, 50.0), 0.0);
	EXPECT_DOUBLE_EQ (b.relativeDifference (100.0, 50.0), 1.0 / 6.0);
	EXPECT_DOUBLE_EQ (b.relativeDifference (50.0, 100.0), -1.0 / 6.0);
}

TEST (test_10_00_blob_relativeDifference_t, relativeDifference_negative_asserts)
{
	Blob b = CreateBlob ();
	EXPECT_DEATH (b.relativeDifference (-50.0, 50.0), "");
	EXPECT_DEATH (b.relativeDifference (50.0, -50.0), "");
	EXPECT_DEATH (b.relativeDifference (-50.0, -50.0), "");
}

TEST (test_10_00_blob_relativeDifference_t, relativeDifference_magnitude)
{
	Blob b = CreateBlob ();
	EXPECT_GT (b.relativeDifference (1000, 50), b.relativeDifference (100, 50));
	EXPECT_GT (b.relativeDifference (1000, 50), b.relativeDifference (1000, 500));
}

