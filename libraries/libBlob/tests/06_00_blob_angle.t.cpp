#include <gtest/gtest.h>
#include <blob.h>

TEST (test_06_00_blob_angle_t, north)
{
	Blob b1 = CreateBlob ().position (make_pt (5.0, 5.0));
	Blob b2 = CreateBlob ().position (make_pt (5.0, 10.0));

	double angle = b1.angle (b2);
	EXPECT_DOUBLE_EQ (angle, 0.0);
}

TEST (test_06_00_blob_angle_t, south)
{
	Blob b1 = CreateBlob ().position (make_pt (-5.0, -5.0));
	Blob b2 = CreateBlob ().position (make_pt (-5.0, -10.0));

	double angle = b1.angle (b2);
	EXPECT_DOUBLE_EQ (angle, M_PI);
}

TEST (test_06_00_blob_angle_t, east)
{
	Blob b1 = CreateBlob ().position (make_pt (5.0, 5.0));
	Blob b2 = CreateBlob ().position (make_pt (10.0, 5.0));

	double angle = b1.angle (b2);
	EXPECT_DOUBLE_EQ (angle, M_PI / 2.0);
}

TEST (test_06_00_blob_angle_t, west)
{
	Blob b1 = CreateBlob ().position (make_pt (-5.0, 5.0));
	Blob b2 = CreateBlob ().position (make_pt (-10.0, 5.0));

	double angle = b1.angle (b2);
	EXPECT_DOUBLE_EQ (angle, 3.0 * M_PI / 2.0);
}

TEST (test_06_00_blob_angle_t, north_east)
{
	Blob b1 = CreateBlob ().position (make_pt (5.0, 5.0));
	Blob b2 = CreateBlob ().position (make_pt (10.0, 10.0));

	double angle = b1.angle (b2);
	EXPECT_DOUBLE_EQ (angle, M_PI / 4);
}

TEST (test_06_00_blob_angle_t, north_west)
{
	Blob b1 = CreateBlob ().position (make_pt (-5.0, 5.0));
	Blob b2 = CreateBlob ().position (make_pt (-10.0, 10.0));

	double angle = b1.angle (b2);
	EXPECT_DOUBLE_EQ (angle, -M_PI / 4);
}

TEST (test_06_00_blob_angle_t, south_east)
{
	Blob b1 = CreateBlob ().position (make_pt (5.0, -5.0));
	Blob b2 = CreateBlob ().position (make_pt (10.0, -10.0));

	double angle = b1.angle (b2);
	EXPECT_DOUBLE_EQ (angle, 3 * M_PI / 4);
}

TEST (test_06_00_blob_angle_t, south_west)
{
	Blob b1 = CreateBlob ().position (make_pt (-5.0, -5.0));
	Blob b2 = CreateBlob ().position (make_pt (-10.0, -10.0));

	double angle = b1.angle (b2);
	EXPECT_DOUBLE_EQ (angle, 5 * M_PI / 4);
}

