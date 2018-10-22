#include <gtest/gtest.h>
#include <blob.h>
#include <blobImpl.h>

TEST (test_08_00_blob_age_t, grows_older)
{
	Blob b1 = CreateBlob ().HP (100U).lifespan (100U).starvationLevel (100U);
	EXPECT_EQ (b1.currentAge (), 0U);
	
	b1.growOlder ();
	EXPECT_EQ (b1.currentAge (), 1U);
}

TEST (test_08_00_blob_age_t, dies_of_old_age)
{
	Blob b1 = CreateBlob ().lifespan (2U).HP (100U).starvationLevel (100U);
	
	EXPECT_EQ (b1.currentAge (), 0U);
	
	b1.growOlder ();
	EXPECT_EQ (b1.currentAge (), 1U);
	
	b1.growOlder ();
	EXPECT_EQ (b1.currentAge (), 2U);
	EXPECT_TRUE (b1.isDead ());
}

TEST (test_08_00_blob_age_t, gets_older_when_moving)
{
	Blob b1 = CreateBlob ().HP (100U).lifespan (100U).starvationLevel (100U);
	EXPECT_EQ (b1.currentAge (), 0U);
	
	b1.move (0.0, 0.0, "");
	EXPECT_EQ (b1.currentAge (), 1U);
}

TEST (test_08_00_blob_age_t, gets_older_when_inflicting_damage)
{
	Blob target = CreateBlob ().HP (100U).lifespan (100U).starvationLevel (100U);
	Blob attacker = CreateBlob ().HP (100U).lifespan (100U).starvationLevel (100U);
	EXPECT_EQ (attacker.currentAge (), 0U);
	
	attacker.inflictDamage (&target, "");
	EXPECT_EQ (attacker.currentAge (), 1U);
}

TEST (test_08_00_blob_age_t, does_not_age_when_taking_damage)
{
	Blob target = CreateBlob ().HP (100U).lifespan (100U).starvationLevel (100U);
	EXPECT_EQ (target.currentAge (), 0U);
	
	target.takeDamage (10U);
	EXPECT_EQ (target.currentAge (), 0U);
}

TEST (test_08_00_blob_age_t, does_not_age_when_retaliating)
{
	Blob target = CreateBlob ().HP (100U).lifespan (100U).starvationLevel (100U);
	Blob attacker = CreateBlob ().HP (100U).lifespan (100U).starvationLevel (100U);
	EXPECT_EQ (target.currentAge (), 0U);
	EXPECT_EQ (attacker.currentAge (), 0U);
	
	target.retaliate (&attacker);
	EXPECT_EQ (target.currentAge (), 0U);
	EXPECT_EQ (attacker.currentAge (), 0U);
}

TEST (test_08_00_blob_age_t, ageRatio_when_born)
{
	Blob b1 = CreateBlob ().lifespan (5U).HP (100U).starvationLevel (100U);
	EXPECT_DOUBLE_EQ (b1.getImpl ()->propertyScalingFactorDueToAge (), 0.5);
}

TEST (test_08_00_blob_age_t, ageRatio_when_dying)
{
	Blob b1 = CreateBlob ().lifespan (5U).HP (100U).starvationLevel (100U);

	b1.growOlder ();
	b1.growOlder ();
	b1.growOlder ();
	b1.growOlder ();
	b1.growOlder ();
	EXPECT_DOUBLE_EQ (b1.getImpl ()->propertyScalingFactorDueToAge (), 0.5);
}

TEST (test_08_00_blob_age_t, ageRatio_halfway)
{
	Blob b1 = CreateBlob ().lifespan (10U).HP (100U).starvationLevel (100U);

	b1.growOlder ();
	b1.growOlder ();
	b1.growOlder ();
	b1.growOlder ();
	b1.growOlder ();
	EXPECT_DOUBLE_EQ (b1.getImpl ()->propertyScalingFactorDueToAge (), 1.0);
}
	
TEST (test_08_00_blob_age_t, blobs_greatestMaxHP_do_not_reduce_with_age)
{
	Blob b1 = CreateBlob ().lifespan (5U).HP (100U).starvationLevel (100U);
	EXPECT_DOUBLE_EQ (b1.greatestMaxHP (), 100.0);

	b1.growOlder ();
	EXPECT_DOUBLE_EQ (b1.greatestMaxHP (), 100.0);
}

TEST (test_08_00_blob_age_t, blobs_maxHP_change_with_age)
{
	Blob b1 = CreateBlob ().lifespan (10U).HP (100U).starvationLevel (100U);
	EXPECT_EQ (b1.currentMaxHP (), 50U);

	unsigned int previous = b1.currentHP ();
	for (size_t i = 0; i < 10; i++)
	{
		b1.growOlder ();
		if (i < 5)
		{
			EXPECT_GT (b1.currentMaxHP (), previous);
		}
		else
		{
			EXPECT_LT (b1.currentMaxHP (), previous);
		}
		previous = b1.currentMaxHP ();
	}
}

TEST (test_08_00_blob_age_t, blobs_HP_change_with_age)
{
	Blob b1 = CreateBlob ().lifespan (10U).HP (100U).starvationLevel (100U);
	EXPECT_EQ (b1.currentHP (), 50U);

	unsigned int previous = b1.currentHP ();
	for (size_t i = 0; i < 10; i++)
	{
		b1.growOlder ();
		if (i < 5)
		{
			EXPECT_GT (b1.currentHP (), previous);
		}
		else
		{
			EXPECT_LT (b1.currentHP (), previous);
		}
		previous = b1.currentHP ();
	}
}

