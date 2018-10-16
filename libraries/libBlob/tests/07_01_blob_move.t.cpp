#include <gtest/gtest.h>
#include <blob.h>

TEST (test_07_01_blob_move_t, do_not_move)
{
	Blob b1 = CreateBlob ();
	b1.move (0.0, 0.0, "");
	
	EXPECT_DOUBLE_EQ (b1.x (), 0.0);
	EXPECT_DOUBLE_EQ (b1.y (), 0.0);
}

TEST (test_07_01_blob_move_t, move_north)
{
	Blob b1 = CreateBlob ().position (make_pt (10.1, 15.2)).lifespan (100U).maxHunger (100U).HP (100U);
	b1.move (5.0, 0.0, "move north");

	EXPECT_DOUBLE_EQ (b1.x (), 10.1);
	EXPECT_DOUBLE_EQ (b1.y (), 20.2);
	EXPECT_EQ (b1.state (), "move north");
}

TEST (test_07_01_blob_move_t, move_south)
{
	Blob b1 = CreateBlob ().position (make_pt (10.1, 20.2)).lifespan (100U).maxHunger (100U).HP (100U);
	b1.move (5.0, M_PI, "move south");

	EXPECT_DOUBLE_EQ (b1.x (), 10.1);
	EXPECT_DOUBLE_EQ (b1.y (), 15.2);
	EXPECT_EQ (b1.state (), "move south");
}

TEST (test_07_01_blob_move_t, move_minus_south)
{
	Blob b1 = CreateBlob ().position (make_pt (10.1, 20.2)).lifespan (100U).maxHunger (100U).HP (100U);
	b1.move (5.0, -M_PI, "move south");

	EXPECT_DOUBLE_EQ (b1.x (), 10.1);
	EXPECT_DOUBLE_EQ (b1.y (), 15.2);
	EXPECT_EQ (b1.state (), "move south");
}

TEST (test_07_01_blob_move_t, move_east)
{
	Blob b1 = CreateBlob ().position (make_pt (10.1, 20.2)).lifespan (100U).maxHunger (100U).HP (100U);
	b1.move (5.0, M_PI / 2.0, "move east");

	EXPECT_DOUBLE_EQ (b1.x (), 15.1);
	EXPECT_DOUBLE_EQ (b1.y (), 20.2);
	EXPECT_EQ (b1.state (), "move east");
}

TEST (test_07_01_blob_move_t, move_west)
{
	Blob b1 = CreateBlob ().position (make_pt (10.1, 20.2)).lifespan (100U).maxHunger (100U).HP (100U);
	b1.move (5.0, 3.0 * M_PI / 2.0, "move west");

	EXPECT_DOUBLE_EQ (b1.x (), 5.1);
	EXPECT_DOUBLE_EQ (b1.y (), 20.2);
	EXPECT_EQ (b1.state (), "move west");
}

TEST (test_07_01_blob_move_t, move_minus_west)
{
	Blob b1 = CreateBlob ().position (make_pt (10.1, 20.2)).lifespan (100U).maxHunger (100U).HP (100U);
	b1.move (5.0, -M_PI / 2.0, "move west");

	EXPECT_DOUBLE_EQ (b1.x (), 5.1);
	EXPECT_DOUBLE_EQ (b1.y (), 20.2);
	EXPECT_EQ (b1.state (), "move west");
}

TEST (test_07_01_blob_move_t, move_north_east)
{
	Blob b1 = CreateBlob ().position (make_pt (10.1, 20.2)).lifespan (100U).maxHunger (100U).HP (100U);
	b1.move (5.0, M_PI / 4.0, "move north east");

	EXPECT_DOUBLE_EQ (b1.x (), 13.63553390593273);
	EXPECT_DOUBLE_EQ (b1.y (), 23.735533905932737);
	EXPECT_EQ (b1.state (), "move north east");
}

TEST (test_07_01_blob_move_t, move_north_west)
{
	Blob b1 = CreateBlob ().position (make_pt (10.1, 20.2)).lifespan (100U).maxHunger (100U).HP (100U);
	b1.move (5.0, -M_PI / 4.0, "move north west");

	EXPECT_DOUBLE_EQ (b1.x (), 06.564466094067262);
	EXPECT_DOUBLE_EQ (b1.y (), 23.735533905932737);
	EXPECT_EQ (b1.state (), "move north west");

	Blob b2 = CreateBlob ().position (make_pt (10.1, 20.2)).lifespan (100U).maxHunger (100U).HP (100U);
	b2.move (5.0, 7.0 * M_PI / 4.0, "move north west");
	
	EXPECT_DOUBLE_EQ (b2.x (), 06.56446609406726);
	EXPECT_DOUBLE_EQ (b2.y (), 23.73553390593273);
	EXPECT_EQ (b1.state (), "move north west");
}

