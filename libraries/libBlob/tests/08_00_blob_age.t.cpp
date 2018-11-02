#include <gtest/gtest.h>
#include <blob.h>

TEST (test_08_00_blob_age_t, grows_older)
{
	Blob b1 = CreateBlob ().HP (100U).lifespan (100U).maxHunger (100U);
	EXPECT_EQ (b1.age (), 0U);
	
	b1.growOlder ();
	EXPECT_EQ (b1.age (), 1U);
}

TEST (test_08_00_blob_age_t, dies_of_old_age)
{
	Blob b1 = CreateBlob ().lifespan (2U).HP (100U).maxHunger (100U);
	
	EXPECT_EQ (b1.age (), 0U);
	
	b1.growOlder ();
	EXPECT_EQ (b1.age (), 1U);
	
	b1.growOlder ();
	EXPECT_EQ (b1.age (), 2U);
	EXPECT_TRUE (b1.isDead ());
}

TEST (test_08_00_blob_age_t, gets_older_when_moving)
{
	Blob b1 = CreateBlob ().HP (100U).lifespan (100U).maxHunger (100U);
	EXPECT_EQ (b1.age (), 0U);
	
	b1.move (0.0, 0.0, "");
	EXPECT_EQ (b1.age (), 1U);
}

TEST (test_08_00_blob_age_t, gets_older_when_inflicting_damage)
{
	Blob target = CreateBlob ().HP (100U).lifespan (100U).maxHunger (100U);
	Blob attacker = CreateBlob ().HP (100U).lifespan (100U).maxHunger (100U);
	EXPECT_EQ (attacker.age (), 0U);
	
	attacker.inflictDamage (&target, "");
	EXPECT_EQ (attacker.age (), 1U);
}

TEST (test_08_00_blob_age_t, does_not_age_when_taking_damage)
{
	Blob target = CreateBlob ().HP (100U).lifespan (100U).maxHunger (100U);
	EXPECT_EQ (target.age (), 0U);
	
	target.takeDamage (10U);
	EXPECT_EQ (target.age (), 0U);
}

TEST (test_08_00_blob_age_t, does_not_age_when_retaliating)
{
	Blob target = CreateBlob ().HP (100U).lifespan (100U).maxHunger (100U);
	Blob attacker = CreateBlob ().HP (100U).lifespan (100U).maxHunger (100U);
	EXPECT_EQ (target.age (), 0U);
	EXPECT_EQ (attacker.age (), 0U);
	
	target.retaliate (&attacker);
	EXPECT_EQ (target.age (), 0U);
	EXPECT_EQ (attacker.age (), 0U);
}

TEST (test_08_00_blob_age_t, ageRatio_when_born)
{
	Blob b1 = CreateBlob ().lifespan (5U).HP (100U).maxHunger (100U);
	EXPECT_DOUBLE_EQ (b1.ageRatio (), 0.5);
}

TEST (test_08_00_blob_age_t, ageRatio_when_dying)
{
	Blob b1 = CreateBlob ().lifespan (5U).HP (100U).maxHunger (100U);

	b1.growOlder ();
	b1.growOlder ();
	b1.growOlder ();
	b1.growOlder ();
	b1.growOlder ();
	EXPECT_DOUBLE_EQ (b1.ageRatio (), 0.5);
}

TEST (test_08_00_blob_age_t, ageRatio_halfway)
{
	Blob b1 = CreateBlob ().lifespan (10U).HP (100U).maxHunger (100U);

	b1.growOlder ();
	b1.growOlder ();
	b1.growOlder ();
	b1.growOlder ();
	b1.growOlder ();
	EXPECT_DOUBLE_EQ (b1.ageRatio (), 1.0);
}
	
TEST (test_08_00_blob_age_t, blobs_baseHP_do_not_reduce_with_age)
{
	Blob b1 = CreateBlob ().lifespan (5U).HP (100U).maxHunger (100U);
	EXPECT_DOUBLE_EQ (b1.baseHP (), 100.0);

	b1.growOlder ();
	EXPECT_DOUBLE_EQ (b1.baseHP (), 100.0);
}

TEST (test_08_00_blob_age_t, blobs_maxHP_change_with_age)
{
	Blob b1 = CreateBlob ().lifespan (10U).HP (100U).maxHunger (100U);
	EXPECT_EQ (b1.maxHP (), 50U);

	unsigned int previous = b1.HP ();
	for (size_t i = 0; i < 10; i++)
	{
		b1.growOlder ();
		if (i < 5)
		{
			EXPECT_GT (b1.maxHP (), previous);
		}
		else
		{
			EXPECT_LT (b1.maxHP (), previous);
		}
		previous = b1.maxHP ();
	}
}

TEST (test_08_00_blob_age_t, blobs_HP_change_with_age)
{
	Blob b1 = CreateBlob ().lifespan (10U).HP (100U).maxHunger (100U);
	EXPECT_EQ (b1.HP (), 50U);

	unsigned int previous = b1.HP ();
	for (size_t i = 0; i < 10; i++)
	{
		b1.growOlder ();
		if (i < 5)
		{
			EXPECT_GT (b1.HP (), previous);
		}
		else
		{
			EXPECT_LT (b1.HP (), previous);
		}
		previous = b1.HP ();
	}
}

