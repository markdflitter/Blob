#include <gtest/gtest.h>
#include <blob.h>

TEST (test_09_01_blob_hunt_t, north)
{
	Blob hunter = CreateBlob ().lifespan (100U).HP (100U).maxHunger (100U).position (make_pt (5.0, 5.0)).runningSpeed (4.0);
	Blob huntee = CreateBlob ().lifespan (100U).HP (100U).maxHunger (100U).position (make_pt (5.0, 10.0));

	std::shared_ptr<Action> a = hunter.createActionHunt (huntee);
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (a));
	std::shared_ptr <Movement> m (std::dynamic_pointer_cast <Movement> (a));
	EXPECT_EQ (*m, Movement (&hunter, "hunting  (fast)", 4, 0));

	a->apply ();
	EXPECT_DOUBLE_EQ (hunter.y (), 9.0);
}

TEST (test_09_01_blob_hunt_t, south)
{
	Blob hunter = CreateBlob ().lifespan (100U).HP (100U).maxHunger (100U).position (make_pt (5.0, 5.0)).runningSpeed (4.0);
	Blob huntee = CreateBlob ().lifespan (100U).HP (100U).maxHunger (100U).position (make_pt (5.0, -10.0));

	std::shared_ptr<Action> a = hunter.createActionHunt (huntee);
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (a));
	std::shared_ptr <Movement> m (std::dynamic_pointer_cast <Movement> (a));
	EXPECT_EQ (*m, Movement (&hunter, "hunting  (fast)", 4, M_PI));

	a->apply ();
	EXPECT_DOUBLE_EQ (hunter.y (), 1.0);
}

TEST (test_09_01_blob_hunt_t, east)
{
	Blob hunter = CreateBlob ().lifespan (100U).HP (100U).maxHunger (100U).position (make_pt (5.0, 5.0)).runningSpeed (4.0);
	Blob huntee = CreateBlob ().lifespan (100U).HP (100U).maxHunger (100U).position (make_pt (10.0, 5.0));

	std::shared_ptr<Action> a = hunter.createActionHunt (huntee);
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (a));
	std::shared_ptr <Movement> m (std::dynamic_pointer_cast <Movement> (a));
	EXPECT_EQ (*m, Movement (&hunter, "hunting  (fast)", 4, M_PI/2));

	a->apply ();
	EXPECT_DOUBLE_EQ (hunter.x (), 9.0);
}

TEST (test_09_01_blob_hunt_t, west)
{
	Blob hunter = CreateBlob ().lifespan (100U).HP (100U).maxHunger (100U).position (make_pt (-5.0, 5.0)).runningSpeed (4.0);
	Blob huntee = CreateBlob ().lifespan (100U).HP (100U).maxHunger (100U).position (make_pt (-10.0, 5.0));

	std::shared_ptr<Action> a = hunter.createActionHunt (huntee);
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (a));
	std::shared_ptr <Movement> m (std::dynamic_pointer_cast <Movement> (a));
	EXPECT_EQ (*m, Movement (&hunter, "hunting  (fast)", 4, 3 * M_PI / 2));

	a->apply ();
	EXPECT_DOUBLE_EQ (hunter.x (), -9.0);
}

TEST (test_09_01_blob_hunt_t, gets_closer)
{
	Blob hunter = CreateBlob ().lifespan (100U).HP (100U).maxHunger (100U).position (make_pt (-5.0, 5.0)).runningSpeed (7.0);
	Blob huntee = CreateBlob ().lifespan (100U).HP (100U).maxHunger (100U).position (make_pt (-10.0, 10.0));

	double d1 = hunter.distance (huntee);
	std::shared_ptr<Action> m = hunter.createActionHunt (huntee);
	m->apply ();

	double d2 = hunter.distance (huntee);
	EXPECT_LT (d2, d1);
}

TEST (test_09_01_blob_hunt_t, catches)
{
	Blob hunter = CreateBlob ().lifespan (100U).HP (100U).maxHunger (100U).position (make_pt (-5.0, 5.0)).runningSpeed (7.0);
	Blob huntee = CreateBlob ().lifespan (100U).HP (100U).maxHunger (100U).position (make_pt (-10.0, 5.0));

	std::shared_ptr<Action> a = hunter.createActionHunt (huntee);
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (a));
	std::shared_ptr<Movement> m = std::dynamic_pointer_cast <Movement> (a);
	EXPECT_LT (m->_speed, 7.0);
	a->apply ();

	EXPECT_TRUE (hunter.isInSameSquare (huntee));
}

