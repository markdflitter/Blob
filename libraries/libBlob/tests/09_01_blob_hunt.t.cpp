#include <gtest/gtest.h>
#include <blob.h>

TEST (test_09_01_blob_hunt_t, north)
{
	Blob hunter = CreateBlob ().lifespan (100U).HP (100U).starvationLevel (100U).position (make_pt (5.0, 5.0)).runningSpeed (4.0);
	Blob huntee = CreateBlob ().lifespan (100U).HP (100U).starvationLevel (100U).position (make_pt (5.0, 10.0));

	std::shared_ptr<Action> a = hunter.createActionHunt (huntee);
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (a));
	std::shared_ptr <Movement> m (std::dynamic_pointer_cast <Movement> (a));
	EXPECT_EQ (*m, Movement (&hunter, "hunting  (fast)", 2.0, 0));

	a->apply ();

	Pt <double> p = hunter.history ().back ();
	EXPECT_DOUBLE_EQ (p.y (), 7.0);
}

TEST (test_09_01_blob_hunt_t, south)
{
	Blob hunter = CreateBlob ().lifespan (100U).HP (100U).starvationLevel (100U).position (make_pt (5.0, 5.0)).runningSpeed (4.0);
	Blob huntee = CreateBlob ().lifespan (100U).HP (100U).starvationLevel (100U).position (make_pt (5.0, -10.0));

	std::shared_ptr<Action> a = hunter.createActionHunt (huntee);
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (a));
	std::shared_ptr <Movement> m (std::dynamic_pointer_cast <Movement> (a));
	EXPECT_EQ (*m, Movement (&hunter, "hunting  (fast)", 2.0, M_PI));

	a->apply ();
	
	Pt <double> p = hunter.history ().back ();
	EXPECT_DOUBLE_EQ (p.y (), 3.0);
}

TEST (test_09_01_blob_hunt_t, east)
{
	Blob hunter = CreateBlob ().lifespan (100U).HP (100U).starvationLevel (100U).position (make_pt (5.0, 5.0)).runningSpeed (4.0);
	Blob huntee = CreateBlob ().lifespan (100U).HP (100U).starvationLevel (100U).position (make_pt (10.0, 5.0));

	std::shared_ptr<Action> a = hunter.createActionHunt (huntee);
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (a));
	std::shared_ptr <Movement> m (std::dynamic_pointer_cast <Movement> (a));
	EXPECT_EQ (*m, Movement (&hunter, "hunting  (fast)", 2.0, M_PI/2));

	a->apply ();
	
	Pt <double> p = hunter.history ().back ();
	EXPECT_DOUBLE_EQ (p.x (), 7.0);
}

TEST (test_09_01_blob_hunt_t, west)
{
	Blob hunter = CreateBlob ().lifespan (100U).HP (100U).starvationLevel (100U).position (make_pt (-5.0, 5.0)).runningSpeed (4.0);
	Blob huntee = CreateBlob ().lifespan (100U).HP (100U).starvationLevel (100U).position (make_pt (-10.0, 5.0));

	std::shared_ptr<Action> a = hunter.createActionHunt (huntee);
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (a));
	std::shared_ptr <Movement> m (std::dynamic_pointer_cast <Movement> (a));
	EXPECT_EQ (*m, Movement (&hunter, "hunting  (fast)", 2.0, 3 * M_PI / 2));

	a->apply ();
	
	Pt <double> p = hunter.history ().back ();
	EXPECT_DOUBLE_EQ (p.x (), -7.0);
}

TEST (test_09_01_blob_hunt_t, gets_closer)
{
	Blob hunter = CreateBlob ().lifespan (100U).HP (100U).starvationLevel (100U).position (make_pt (-5.0, 5.0)).runningSpeed (7.0);
	Blob huntee = CreateBlob ().lifespan (100U).HP (100U).starvationLevel (100U).position (make_pt (-10.0, 10.0));

	double d1 = hunter.distance (huntee);
	std::shared_ptr<Action> m = hunter.createActionHunt (huntee);
	m->apply ();

	double d2 = hunter.distance (huntee);
	EXPECT_LT (d2, d1);
}

TEST (test_09_01_blob_hunt_t, catches)
{
	Blob hunter = CreateBlob ().lifespan (100U).HP (100U).starvationLevel (100U).position (make_pt (-5.0, 5.0)).runningSpeed (14.0);
	Blob huntee = CreateBlob ().lifespan (100U).HP (100U).starvationLevel (100U).position (make_pt (-10.0, 5.0));

	std::shared_ptr<Action> a = hunter.createActionHunt (huntee);
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (a));
	std::shared_ptr<Movement> m = std::dynamic_pointer_cast <Movement> (a);
	EXPECT_LT (m->_speed, 7.0);
	a->apply ();

	EXPECT_TRUE (hunter.isInSameSquare (huntee));
}

