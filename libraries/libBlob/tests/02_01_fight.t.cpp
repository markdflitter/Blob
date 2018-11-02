#include <fight.h>
#include <gtest/gtest.h>
#include <memory>
#include <blob.h>

class TargetMock : public Target
{
public:
	TargetMock (unsigned int damage) :
		_damage (damage) 
	{
	}

	void takeDamage (unsigned int damage)
	{
		_damageTaken = damage;
	}

	void inflictDamage (Target* target, const std::string& state)
	{
		_damageInflicted = _damage;
		target->takeDamage (_damage);
	}

	void retaliate (Target* target)
	{
		_damageRetaliated = _damage;
		target->takeDamage (_damage);
	}


	unsigned int _damageTaken = 0;
	unsigned int _damageInflicted = 0;
	unsigned int _damageRetaliated = 0;
	unsigned int _damage = 0;
};

TEST(test_02_01_fight_t, create)
{
	TargetMock tm1 (10U);
	TargetMock tm2 (20U);
	Fight a (&tm1, &tm2, "");
	EXPECT_EQ (a._target, &tm1);
	EXPECT_EQ (a._attacker, &tm2);
}

TEST(test_02_01_fight_t, equal_to_self)
{
	TargetMock tm1 (10U);
	TargetMock tm2 (20U);
	Fight a (&tm1, &tm2, "");
	EXPECT_TRUE (a == a);
}

TEST (test_02_01_fight_t, equal_to_other)
{
	TargetMock tm1 (10U);
	TargetMock tm2 (20U);
	Fight a1 (&tm1, &tm2, "");
	Fight a2 (&tm1, &tm2, "");
	EXPECT_TRUE (a1 == a2);
}

TEST (test_02_01_fight_t, equal_false_different_target)
{
	TargetMock tm1 (10U);
	TargetMock tm2 (20U);
	Fight a1 (&tm1, &tm2, "");
	Fight a2 (&tm2, &tm2, "");
	EXPECT_FALSE (a1 == a2);
}

TEST (test_02_01_fight_t, equal_false_different_attacker)
{
	TargetMock tm1 (10U);
	TargetMock tm2 (20U);
	Fight a1 (&tm1, &tm2, "");
	Fight a2 (&tm1, &tm1, "");
	EXPECT_FALSE (a1 == a2);
}

TEST(test_02_01_fight_t, not_equal_equal_to_self)
{
	TargetMock tm1 (10U);
	TargetMock tm2 (20U);
	Fight a (&tm1, &tm2, "");
	EXPECT_FALSE (a != a);
}

TEST(test_02_01_fight_t, not_equal_different_target)
{
	TargetMock tm1 (10U);
	TargetMock tm2 (20U);
	Fight a1 (&tm1, &tm2, "");
	Fight a2 (&tm2, &tm2, "");
	EXPECT_TRUE (a1 != a2);
}

TEST(test_02_01_fight_t, not_equal_different_attacker)
{
	TargetMock tm1 (10U);
	TargetMock tm2 (20U);
	Fight a1 (&tm1, &tm2, "");
	Fight a2 (&tm1, &tm1, "");
	EXPECT_TRUE (a1 != a2);
}

TEST (test_02_01_fight_t, apply_target)
{
	TargetMock tm1 (10U);
	TargetMock tm2 (20U);
	std::shared_ptr<Action> a (new Fight (&tm1, &tm2, ""));
	
	a->apply ();
	EXPECT_EQ (tm1._damageTaken, 20U);
	EXPECT_EQ (tm1._damageInflicted, 0U);
	EXPECT_EQ (tm1._damageRetaliated, 10U);
	EXPECT_EQ (tm2._damageTaken, 10U);
	EXPECT_EQ (tm2._damageInflicted, 20U);
	EXPECT_EQ (tm2._damageRetaliated, 0U);
}

TEST (test_02_01_fight_t, applies_to_blobs)
{
	Blob target = CreateBlob ().HP (20U).damage (4U).lifespan (1000U).maxHunger (1000U);
	Blob attacker = CreateBlob ().HP (40U).damage (6U).lifespan (1000U).maxHunger (1000U);
	EXPECT_EQ (target.HP (), 10U);
	EXPECT_EQ (attacker.HP (), 20U);

	std::shared_ptr <Action> a = attacker.createActionFight (target);
	a->apply ();
	EXPECT_EQ (target.HP (), 7U);
	EXPECT_EQ (attacker.HP (), 19U);
}

