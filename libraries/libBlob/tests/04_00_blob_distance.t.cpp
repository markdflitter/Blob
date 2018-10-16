#include <gtest/gtest.h>
#include <blob.h>

TEST (test_04_00_blob_distance_t, distance_equals_0)
{
	Blob b1 = CreateBlob ().position (make_pt (10.0, 10.0));
	Blob b2 = CreateBlob ().position (make_pt (10.0, 10.0));

	EXPECT_DOUBLE_EQ (b1.distance (b2), 0.0);
	EXPECT_DOUBLE_EQ (b2.distance (b1), 0.0);
}

TEST (test_04_00_blob_distance_t, distance_345_triangle_at_origin)
{
	Blob b1 = CreateBlob ().position (make_pt (0.0, 0.0));
	Blob b2 = CreateBlob ().position (make_pt (3.0, 4.0));

	EXPECT_DOUBLE_EQ (b1.distance (b2), 5.0);
	EXPECT_DOUBLE_EQ (b2.distance (b1), 5.0);
}

TEST (test_04_00_blob_distance_t, distance_minus_345_triangle_at_origin)
{
	Blob b1 = CreateBlob ().position (make_pt (0.0, 0.0));
	Blob b2 = CreateBlob ().position (make_pt (-3.0, -4.0));

	EXPECT_DOUBLE_EQ (b1.distance (b2), 5.0);
	EXPECT_DOUBLE_EQ (b2.distance (b1), 5.0);
}

TEST (test_04_00_blob_distance_t, distance_345_triangle_off_origin)
{
	Blob b1 = CreateBlob ().position (make_pt (3.0, 4.0));
	Blob b2 = CreateBlob ().position (make_pt (6.0, 8.0));

	EXPECT_DOUBLE_EQ (b1.distance (b2), 5.0);
	EXPECT_DOUBLE_EQ (b2.distance (b1), 5.0);
}

TEST (test_04_00_blob_distance_t, distance_minus_345_triangle_off_origin)
{
	Blob b1 = CreateBlob ().position (make_pt (-3.0, -4.0));
	Blob b2 = CreateBlob ().position (make_pt (-6.0, -8.0));

	EXPECT_DOUBLE_EQ (b1.distance (b2), 5.0);
	EXPECT_DOUBLE_EQ (b2.distance (b1), 5.0);
}

TEST (test_04_00_blob_distance_t, distance_minus_345_triangle_cross_origin)
{
	Blob b1 = CreateBlob ().position (make_pt (-2.0, -2.0));
	Blob b2 = CreateBlob ().position (make_pt (1.0, 2.0));

	EXPECT_DOUBLE_EQ (b1.distance (b2), 5.0);
	EXPECT_DOUBLE_EQ (b2.distance (b1), 5.0);
}

