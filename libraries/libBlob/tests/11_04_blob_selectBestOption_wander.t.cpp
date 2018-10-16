#include <gtest/gtest.h>
#include <blob.h>

namespace
{
	auto weaker = CreateBlob ().HP (50U).position (make_pt (5.0, 5.0));
	auto stronger = CreateBlob ().HP (200U).smell (100.0).damage (100U).position (make_pt (10.0, 10.0));
}

TEST (test_11_03_blob_selectBestOption_wander_t, wanders)
{
	std::vector <Blob> b {weaker, stronger};
	EXPECT_TRUE (b[0].chooseBestOption (b).matches (Option(wander, 0, 0)));
}

