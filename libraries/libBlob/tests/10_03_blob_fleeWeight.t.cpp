#include <gtest/gtest.h>
#include <blob.h>

TEST (test_10_03_blob_fleeWeight_t, avoidDamageWeight_peer)
{
	Blob b1 = CreateBlob ().HP (200U).damage (200U).lifespan (1000U).maxHunger (100U);

	EXPECT_DOUBLE_EQ (b1.avoidDamageWeight (b1), 0.0049751243781094527);
}

TEST (test_10_03_blob_fleeWeight_t, avoidDamageWeight_positive)
{
	Blob Attacker = CreateBlob ().HP (100U).damage (200U).lifespan (1000U).maxHunger (100U);
	Blob runner = CreateBlob ().HP (100U).lifespan (100U).maxHunger (100U);

	EXPECT_DOUBLE_EQ (runner.avoidDamageWeight (Attacker), 51.0 / 151.0);
}

TEST (test_10_03_blob_fleeWeight_t, avoidDamageWeight_negative)
{
	Blob Attacker = CreateBlob ().HP (100U).damage (100U).lifespan (1000U).maxHunger (100U);
	Blob runner = CreateBlob ().HP (200U).lifespan (100U).maxHunger (100U);

	EXPECT_DOUBLE_EQ (runner.avoidDamageWeight (Attacker), -49.0 / 151.0);
}

TEST (test_10_03_blob_fleeWeight_t, avoidDamageWeight_increases_for_stronger_attackers)
{
	Blob runner = CreateBlob ().HP (100U).lifespan (1000U).maxHunger (100U);
	Blob weakAttacker  = CreateBlob ().HP (100U).damage (50U).lifespan (1000U).maxHunger (100U);
	Blob strongAttacker  = CreateBlob ().HP (100U).damage (100U).lifespan (1000U).maxHunger (100U);

	EXPECT_GT (runner.avoidDamageWeight (strongAttacker),
		   runner.avoidDamageWeight (weakAttacker));
}

TEST (test_10_03_blob_fleeWeight_t, avoidDamageWeight_increases_for_weaker_defenders)
{
	Blob weakRunner = CreateBlob ().HP (50U).lifespan (1000U).maxHunger (100U);
	Blob strongRunner  = CreateBlob ().HP (100U).lifespan (1000U).maxHunger (100U);
	Blob attacker  = CreateBlob ().HP (100U).damage (100U).lifespan (1000U).maxHunger (100U);

	EXPECT_GT (weakRunner.avoidDamageWeight (attacker),
		   strongRunner.avoidDamageWeight (attacker));
}

TEST (test_10_03_blob_fleeWeight_t, fleeWeight)
{
	Blob attacker = CreateBlob ().position (make_pt (9.0, 12.0)).HP (200U).damage (220U).lifespan (1000U).maxHunger (100U);
	Blob defender = CreateBlob ().position  (make_pt (3.0, 4.0)).smell (100.0).HP (200U).lifespan (1000U).maxHunger (100U);

	EXPECT_DOUBLE_EQ (defender.fleeWeight (attacker), 0.8 * 11.0 / 211.0);
}

TEST (test_10_03_blob_fleeWeight_t, fleeWeight_scales_with_distance)
{
	Blob attacker = CreateBlob ().position (make_pt (3.0, 4.0)).HP (100U).damage (100U).lifespan (1000U).maxHunger (100U);
	Blob defender1 = CreateBlob ().position (make_pt (10.0, 20.0)).smell (100.0).HP (50U).lifespan (1000U).maxHunger (100U);
	Blob defender2 = CreateBlob ().position (make_pt (20.0, 40.0)).smell (100.0).HP (50U).lifespan (100U).maxHunger (100U);

	EXPECT_GT (defender1.fleeWeight (attacker), defender2.fleeWeight (attacker));
}

TEST (test_10_03_blob_fleeWeight_t, distance_does_not_affect_fleeWeight_in_same_square)
{
	Blob attacker = CreateBlob ().position (make_pt (3.0, 4.0)).HP (100U).damage (100U).lifespan (1000U).maxHunger (100U);
	Blob defender1 = CreateBlob ().position (make_pt (3.1, 4.0)).smell (100.0).HP (50U).lifespan (1000U).maxHunger (100U);
	Blob defender2 = CreateBlob ().position (make_pt (3.9, 4.0)).smell (100.0).HP (50U).lifespan (1000U).maxHunger (100U);

	EXPECT_DOUBLE_EQ (defender1.fleeWeight (attacker), defender2.fleeWeight (attacker));
}

