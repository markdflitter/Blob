#include <gtest/gtest.h>
#include <blob.h>
// this tests the mapping from possibility to real action

namespace
{
	auto weaker = CreateBlob ().HP (50U).smell (100.0).position (make_pt (5.0, 5.0)).lifespan (1000u).maxHunger (100U);
	auto stronger = CreateBlob ().HP (200U).smell (100.0).damage (100U).position (make_pt (10.0, 10.0)).lifespan (1000U).maxHunger (100U);
}

TEST (test_11_06_blob_chooseNextAction_t, fight)
{
	std::vector <Blob> b {weaker, stronger.position (make_pt (5.5, 5.5))};
	auto action = b[1].chooseNextAction (b);
	ASSERT_TRUE (std::dynamic_pointer_cast <Fight> (action));
	std::shared_ptr <Fight> f (std::dynamic_pointer_cast <Fight> (action));

	EXPECT_EQ (f->_target, &b[0]);
	EXPECT_EQ (f->_attacker, &b[1]);
}

TEST (test_11_06_blob_chooseNextAction_t, hunt)
{
	std::vector <Blob> b {weaker, stronger};
	auto action = b[1].chooseNextAction (b);
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (action));
	std::shared_ptr <Movement> m (std::dynamic_pointer_cast <Movement> (action));

	EXPECT_EQ (m->_reason, "hunting  (fast)");
}

TEST (test_11_06_blob_chooseNextAction_t, flee_same_square)
{
	std::vector <Blob> b {weaker, stronger.position (make_pt (5.5, 5.5))};
	auto action = b[0].chooseNextAction (b);
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (action));
	std::shared_ptr <Movement> m (std::dynamic_pointer_cast <Movement> (action));

	EXPECT_EQ (m->_reason, "running from  (fast)");
}

TEST (test_11_06_blob_chooseNextAction_t, flee_in_range)
{
	std::vector <Blob> b {weaker, stronger};
	auto action = b[0].chooseNextAction (b);
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (action));
	std::shared_ptr <Movement> m (std::dynamic_pointer_cast <Movement> (action));

	EXPECT_EQ (m->_reason, "running from  (fast)");
}

TEST (test_11_06_blob_chooseNextAction_t, wander)
{
	std::vector <Blob> b {weaker, stronger.position (make_pt (500.0, 500.0))};
	auto action = b[0].chooseNextAction (b);
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (action));
	std::shared_ptr <Movement> m (std::dynamic_pointer_cast <Movement> (action));

	EXPECT_EQ (m->_reason, "wandering");
}

TEST (test_11_06_blob_chooseNextAction_t, dead_blobs_do_nothing)
{
	std::vector<Blob> blobs {CreateBlob ().speed (1.0).HP (100U).lifespan (100U).maxHunger (100U)};

	std::shared_ptr<Action> a1 = blobs[0].chooseNextAction (blobs);
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (a1));
	std::shared_ptr <Movement> m1 (std::dynamic_pointer_cast <Movement> (a1));
	EXPECT_EQ (*m1, Movement (&blobs[0], "wandering", 1.0, 0.0));

	blobs[0].kill ();
	
	std::shared_ptr<Action> a2 = blobs[0].chooseNextAction (blobs);
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (a2));
	std::shared_ptr <Movement> m2 (std::dynamic_pointer_cast <Movement> (a2));
	EXPECT_EQ (*m2, Movement (&blobs[0], "dead", 0.0, 0.0));
}

TEST (test_11_06_blob_chooseNextAction_t, dead_blobs_do_not_age)
{
	std::vector<Blob> blobs {CreateBlob ().lifespan (2U)};
	EXPECT_EQ (blobs[0].age (), 0U);
	blobs[0].kill ();	
	std::shared_ptr <Action> a = blobs[0].chooseNextAction (blobs);
	a->apply ();
	EXPECT_EQ (blobs[0].age (), 0U);
}

