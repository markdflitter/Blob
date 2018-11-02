#include <gtest/gtest.h>
#include <blob.h>

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
	EXPECT_DOUBLE_EQ (b.x (), 100.1);
	EXPECT_DOUBLE_EQ (b.y (), 200.2);
}

TEST (test_03_00_blob_t, smell)
{
	Blob b = CreateBlob ().HP (100U).smell (300.3).lifespan (100U).maxHunger (100U);
	EXPECT_DOUBLE_EQ (b.smell (), 150.15);
}

TEST (test_03_00_blob_t, speed)
{
	Blob b = CreateBlob ().speed (300.3).HP (100U).lifespan (100U).maxHunger (100U);
	EXPECT_DOUBLE_EQ (b.baseSpeed (), 300.3);
	EXPECT_DOUBLE_EQ (b.speed (), 150.15);
}

TEST (test_03_00_blob_t, running_speed)
{
	Blob b = CreateBlob ().runningSpeed (400.4).HP (100U).lifespan (100U).maxHunger (100U);
	EXPECT_DOUBLE_EQ (b.runningSpeed (), 200.2);
}

TEST (test_03_00_blob_t, HP)
{
	Blob b = CreateBlob ().HP (500U).lifespan (100U).maxHunger (100U);
	EXPECT_EQ (b.baseHP (), 500U);
	EXPECT_EQ (b.maxHP (), 250U);
	EXPECT_EQ (b.HP (), 250U);
}

TEST (test_03_00_blob_t, setHP)
{
	Blob b = CreateBlob ().HP (500U).lifespan (100U).maxHunger (100U);

	EXPECT_EQ (b.HP (), 250U);
	b.setHP (80U);	
	EXPECT_EQ (b.HP (), 80U);
}

TEST (test_03_00_blob_t, setHP_and_die)
{
	Blob b = CreateBlob ().HP (500U).lifespan (100U).maxHunger (100U);

	EXPECT_EQ (b.HP (), 250U);
	b.setHP (0U);	
	EXPECT_EQ (b.HP (), 0U);
	EXPECT_TRUE (b.isDead ());
}

TEST (test_03_00_blob_t, endurance)
{
	Blob b = CreateBlob ().HP (100U).endurance(600U).lifespan (100U).maxHunger (100U);
	EXPECT_EQ (b.endurance (), 600U);
}

TEST (test_03_00_blob_t, aggression)
{
	Blob b = CreateBlob ().HP (100U).aggression (700.7).lifespan (100U).maxHunger (100U);
	EXPECT_DOUBLE_EQ (b.aggression (), 700.7);
}

TEST (test_03_00_blob_t, lifespan)
{
	Blob b = CreateBlob().lifespan (800U);
	EXPECT_EQ (b.lifespan (), 800U);
}

TEST (test_03_00_blob_t, age)
{
	Blob b = CreateBlob ();
	EXPECT_EQ (b.age (), 0U);
}

TEST (test_03_00_blob_t, damage)
{
	Blob b = CreateBlob ().damage (100U).lifespan (5U).HP (100U).maxHunger (100U);
	EXPECT_EQ (b.baseDamage (), 100U);
	EXPECT_EQ (b.damage (), 50U);
}

TEST (test_03_00_blob_t, hunger)
{
	Blob b = CreateBlob ().HP (100U).maxHunger (100U).lifespan (100U).maxHunger (100U);
	EXPECT_EQ (b.maxHunger (), 100U);
	EXPECT_EQ (b.hunger (), 0U);
}

TEST (test_03_00_blob_t, state)
{
	Blob b = CreateBlob ().HP (100U).lifespan (100U).maxHunger (100U);
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
	EXPECT_FALSE (b.isTired ());
}

TEST (test_03_00_blob_t, starts_dead)
{
	Blob b = CreateBlob ();
	EXPECT_TRUE (b.isDead ());
}

TEST (test_03_00_blob_t, starts_alive)
{
	Blob b = CreateBlob ().HP (100U).lifespan (100U).maxHunger (100U);
	EXPECT_FALSE (b.isDead ());
}


TEST (test_03_00_blob_t, output)
{
	Blob b = CreateBlob ().position (make_pt (20.2, 40.4));
	
	std::stringstream s;

	s << b;

	EXPECT_EQ (s.str (), "20.2,40.4");
}

TEST (test_03_00_blob_t, limitHPtoMax_keeps_damage)
{
	Blob b = CreateBlob ().HP (100U).lifespan (10U).maxHunger (100U);
	EXPECT_EQ (b.HP (), 50U);

	b.takeDamage (10U);
	EXPECT_EQ (b.HP (), 40U);

	b.limitHPtoMax (20U);
	EXPECT_EQ (b.HP (), 30U);

	b.growOlder ();
	EXPECT_EQ (b.HP (), 49U);
}

