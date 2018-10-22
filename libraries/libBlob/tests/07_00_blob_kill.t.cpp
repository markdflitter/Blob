#include <gtest/gtest.h>
#include <blob.h>

TEST(test_07_00_blob_kill_t, is_dead_after_being_killed)
{
	Blob b1 = CreateBlob ().smell (100.0).speed (100.0).runningSpeed (100.0).HP (100U)
			.damage (100U).endurance (100U).lifespan (100U).aggression (1.0).starvationLevel (100U);
	EXPECT_FALSE (b1.isDead ());
	
	b1.kill ();

	EXPECT_TRUE (b1.isDead ());
	EXPECT_EQ (b1.state (), "dead");
	EXPECT_EQ (b1.currentMaxHP (), 0U);
	EXPECT_EQ (b1.greatestMaxHP (), 0U);
	EXPECT_EQ (b1.starvationLevel (), 0U);
	EXPECT_EQ (b1.currentHunger (), 0U);
	EXPECT_DOUBLE_EQ (b1.currentWanderingSpeed (), 0.0);
	EXPECT_DOUBLE_EQ (b1.currentRunningSpeed (), 0.0);
	EXPECT_DOUBLE_EQ (b1.currentSmell (), 0.0);
	EXPECT_EQ (b1.currentDamage (), 0U);
	EXPECT_EQ (b1.endurance (), 0U);
	EXPECT_DOUBLE_EQ (b1.currentAggression (), 0.0);
}