TEST (test_08_00_blob_age_t, damaged_blobs_stay_damaged_with_age)
{
	Blob b1 = CreateBlob ().lifespan (10U).HP (100U).starvationLevel (100U);
	EXPECT_EQ (b1.currentHP (), 50U);
	b1.getImpl ()->setHP (40U);

	b1.growOlder ();

	EXPECT_EQ (b1.currentHP (), 59U);
}


TEST (test_08_00_blob_age_t, blobs_get_slower_with_less_HP)
{
	Blob b1 = CreateBlob ().lifespan (5U).speed (100.0).HP (100U).starvationLevel (100U);
	EXPECT_DOUBLE_EQ (b1.currentWanderingSpeed (), 50.0);

	b1.getImpl ()->setHP (40);

	EXPECT_DOUBLE_EQ (b1.greatestWanderingSpeed (), 100.0);
	EXPECT_DOUBLE_EQ (b1.currentWanderingSpeed (), 40.0);
}

TEST (test_08_00_blob_age_t, blobs_change_speed_with_age)
{
	Blob b1 = CreateBlob ().speed (100.0).HP (100U).lifespan (10U).starvationLevel (100U);
	EXPECT_DOUBLE_EQ (b1.currentWanderingSpeed (), 50.0);

	double previous = b1.currentWanderingSpeed ();
	for (size_t i = 0; i < 10; i++)
	{
		b1.growOlder ();
		if (i < 5)
		{
			EXPECT_GT (b1.currentWanderingSpeed (), previous);
		}
		else
		{
			EXPECT_LT (b1.currentWanderingSpeed (), previous);
		}
		previous = b1.currentWanderingSpeed ();
		EXPECT_DOUBLE_EQ (b1.greatestWanderingSpeed (), (i == 9) ? 0.0 : 100.0);
	}
}

TEST (test_08_00_blob_age_t, blobs_run_slower_with_less_HP)
{
	Blob b1 = CreateBlob ().lifespan (5U).starvationLevel (100U).runningSpeed (100.0).HP (100U);
	EXPECT_DOUBLE_EQ (b1.currentRunningSpeed (), 50.0);

	b1.getImpl ()->setHP (40);

	EXPECT_DOUBLE_EQ (b1.currentRunningSpeed (), 40.0);
}

TEST (test_08_00_blob_age_t, blobs_change_runningSpeed_with_age)
{
	Blob b1 = CreateBlob ().runningSpeed (100.0).HP (100U).lifespan (10U).starvationLevel (100U);
	EXPECT_DOUBLE_EQ (b1.currentRunningSpeed (), 50.0);

	double previous = b1.currentRunningSpeed ();
	for (size_t i = 0; i < 10; i++)
	{
		b1.growOlder ();
		if (i < 5)
		{
			EXPECT_GT (b1.currentRunningSpeed (), previous);
		}
		else
		{
			EXPECT_LT (b1.currentRunningSpeed (), previous);
		}
		previous = b1.currentRunningSpeed ();
	}
}

TEST (test_08_00_blob_age_t, blobs_change_smell_with_age)
{
	Blob b1 = CreateBlob ().smell (100.0).HP (100U).lifespan (10U).starvationLevel (100U);
	EXPECT_DOUBLE_EQ (b1.currentSmell (), 50.0);

	double previous = b1.currentSmell ();
	for (size_t i = 0; i < 10; i++)
	{
		b1.growOlder ();
		if (i < 5)
		{
			EXPECT_GT (b1.currentSmell (), previous);
		}
		else
		{
			EXPECT_LT (b1.currentSmell (), previous);
		}
		previous = b1.currentSmell ();
	}
}

TEST (test_08_00_blob_age_t, damage_reduces_with_less_HP)
{
	Blob b1 = CreateBlob ().lifespan (5U).starvationLevel (100U).damage (100.0).HP (100U);
	EXPECT_EQ (b1.greatestDamage (), 100U);
	EXPECT_EQ (b1.currentDamage (), 50U);

	b1.getImpl ()->setHP (40U);
	EXPECT_EQ (b1.greatestDamage (), 100U);
	EXPECT_EQ (b1.currentDamage (), 40U);
}

TEST (test_08_00_blob_age_t, blobs_change_damage_with_age)
{
	Blob b1 = CreateBlob ().damage (100U).HP (100U).lifespan (10U).starvationLevel (100U);
	EXPECT_DOUBLE_EQ (b1.currentDamage (), 50U);

	unsigned int previous = b1.currentDamage ();
	for (size_t i = 0; i < 10; i++)
	{
		b1.growOlder ();
		if (i < 5)
		{
			EXPECT_GT (b1.currentDamage (), previous);
		}
		else
		{
			EXPECT_LT (b1.currentDamage (), previous);
		}
		previous = b1.currentDamage ();
	}
}

TEST (test_08_00_blob_age_t, blobs_that_start_dead_do_not_age)
{
	Blob b1 = CreateBlob ().lifespan (5U).damage (100.0).starvationLevel (100U);
	EXPECT_TRUE (b1.isDead ());
	EXPECT_EQ (b1.currentAge (), 0U);
	
	b1.growOlder ();
	EXPECT_EQ (b1.currentAge (), 0U);
}

TEST (test_08_00_blob_age_t, blobs_age_1_year_when_attacked)
{
	std::vector <Blob> blobs {CreateBlob ().HP (100U).damage (10U).lifespan (5U).starvationLevel (100U),
			    	  CreateBlob ().HP (1000U).damage (50U).lifespan (5U).starvationLevel (100U)};
	blobs[0].chooseNextAction (blobs)->apply ();
	blobs[1].chooseNextAction (blobs)->apply ();
	
	EXPECT_EQ (blobs[0].currentAge (), 1U);
	EXPECT_EQ (blobs[1].currentAge (), 1U);
}