TEST (test_08_00_blob_age_t, damaged_blobs_stay_damaged_with_age)
{
	Blob b1 = CreateBlob ().lifespan (10U).HP (100U).maxHunger (100U);
	EXPECT_EQ (b1.HP (), 50U);
	b1.setHP (40U);

	b1.growOlder ();

	EXPECT_EQ (b1.HP (), 59U);
}


TEST (test_08_00_blob_age_t, blobs_get_slower_with_less_HP)
{
	Blob b1 = CreateBlob ().lifespan (5U).speed (100.0).HP (100U).maxHunger (100U);
	EXPECT_DOUBLE_EQ (b1.speed (), 50.0);

	b1.setHP (40);

	EXPECT_DOUBLE_EQ (b1.baseSpeed (), 100.0);
	EXPECT_DOUBLE_EQ (b1.speed (), 40.0);
}

TEST (test_08_00_blob_age_t, blobs_change_speed_with_age)
{
	Blob b1 = CreateBlob ().speed (100.0).HP (100U).lifespan (10U).maxHunger (100U);
	EXPECT_DOUBLE_EQ (b1.speed (), 50.0);

	double previous = b1.speed ();
	for (size_t i = 0; i < 10; i++)
	{
		b1.growOlder ();
		if (i < 5)
		{
			EXPECT_GT (b1.speed (), previous);
		}
		else
		{
			EXPECT_LT (b1.speed (), previous);
		}
		previous = b1.speed ();
		EXPECT_DOUBLE_EQ (b1.baseSpeed (), (i == 9) ? 0.0 : 100.0);
	}

}

TEST (test_08_00_blob_age_t, blobs_run_slower_with_less_HP)
{
	Blob b1 = CreateBlob ().lifespan (5U).maxHunger (100U).runningSpeed (100.0).HP (100U);
	EXPECT_DOUBLE_EQ (b1.runningSpeed (), 50.0);

	b1.setHP (40);

	EXPECT_DOUBLE_EQ (b1.runningSpeed (), 40.0);
}

TEST (test_08_00_blob_age_t, blobs_change_runningSpeed_with_age)
{
	Blob b1 = CreateBlob ().runningSpeed (100.0).HP (100U).lifespan (10U).maxHunger (100U);
	EXPECT_DOUBLE_EQ (b1.runningSpeed (), 50.0);

	double previous = b1.runningSpeed ();
	for (size_t i = 0; i < 10; i++)
	{
		b1.growOlder ();
		if (i < 5)
		{
			EXPECT_GT (b1.runningSpeed (), previous);
		}
		else
		{
			EXPECT_LT (b1.runningSpeed (), previous);
		}
		previous = b1.runningSpeed ();
	}
}

TEST (test_08_00_blob_age_t, blobs_change_smell_with_age)
{
	Blob b1 = CreateBlob ().smell (100.0).HP (100U).lifespan (10U).maxHunger (100U);
	EXPECT_DOUBLE_EQ (b1.smell (), 50.0);

	double previous = b1.smell ();
	for (size_t i = 0; i < 10; i++)
	{
		b1.growOlder ();
		if (i < 5)
		{
			EXPECT_GT (b1.smell (), previous);
		}
		else
		{
			EXPECT_LT (b1.smell (), previous);
		}
		previous = b1.smell ();
	}
}

TEST (test_08_00_blob_age_t, damage_reduces_with_less_HP)
{
	Blob b1 = CreateBlob ().lifespan (5U).maxHunger (100U).damage (100.0).HP (100U);
	EXPECT_EQ (b1.baseDamage (), 100U);
	EXPECT_EQ (b1.damage (), 50U);

	b1.setHP (40U);
	EXPECT_EQ (b1.baseDamage (), 100U);
	EXPECT_EQ (b1.damage (), 40U);
}

TEST (test_08_00_blob_age_t, blobs_change_damage_with_age)
{
	Blob b1 = CreateBlob ().damage (100U).HP (100U).lifespan (10U).maxHunger (100U);
	EXPECT_DOUBLE_EQ (b1.damage (), 50U);

	unsigned int previous = b1.damage ();
	for (size_t i = 0; i < 10; i++)
	{
		b1.growOlder ();
		if (i < 5)
		{
			EXPECT_GT (b1.damage (), previous);
		}
		else
		{
			EXPECT_LT (b1.damage (), previous);
		}
		previous = b1.damage ();
	}
}

TEST (test_08_00_blob_age_t, blobs_that_start_dead_do_not_age)
{
	Blob b1 = CreateBlob ().lifespan (5U).damage (100.0).maxHunger (100U);
	EXPECT_TRUE (b1.isDead ());
	EXPECT_EQ (b1.age (), 0U);
	
	b1.growOlder ();
	EXPECT_EQ (b1.age (), 0U);
}

TEST (test_08_00_blob_age_t, blobs_age_1_year_when_attacked)
{
	std::vector <Blob> blobs {CreateBlob ().HP (100U).damage (10U).lifespan (5U).maxHunger (100U),
			    	  CreateBlob ().HP (1000U).damage (50U).lifespan (5U).maxHunger (100U)};
	blobs[0].chooseNextAction (blobs)->apply ();
	blobs[1].chooseNextAction (blobs)->apply ();
	
	EXPECT_EQ (blobs[0].age (), 1U);
	EXPECT_EQ (blobs[1].age (), 1U);
}

