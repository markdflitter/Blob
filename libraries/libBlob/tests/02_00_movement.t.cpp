#include <movement.h>
#include <gtest/gtest.h>
#include <memory>
#include <blob.h>

class MoveableMock : public Moveable
{
public:
	void move (double speed, double angleInRadians, const std::string& reason)
	{
		_speed = speed;
		_angleInRadians = angleInRadians;
		_reason = reason;	
	}

	double _speed = 0.0;
	double _angleInRadians = 0.0;
	std::string _reason;
};

TEST(test_02_00_movement_t, create)
{
	MoveableMock mm;
	Movement m (&mm, "a", 10.0, 20.0);
	EXPECT_EQ (m._target, &mm);
	EXPECT_EQ (m._reason, "a");
	EXPECT_EQ (m._speed, 10.0);
	EXPECT_EQ (m._angleInRadians, 20);
}

TEST(test_02_00_movement_t, equal_to_self)
{
	MoveableMock mm;
	Movement m (&mm, "a", 10.0, 20.0);
	EXPECT_TRUE (m == m);
}

TEST (test_02_00_movement_t, equal_to_other)
{
	MoveableMock mm;
	Movement m1 (&mm, "a", 10.0, 20.0);
	Movement m2 (&mm, "a", 10.0, 20.0);
	EXPECT_TRUE (m1 == m2);
}

TEST (test_02_00_movement_t, equal_false_different_moveable)
{
	MoveableMock mm1;
	MoveableMock mm2;
	Movement m1 (&mm1, "a", 10.0, 20.0);
	Movement m2 (&mm2, "a", 10.0, 20.0);
	EXPECT_FALSE (m1 == m2);
}

TEST (test_02_00_movement_t, equal_false_different_reason)
{
	MoveableMock mm;
	Movement m1 (&mm, "a", 10.0, 20.0);
	Movement m2 (&mm, "b", 10.0, 20.0);
	EXPECT_FALSE (m1 == m2);
}

TEST (test_02_00_movement_t, equal_false_different_speed)
{
	MoveableMock mm;
	Movement m1 (&mm, "a", 10.0, 20.0);
	Movement m2 (&mm, "a", 15.0, 20.0);
	EXPECT_FALSE (m1 == m2);
}

TEST (test_02_00_movement_t, equal_false_different_angle)
{
	MoveableMock mm;
	Movement m1 (&mm, "a", 10.0, 20.0);
	Movement m2 (&mm, "a", 10.0, 25.0);
	EXPECT_FALSE (m1 == m2);
}

TEST(test_02_00_movement_t, not_equal_equal_to_self)
{
	MoveableMock mm;
	Movement m (&mm, "a", 10.0, 20.0);
	EXPECT_FALSE (m != m);
}

TEST(test_02_00_movement_t, not_equal_different_moveable)
{
	MoveableMock mm1;
	MoveableMock mm2;
	Movement m1 (&mm1, "a", 10.0, 20.0);
	Movement m2 (&mm2, "a", 10.0, 20.0);
	EXPECT_TRUE (m1 != m2);
}

TEST(test_02_00_movement_t, not_equal_different_reason)
{
	MoveableMock mm;
	Movement m1 (&mm, "a", 10.0, 20.0);
	Movement m2 (&mm, "b", 10.0, 20.0);
	EXPECT_TRUE (m1 != m2);
}

TEST(test_02_00_movement_t, not_equal_different_speed)
{
	MoveableMock mm;
	Movement m1 (&mm, "", 10.0, 20.0);
	Movement m2 (&mm, "", 30.0, 20.0);
	EXPECT_TRUE (m1 != m2);
}

TEST(test_02_00_movement_t, not_equal_different_angle)
{
	MoveableMock mm;
	Movement m1 (&mm, "", 10.0, 20.0);
	Movement m2 (&mm, "", 10.0, 30.0);
	EXPECT_TRUE (m1 != m2);
}

TEST (test_02_00_movement_t, output)
{
	MoveableMock mm;
	std::shared_ptr<Action> a (new Movement (&mm, "hello", 10.0, M_PI / 2));
	EXPECT_TRUE (std::dynamic_pointer_cast <Movement> (a));
	std::shared_ptr <Movement> m (std::dynamic_pointer_cast <Movement> (a));
	
	std::stringstream s;

	s << *m;

	EXPECT_EQ (s.str (), "hello,10.00000,90.00000");
}
TEST (test_02_00_movement_t, apply)
{
	MoveableMock mm;
	std::shared_ptr<Action> a (new Movement (&mm, "hello", 10.0, M_PI / 2));
	
	a->apply ();

	EXPECT_EQ (mm._speed, 10.0);
	EXPECT_DOUBLE_EQ (mm._angleInRadians, M_PI / 2.0);
	EXPECT_EQ (mm._reason, "hello");
}

TEST (test_02_00_movement_t, applies_to_blob)
{
	Blob b1 = CreateBlob ().position (make_pt (10.1, 20.2)).HP (100U).maxHunger (100U).lifespan (100U);
	std::shared_ptr<Action> m (new Movement (&b1, "because", 5.0, 0.0));
        m->apply ();
	
	EXPECT_DOUBLE_EQ (b1.x (), 10.1);
	EXPECT_DOUBLE_EQ (b1.y (), 25.2);
	EXPECT_EQ (b1.state (), "because");
}

