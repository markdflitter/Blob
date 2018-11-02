#include <gtest/gtest.h>
#include <blob.h>

TEST (test_10_02_blob_attackWeight_t, inflictDamageWeight_peer)
{
	Blob b1 = CreateBlob ().HP (100U).damage (100U).lifespan (1000U).maxHunger (100U);

	EXPECT_DOUBLE_EQ (b1.inflictDamageWeight (b1), 0.0);
}


TEST (test_10_02_blob_attackWeight_t, inflictDamageWeight_positive)
{
	Blob attacker = CreateBlob ().HP (100U).damage (110U).lifespan (1000U).maxHunger (100U);
	Blob target = CreateBlob ().HP (100U).lifespan (1000U).maxHunger (100U);

	EXPECT_DOUBLE_EQ (attacker.inflictDamageWeight (target), 10.0 / 210.0);	
}

TEST (test_10_02_blob_attackWeight_t, inflictDamageWeight_negative)
{
	Blob attacker = CreateBlob ().HP (100U).damage (90U).lifespan (1000U).maxHunger (100U);
	Blob target = CreateBlob ().HP (100U).lifespan (1000U).maxHunger (100U);

	EXPECT_DOUBLE_EQ (attacker.inflictDamageWeight (target), -10.0 / 190.0);
}

TEST (test_10_02_blob_attackWeight_t, inflictDamageWeight_increases_for_stronger_attackers)
{
	Blob target = CreateBlob ().HP (500U).lifespan (1000U).maxHunger (100U);
	Blob weakAttacker = CreateBlob ().HP (100U).damage (50U).lifespan (1000U).maxHunger (100U);
	Blob strongAttacker = CreateBlob ().HP (100U).damage (100U).lifespan (1000U).maxHunger (100U);

	EXPECT_GT (strongAttacker.inflictDamageWeight (target),
		   weakAttacker.inflictDamageWeight (target));
}

TEST (test_10_02_blob_attackWeight_t, inflictDamageWeight_increases_for_weaker_defenders)
{
	Blob weakTarget = CreateBlob ().HP (50U).lifespan (1000U).maxHunger (100U);
	Blob strongTarget = CreateBlob ().HP (100U).lifespan (1000U).maxHunger (100U);
	Blob attacker = CreateBlob ().HP (100U).damage (100U).lifespan (1000U).maxHunger (100U);

	EXPECT_GT (attacker.inflictDamageWeight (weakTarget),
		   attacker.inflictDamageWeight (strongTarget));
}

TEST (test_10_02_blob_attackWeight_t, attackWeight)
{
	Blob attacker = CreateBlob ().position (make_pt (3.0, 4.0)).smell (100.0).HP (100U).damage (110U).lifespan (1000U).maxHunger (100U);
	Blob defender = CreateBlob ().position (make_pt (9.0, 12.0)).HP (100U).lifespan (1000U).maxHunger (100U);

	EXPECT_DOUBLE_EQ (attacker.attackWeight (defender), 0.8 * 1.0 / 21.0);
}

TEST (test_10_02_blob_attackWeight_t, attackWeight_scales_with_distance)
{
	Blob attacker = CreateBlob ().position (make_pt (3.0, 4.0)).smell (100.0).HP (100U).damage (100U).lifespan (1000U).maxHunger (100U);
	Blob defender1 = CreateBlob ().position (make_pt (10.0, 20.0)).HP (50U).lifespan (1000U).maxHunger (100U);
	Blob defender2 = CreateBlob ().position (make_pt (20.0, 40.0)).HP (50U).lifespan (1000U).maxHunger (100U);

	EXPECT_GT (attacker.attackWeight (defender1), attacker.attackWeight (defender2));
}

TEST (test_10_02_blob_attackWeight_t, distance_does_not_affect_attackWeight_in_same_square)
{
	Blob attacker1 = CreateBlob ().position (make_pt (3.9, 4.0)).HP (100U).damage (100U).lifespan (1000U).maxHunger (100U);
	Blob attacker2 = CreateBlob ().position (make_pt (3.1, 4.0)).HP (100U).damage (100U).lifespan (1000U).maxHunger (100U);
	Blob defender = CreateBlob ().position (make_pt (3.0, 4.0)).smell (100.0).HP (50U).lifespan (1000U).maxHunger (100U);

	EXPECT_DOUBLE_EQ (attacker1.attackWeight (defender), attacker2.attackWeight (defender));
}

