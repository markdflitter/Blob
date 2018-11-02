#include <gtest/gtest.h>
#include <blob.h>

namespace
{
	auto weakest = CreateBlob ().HP (25U).position (make_pt (5.0, 5.0)).lifespan (1000U).maxHunger (100U);
	auto weaker = CreateBlob ().HP (50U).position (make_pt (5.0, 5.0)).lifespan (1000U).maxHunger (100U);
	auto stronger = CreateBlob ().HP (200U).damage (100U).position (make_pt (5.0, 5.0)).lifespan (1000U).maxHunger (100U);
	auto strongest = CreateBlob ().HP (200U).damage (200U).position (make_pt (5.0, 5.0)).lifespan (1000U).maxHunger (100U);
}

TEST (test_11_02_blob_selectBestOption_same_square_t, weaker_flees_stronger)
{
	std::vector <Blob> b {weaker, stronger};
	EXPECT_TRUE (b[0].chooseBestOption (b).matches (Option(flee, 0, &b[1])));
}

TEST (test_11_02_blob_selectBestOption_same_square_t, weaker_flees_strongest)
{
	std::vector <Blob> b {weaker, stronger, strongest};
	EXPECT_TRUE (b[0].chooseBestOption (b).matches (Option(flee, 0, &b[2])));
}

TEST (test_11_02_blob_selectBestOption_same_square_t, weakest_flees)
{
	std::vector <Blob> b {weakest, weaker, stronger};
	EXPECT_TRUE (b[0].chooseBestOption (b).matches (Option(flee, 0, &b[2])));
	EXPECT_TRUE (b[1].chooseBestOption (b).matches (Option(flee, 0, &b[2])));
	EXPECT_GT (b[0].chooseBestOption (b).weight (), b[1].chooseBestOption (b).weight ());
}

TEST (test_11_02_blob_selectBestOption_same_square_t, weaker_wont_flee_if_aggressive)
{
	std::vector <Blob> b {weakest.aggression (1.0), stronger};
	EXPECT_TRUE (b[0].chooseBestOption (b).matches (Option(attack, 0, &b[1])));
}

TEST (test_11_02_blob_selectBestOption_same_square_t, stronger_attacks_weaker)
{
	std::vector <Blob> b {stronger, weaker};
	EXPECT_TRUE (b[0].chooseBestOption (b).matches (Option(attack, 0, &b[1])));
}

TEST (test_11_02_blob_selectBestOption_same_square_t, stronger_attacks_weakest)
{
	std::vector <Blob> b {stronger, weaker, weakest};
	EXPECT_TRUE (b[0].chooseBestOption (b).matches (Option(attack, 0, &b[2])));
}

TEST (test_11_02_blob_selectBestOption_same_square_t, strongest_attacks)
{
	std::vector <Blob> b {strongest, stronger, weaker};
	EXPECT_TRUE (b[0].chooseBestOption (b).matches (Option(attack, 0, &b[2])));
	EXPECT_TRUE (b[1].chooseBestOption (b).matches (Option(attack, 0, &b[2])));
	EXPECT_GT (b[0].chooseBestOption (b).weight (), b[1].chooseBestOption (b).weight ());
}

TEST (test_11_02_blob_selectBestOption_same_square_t, stronger_wont_attack_if_cowardly)
{
	std::vector <Blob> b {stronger.aggression (-1.0), weaker};
	EXPECT_TRUE (b[0].chooseBestOption (b).matches (Option(flee, 0, &b[1])));
}

