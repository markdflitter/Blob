#include <gtest/gtest.h>
#include <blob.h>

namespace
{
	auto blob = CreateBlob ().HP (400U).damage (400U).position (make_pt (5.0, 5.0)).lifespan (1000U).maxHunger (100U);
}

TEST (test_11_05_blob_selectBestOption_aggression_t, aggressionFn_is_applied_to_attackWeight)
{
	std::vector <Blob> b {blob.aggressionFn ([](double) {return 1000;}), blob};
	EXPECT_TRUE (b[0].chooseBestOption (b).matches (Option(attack, 0, &b[1])));
	EXPECT_EQ (b[0].chooseBestOption (b).weight (), 1000);
}

TEST (test_11_05_blob_selectBestOption_aggression_t, aggressionFn_is_applied_to_fleeWeight)
{
	std::vector <Blob> b {blob.aggressionFn ([](double) {return -1000;}), blob};
	EXPECT_TRUE (b[0].chooseBestOption (b).matches (Option(flee, 0, &b[1])));
	EXPECT_EQ (b[0].chooseBestOption (b).weight (), 1000.0 + (1.0 / 401.0));
}

TEST (test_11_05_blob_selectBestOption_aggression_t, RandomAggression)
{
	RandomAggression rnd (0);
	for (size_t i = 0; i < 100; ++i)
	{
		double r = rnd (0);
		EXPECT_GE (r, -1.0);
		EXPECT_LE (r, 1.0);
	}
}

TEST (test_11_05_blob_selectBestOption_aggression_t, RandomAggression_different_seeds_give_different_result)
{
	RandomAggression rnd1 (0);
	RandomAggression rnd2 (1);
	EXPECT_NE (rnd1 (0), rnd2 (0));
}

TEST (test_11_05_blob_selectBestOption_aggression_t, RandomAggression_uses_base_aggression)
{
	RandomAggression rnd (0);
	EXPECT_DOUBLE_EQ (rnd (10.0), 1.0);
	EXPECT_DOUBLE_EQ (rnd (-10.0), -1.0);
}

TEST (test_11_05_blob_selectBestOption_aggression_t, RandomAggression_reduces_high_aggression)
{
	RandomAggression rnd (0);
	double result = 0.0;
	for (size_t i = 0; i < 100; ++i)
	{
		result += rnd (1.0);
	}

	EXPECT_LT (result, 100);
}

TEST (test_11_05_blob_selectBestOption_aggression_t, RandomAggression_increases_high_aggression)
{
	RandomAggression rnd (0);
	double result = 0.0;
	for (size_t i = 0; i < 100; ++i)
	{
		result += rnd (-1.0);
	}

	EXPECT_GT (result, -100.0);
}

TEST (test_11_05_blob_selectBestOption_aggression_t, different_options_get_different_aggression)
{
	std::vector <Blob> b1 {blob.damage (200U).HP (200U), blob.damage (198U).HP (200U)};
	std::vector <Blob> b2 {blob.damage (200U).HP (200U).aggressionFn (RandomAggression (0)), blob.damage (198U).HP (200U)};
	auto options1 = b1[0].findOptions (b1);
	auto options2 = b2[0].findOptions (b2);

	EXPECT_DOUBLE_EQ (options1 [0].weight (), options1 [1].weight ());
	EXPECT_NE (options2 [0].weight (), options2 [1].weight ());
}

TEST (test_11_05_blob_selectBestOption_aggression_t, aggressionFn_forces_attack_same_square)
{
	auto aggressionFn = [] (double) 
	{
		static unsigned int count = 0;
		return count++ == 0 ? 1000 : 0;
	};

	std::vector <Blob> b {blob.aggressionFn (aggressionFn), blob};
	auto option = b[0].chooseBestOption(b);

	EXPECT_TRUE (option.matches (Option (attack, 0, &b[1])));
}

TEST (test_11_05_blob_selectBestOption_aggression_t, aggressionFn_forces_flee_same_square)
{
	auto aggressionFn = [] (double) 
	{
		static unsigned int count = 0;
		return count++ == 1 ? -1000 : 0;
	};

	std::vector <Blob> b {blob.aggressionFn (aggressionFn), blob};
	auto option = b[0].chooseBestOption(b);

	EXPECT_TRUE (option.matches (Option (flee, 0, &b[1])));
}

TEST (test_11_05_blob_selectBestOption_aggression_t, aggressionFn_forces_attack_in_range)
{
	auto aggressionFn = [] (double) 
	{
		static unsigned int count = 0;
		std::cout << count << std::endl;
		return count++ == 0 ? 1000 : 0;
	};

	std::vector <Blob> b {blob.smell (100.0).aggressionFn (aggressionFn), blob.position (make_pt (10.0, 10.0))};
	auto option = b[0].chooseBestOption(b);

	std::cout << option.action () << " " << option.weight () << std::endl;
	EXPECT_TRUE (option.matches (Option (attack, 0, &b[1])));
}

TEST (test_11_05_blob_selectBestOption_aggression_t, aggressionFn_forces_flee_in_range)
{
	auto aggressionFn = [] (double) 
	{
		static unsigned int count = 0;
		return count++ == 1 ? -1000 : 0;
	};

	std::vector <Blob> b {blob.smell (100.0).aggressionFn (aggressionFn), blob.position (make_pt (10.0, 10.0))};
	auto option = b[0].chooseBestOption(b);

	EXPECT_TRUE (option.matches (Option (flee, 0, &b[1])));
}

