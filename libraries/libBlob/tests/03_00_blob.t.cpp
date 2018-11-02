#include <gtest/gtest.h>
#include <blob.h>
#include <blobImpl.h>

TEST (test_03_00_blob_t, create_default_blob)
{
	Blob b = CreateBlob ();
}

TEST (test_03_00_blob_t, name)
{
	Blob mark = CreateBlob ().name("mark");
	EXPECT_EQ (mark.name (), "mark");
}

TEST (test_03_00_blob_t, position)
{
	Blob b = CreateBlob ().position (make_pt (100.1, 200.2));
	Pt <double> p = b.history ().back ();

	EXPECT_DOUBLE_EQ (p.x (), 100.1);
	EXPECT_DOUBLE_EQ (p.y (), 200.2);
}

TEST (test_03_00_blob_t, smell)
{
	Blob b = CreateBlob ().HP (100U).smell (300.3).lifespan (100U).starvationLevel (100U);
	EXPECT_DOUBLE_EQ (b.smell (), 150.15);
}

TEST (test_03_00_blob_t, wanderingSpeed)
{
	Blob b = CreateBlob ().speed (300.3).HP (100U).lifespan (100U).starvationLevel (100U);
	EXPECT_DOUBLE_EQ (b.maxWanderingSpeed (), 300.3);
	EXPECT_DOUBLE_EQ (b.currentWanderingSpeed (), 150.15);
}

TEST (test_03_00_blob_t, runningSpeed)
{
	Blob b = CreateBlob ().runningSpeed (400.4).HP (100U).lifespan (100U).starvationLevel (100U);
	EXPECT_DOUBLE_EQ (b.currentRunningSpeed (), 200.2);
}

TEST (test_03_00_blob_t, HP)
{
	Blob b = CreateBlob ().HP (500U).lifespan (100U).starvationLevel (100U);
	EXPECT_EQ (b.greatestMaxHP (), 500U);
	EXPECT_EQ (b.currentMaxHP (), 250U);
	EXPECT_EQ (b.currentHP (), 250U);
}

TEST (test_03_00_blob_t, setHP)
{
	Blob b = CreateBlob ().HP (500U).lifespan (100U).starvationLevel (100U);

	EXPECT_EQ (b.currentHP (), 250U);
	b.getImpl ()->setHP (80U);	
	EXPECT_EQ (b.currentHP (), 80U);
}

TEST (test_03_00_blob_t, setHP_and_die)
{
	Blob b = CreateBlob ().HP (500U).lifespan (100U).starvationLevel (100U);

	EXPECT_EQ (b.currentHP (), 250U);
	b.getImpl ()->setHP (0U);	
	EXPECT_EQ (b.currentHP (), 0U);
	EXPECT_TRUE (b.isDead ());
}

TEST (test_03_00_blob_t, endurance)
{
	Blob b = CreateBlob ().HP (100U).endurance(600U).lifespan (100U).starvationLevel (100U);
	EXPECT_EQ (b.endurance (), 600U);
}

TEST (test_03_00_blob_t, aggression)
{
	Blob b = CreateBlob ().HP (100U).aggression (700.7).lifespan (100U).starvationLevel (100U);
	EXPECT_DOUBLE_EQ (b.aggression (), 700.7);
}

TEST (test_03_00_blob_t, lifespan)
{
	Blob b = CreateBlob().lifespan (800U);
	EXPECT_EQ (b.lifespan (), 800U);
}

TEST (test_03_00_blob_t, currentAge)
{
	Blob b = CreateBlob ();
	EXPECT_EQ (b.currentAge (), 0U);
}

TEST (test_03_00_blob_t, damage)
{
	Blob b = CreateBlob ().damage (100U).lifespan (5U).HP (100U).starvationLevel (100U);
	EXPECT_EQ (b.baseDamage (), 100U);
	EXPECT_EQ (b.damage (), 50U);
}

TEST (test_03_00_blob_t, hunger)
{
	Blob b = CreateBlob ().HP (100U).starvationLevel (100U).lifespan (100U).starvationLevel (100U);
	EXPECT_EQ (b.starvationLevel (), 100U);
	EXPECT_EQ (b.currentHunger (), 0U);
}

TEST (test_03_00_blob_t, state)
{
	Blob b = CreateBlob ().HP (100U).lifespan (100U).starvationLevel (100U);
	EXPECT_EQ (b.state (), "newborn");
}

TEST (test_03_00_blob_t, fatigue)
{
	Blob b = CreateBlob ();
	EXPECT_EQ (b.fatigue (), 0U);
}

TEST (test_03_00_blob_t, starts_untired)
{
	Blob b = CreateBlob ();
	EXPECT_FALSE (b.getImpl ()->_tired);
}

TEST (test_03_00_blob_t, starts_dead)
{
	Blob b = CreateBlob ();
	EXPECT_TRUE (b.isDead ());
}

TEST (test_03_00_blob_t, starts_alive)
{
	Blob b = CreateBlob ().HP (100U).lifespan (100U).starvationLevel (100U);
	EXPECT_FALSE (b.isDead ());
}

TEST (test_03_00_blob_t, limitHPtoMax_keeps_damage)
{
	Blob b = CreateBlob ().HP (100U).lifespan (10U).starvationLevel (100U);
	EXPECT_EQ (b.currentHP (), 50U);

	b.takeDamage (10U);
	EXPECT_EQ (b.currentHP (), 40U);

	b.limitHPtoMax (20U);
	EXPECT_EQ (b.currentHP (), 30U);

	b.growOlder ();
	EXPECT_EQ (b.currentHP (), 49U);
}

