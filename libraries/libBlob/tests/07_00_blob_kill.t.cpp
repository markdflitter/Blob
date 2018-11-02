#include <gtest/gtest.h>
#include <blob.h>

TEST(test_07_00_blob_kill_t, is_dead_after_being_killed)
{
	Blob b1 = CreateBlob ().smell (100.0).speed (100.0).runningSpeed (100.0).HP (100U)
			.damage (100U).endurance (100U).lifespan (100U).aggression (1.0).maxHunger (100U);
	EXPECT_FALSE (b1.isDead ());
	
	b1.kill ();

	EXPECT_TRUE (b1.isDead ());
	EXPECT_EQ (b1.state (), "dead");
	EXPECT_DOUBLE_EQ (b1.smell (), 0.0);
	EXPECT_DOUBLE_EQ (b1.speed (), 0.0);
	EXPECT_DOUBLE_EQ (b1.runningSpeed (), 0.0);
	EXPECT_DOUBLE_EQ (b1.aggression (), 0.0);
	EXPECT_EQ (b1.maxHP (), 0U);
	EXPECT_EQ (b1.baseHP (), 0U);
	EXPECT_EQ (b1.damage (), 0U);
	EXPECT_EQ (b1.endurance (), 0U);
	EXPECT_EQ (b1.maxHunger (), 0U);
	EXPECT_EQ (b1.hunger (), 0U);

}

