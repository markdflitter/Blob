#include <gtest/gtest.h>
#include <blob.h>

TEST (test_07_03_blob_attack_t, hunts_if_not_in_same_square)
{
	Blob attacker = CreateBlob ().position (make_pt (10.0, 10.0));
	Blob target = CreateBlob ().position (make_pt (20.0, 20.0));

	std::shared_ptr<Action> a = attacker.createActionAttack (target);
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (a));
	std::shared_ptr <Movement> m (std::dynamic_pointer_cast <Movement> (a));

	EXPECT_EQ (m->_reason, "hunting  (fast)");
}

TEST (test_07_03_blob_attack_t, fights_if_in_same_square)
{
	Blob attacker = CreateBlob ().position (make_pt (10.0, 10.0));
	Blob target = CreateBlob ().position (make_pt (10.0, 10.0));

	std::shared_ptr<Action> a = attacker.createActionAttack (target);
	ASSERT_TRUE (std::dynamic_pointer_cast <Fight> (a));
	std::shared_ptr <Fight> m (std::dynamic_pointer_cast <Fight> (a));
}