TEST (test_07_01_blob_move_t, move_south_east)
{
	Blob b1 = CreateBlob ().position (make_pt (10.1, 20.2)).lifespan (100U).maxHunger (100U).HP (100U);
	b1.move (5.0, 3.0 * M_PI / 4.0, "move south east");

	EXPECT_DOUBLE_EQ (b1.x (), 13.63553390593273);
	EXPECT_DOUBLE_EQ (b1.y (), 16.66446609406726);
	EXPECT_EQ (b1.state (), "move south east");
}

TEST (test_07_01_blob_move_t, move_south_west)
{
	Blob b1 = CreateBlob ().position (make_pt (10.1, 20.2)).lifespan (100U).maxHunger (100U).HP (100U);
	b1.move (5.0, 5.0 * M_PI / 4.0, "move south west");

	EXPECT_DOUBLE_EQ (b1.x (), 06.56446609406726);
	EXPECT_DOUBLE_EQ (b1.y (), 16.66446609406726);
	EXPECT_EQ (b1.state (), "move south west");
}

TEST (test_07_01_blob_move_t, move_minus_south_west)
{
	Blob b1 = CreateBlob ().position (make_pt (10.1, 20.2)).lifespan (100U).maxHunger (100U).HP (100U);
	b1.move (5.0, -3.0 * M_PI / 4.0, "move south west");

	EXPECT_DOUBLE_EQ (b1.x (), 06.56446609406726);
	EXPECT_DOUBLE_EQ (b1.y (), 16.66446609406726);
	EXPECT_EQ (b1.state (), "move south west");
}

TEST (test_07_01_blob_move_t, move_negative_x)
{
	Blob b1 = CreateBlob ().position (make_pt (10.1, 20.2)).lifespan (100U).maxHunger (100U).HP (100U);
	b1.move (20.0, 3.0 * M_PI / 2.0, "-x");

	EXPECT_DOUBLE_EQ (b1.x (), -9.9);
	EXPECT_DOUBLE_EQ (b1.y (), 20.2);
	EXPECT_EQ (b1.state (), "-x");
}

TEST (test_07_01_blob_move_t, move_negative_y)
{
	Blob b1 = CreateBlob ().position (make_pt (10.1, 20.2)).lifespan (100U).maxHunger (100U).HP (100U);
	b1.move (30.0, M_PI, "-y");

	EXPECT_DOUBLE_EQ (b1.x (), 10.1);
	EXPECT_DOUBLE_EQ (b1.y (), -9.8);
	EXPECT_EQ (b1.state (), "-y");
}

TEST (test_07_01_blob_move_t, move_fast)
{
	Blob b1 = CreateBlob ().position (make_pt (10.1, 20.2)).lifespan (100U).maxHunger (100U).HP (100U);
	b1.move (10.0, 0.0, "move fast");

	EXPECT_DOUBLE_EQ (b1.x (), 10.1);
	EXPECT_DOUBLE_EQ (b1.y (), 30.2);
	EXPECT_EQ (b1.state (), "move fast");
}

TEST (test_07_01_blob_move_t, move_limit_minx)
{
	Blob b1 = CreateBlob ().lifespan (100U).maxHunger (100U).HP (100U);
	b1.move (2400.0, 3.0 * M_PI / 2.0, "");

	EXPECT_DOUBLE_EQ (b1.x (), -1500.0);
}

TEST (test_07_01_blob_move_t, move_limit_maxx)
{
	Blob b1 = CreateBlob ().lifespan (100U).maxHunger (100U).HP (100U);
	b1.move (2400.0, M_PI / 2.0, "");

	EXPECT_DOUBLE_EQ (b1.x (), 1500.0);
}

TEST (test_07_01_blob_move_t, move_limit_miny)
{
	Blob b1 = CreateBlob ().lifespan (100U).maxHunger (100U).HP (100U);
	b1.move (1200.0, M_PI, "");

	EXPECT_DOUBLE_EQ (b1.y (), -750.0);
}

TEST (test_07_01_blob_move_t, move_limit_maxy)
{
	Blob b1 = CreateBlob ().lifespan (100U).maxHunger (100U).HP (100U);
	b1.move (1200.0, 0.0, "");

	EXPECT_DOUBLE_EQ (b1.y (), 750.0);
}

TEST (test_07_01_blob_move_t, move_hitPoints_recover)
{
	Blob b1 = CreateBlob ().HP (100U).lifespan (100000U).maxHunger (100U).HP (100U);
	b1.takeDamage (5U);

	EXPECT_EQ (b1.HP (), 45U);
	b1.growOlder ();
	EXPECT_EQ (b1.HP (), 46U);
}

TEST (test_07_01_blob_move_t, move_hitPoints_recover_stops_at_max)
{
	Blob b1 = CreateBlob ().HP (100U).lifespan (10000U).maxHunger (100U).HP (100U);
	b1.takeDamage (5U);
	EXPECT_EQ (b1.HP (), 45U);
	b1.growOlder ();
	b1.growOlder ();
	b1.growOlder ();
	b1.growOlder ();
	b1.growOlder ();
	b1.growOlder ();
	EXPECT_EQ (b1.HP (), 50U);
}

