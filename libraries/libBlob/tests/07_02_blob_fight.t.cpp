#include <gtest/gtest.h>
#include <blob.h>

TEST (test_07_02_blob_fight_t, takesDamage)
{
	Blob b = CreateBlob ().HP (100U).lifespan (1000U).maxHunger (100U);
	EXPECT_EQ (b.HP (), 50U);
	EXPECT_FALSE (b.isDead ());

	b.takeDamage (1U);
	EXPECT_EQ (b.HP (), 49U);
	EXPECT_FALSE (b.isDead ());
}

TEST (test_07_02_blob_fight_t, inflictsDamage)
{
	Blob target = CreateBlob ().HP (100U).lifespan (1000U).maxHunger (100U);
	Blob attacker = CreateBlob ().HP (100U).lifespan (1000U).maxHunger (100U).damage (10U);
	EXPECT_EQ (target.HP (), 50U);
	EXPECT_FALSE (attacker.isDead ());

	attacker.inflictDamage (&target, "");
	EXPECT_EQ (target.HP (), 45U);
	EXPECT_FALSE (target.isDead ());
}

TEST (test_07_02_blob_fight_t, retaliates)
{
	Blob target = CreateBlob ().HP (100U).lifespan (1000U).maxHunger (100U).damage (10U);
	Blob attacker = CreateBlob ().HP (100U).lifespan (1000U).maxHunger (100U).damage (10U);
	EXPECT_EQ (target.HP (), 50U);
	EXPECT_FALSE (attacker.isDead ());

	target.retaliate (&attacker);
	EXPECT_EQ (attacker.HP (), 45U);
}

TEST (test_07_02_blob_fight_t, does_not_retaliate_if_dead)
{
	Blob target = CreateBlob ().HP (100U).lifespan (1000U).maxHunger (10000U).damage (10000U);
	Blob attacker = CreateBlob ().HP (100U).lifespan (1000U).maxHunger (10000U).damage (100U);
	EXPECT_EQ (target.HP (), 50U);
	EXPECT_FALSE (attacker.isDead ());

	attacker.inflictDamage (&target, "");
	EXPECT_TRUE (target.isDead ());
	target.retaliate (&attacker);
	EXPECT_EQ (attacker.HP (), 50U);
}

TEST (test_07_02_blob_fight_t, kill)
{
	Blob b = CreateBlob ().HP (100U).lifespan (10U).maxHunger (100U);
	EXPECT_EQ (b.HP (), 50U);
	EXPECT_FALSE (b.isDead ());

	b.takeDamage (100U);
	EXPECT_EQ (b.HP (), 0U);
	EXPECT_TRUE (b.isDead ());
}

TEST (test_07_02_blob_fight_t, overkill)
{
	Blob b = CreateBlob ().HP (100U).lifespan (10U).maxHunger (100U);
	EXPECT_EQ (b.HP (), 50U);
	EXPECT_FALSE (b.isDead ());

	b.takeDamage (1000U);
	EXPECT_EQ (b.HP (), 0U);
	EXPECT_TRUE (b.isDead ());
}

