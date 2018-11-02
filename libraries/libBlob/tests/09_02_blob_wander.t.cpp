#include <gtest/gtest.h>
#include <blob.h>

namespace
{
	double fixed_angle (double previousMoveDirection)
	{
		return previousMoveDirection + M_PI / 4.0;
	};
}

TEST (test_09_02_blob_wander_t, fixed_angle_is_fixed)
{
	for (int i = 0; i < 8; i++)
	{
		EXPECT_DOUBLE_EQ (fixed_angle (i * M_PI / 4.0), (i * M_PI / 4.0) + M_PI / 4.0);
	}
}

TEST (test_09_02_blob_wander_t, seeded_normal_distribution_in_degrees)
{
	std::mt19937 gen {0};
	std::normal_distribution<double> dist{0.0, 40.0};

	auto rnd_angle = [gen, dist](double previousMoveDirection) mutable 
	{
		return previousMoveDirection + (dist (gen) );
	};

	EXPECT_DOUBLE_EQ (rnd_angle (0.0), 44.911797631556773);
	EXPECT_DOUBLE_EQ (rnd_angle (0.0), 12.112208948296557);
	EXPECT_DOUBLE_EQ (rnd_angle (0.0), 2.8343695072884749);
	EXPECT_DOUBLE_EQ (rnd_angle (0.0), 2.9216560821516016);
	EXPECT_DOUBLE_EQ (rnd_angle (0.0), -56.893033673736078);
	EXPECT_DOUBLE_EQ (rnd_angle (0.0), 60.802779964348865);
}

TEST (test_09_02_blob_wander_t, seeded_normal_distribution_in_degrees_progresses)
{
	std::mt19937 gen {0};
	std::normal_distribution<double> dist{0.0, 40.0};

	auto rnd_angle = [gen, dist](double previousMoveDirection) mutable 
	{
		return previousMoveDirection + dist (gen);
	};

	EXPECT_DOUBLE_EQ (rnd_angle (0.0), 44.911797631556773);
	EXPECT_DOUBLE_EQ (rnd_angle (44.911797631556773), 44.911797631556773 + 12.112208948296557);
	EXPECT_DOUBLE_EQ (rnd_angle (44.911797631556773 + 12.112208948296557),
		 44.911797631556773 + 12.112208948296557 + 2.8343695072884749);
	EXPECT_DOUBLE_EQ (rnd_angle (44.911797631556773 + 12.112208948296557 + 2.8343695072884749),
		44.911797631556773 + 12.112208948296557 + 2.8343695072884749 + 2.9216560821516016); 
	EXPECT_DOUBLE_EQ (rnd_angle (44.911797631556773 + 12.112208948296557 + 2.8343695072884749 + 2.9216560821516016),
		44.911797631556773 + 12.112208948296557 + 2.8343695072884749 + 2.9216560821516016 - 56.893033673736078); 
	EXPECT_DOUBLE_EQ (
	     	rnd_angle (44.911797631556773 + 12.112208948296557 + 2.8343695072884749 + 2.9216560821516016 
			   - 56.893033673736078),
         	44.911797631556773 + 12.112208948296557 + 2.8343695072884749 + 2.9216560821516016 - 56.893033673736078 		   	       + 60.802779964348865);
}

TEST (test_09_02_blob_wander_t, seeded_normal_distribution_in_radians_progresses)
{
	std::mt19937 gen {0};
	std::normal_distribution<> dist{0.0, 40.0};

	auto rnd_angle = [gen, dist](double previousMoveDirection) mutable 
	{
		return previousMoveDirection + (2.0 * M_PI * dist (gen) / 360.0);
	};

        EXPECT_DOUBLE_EQ (rnd_angle (0.0), 0.78385874166005687);
        EXPECT_DOUBLE_EQ (rnd_angle (0.0), 0.21139792583729455);
        EXPECT_DOUBLE_EQ (rnd_angle (0.0), 0.0494690801203133);
        EXPECT_DOUBLE_EQ (rnd_angle (0.0), 0.050992518244463381);
        EXPECT_DOUBLE_EQ (rnd_angle (0.0), -0.99297075905469978);
	EXPECT_DOUBLE_EQ (rnd_angle (0.0), 1.0612087047435281);
	EXPECT_DOUBLE_EQ (rnd_angle (0.0), -0.20343135772250606);
}

TEST (test_09_02_blob_wander_t, uses_fixed_angle)
{
	Blob b = CreateBlob ().lifespan (100U).HP (100U).maxHunger (100U).position (make_pt (10.1, 20.2)).speed (5.0).moveDirectionFn (fixed_angle);
	for (size_t step = 1; step <= 8; step++)
	{
		std::shared_ptr<Action> a = b.createActionWander ();
		ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (a));
		std::shared_ptr <Movement> m (std::dynamic_pointer_cast <Movement> (a));
		EXPECT_EQ (m->_reason, "wandering");
		EXPECT_DOUBLE_EQ (m->_speed, 5.0);
		EXPECT_DOUBLE_EQ (m->_angleInRadians, step * (M_PI / 4.0));
		m->apply ();
	}
}

TEST (test_09_02_blob_wander_t, wanders_in_a_circle)
{
	Blob b = CreateBlob ().position (make_pt (10.1, 20.2))
			      .lifespan (100U).HP (100U).maxHunger (100U)
			      .speed (5.0).endurance (10U).moveDirectionFn (fixed_angle);

	std::shared_ptr<Action> a = b.createActionWander ();
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (a));
	std::shared_ptr <Movement> m (std::dynamic_pointer_cast <Movement> (a));
	EXPECT_EQ (*m, Movement (&b, "wandering", 5, M_PI / 4));
	a->apply ();
        EXPECT_DOUBLE_EQ (b.x (), 13.635533905932737);
	EXPECT_DOUBLE_EQ (b.y (), 23.735533905932737);

	a = b.createActionWander ();
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (m));
	m = std::dynamic_pointer_cast <Movement> (a);
       	EXPECT_TRUE (*m  == Movement (&b, "wandering", 5, M_PI / 2));
	a->apply ();
	EXPECT_DOUBLE_EQ (b.x (), 18.635533905932739);
	EXPECT_DOUBLE_EQ (b.y (), 23.735533905932737);

	a = b.createActionWander ();
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (m));
	m = std::dynamic_pointer_cast <Movement> (a);
	EXPECT_TRUE (*m  == Movement (&b, "wandering", 5, 3 * M_PI / 4));
	a->apply ();
        EXPECT_DOUBLE_EQ (b.x (), 22.171067811865477);
	EXPECT_DOUBLE_EQ (b.y (), 20.2);

	a = b.createActionWander ();
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (m));
	m = std::dynamic_pointer_cast <Movement> (a);
	EXPECT_TRUE (*m  == Movement (&b, "wandering", 5, M_PI));
	a->apply ();
        EXPECT_DOUBLE_EQ (b.x (), 22.171067811865477);
	EXPECT_DOUBLE_EQ (b.y (), 15.2);

	a = b.createActionWander ();
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (m));
	m = std::dynamic_pointer_cast <Movement> (a);
	EXPECT_TRUE (*m  == Movement (&b, "wandering", 5, 5 * M_PI / 4));
	m->apply ();
        EXPECT_DOUBLE_EQ (b.x (), 18.635533905932739);
	EXPECT_DOUBLE_EQ (b.y (), 11.664466094067262);

	a = b.createActionWander ();
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (m));
	m = std::dynamic_pointer_cast <Movement> (a);
	EXPECT_TRUE (*m  == Movement (&b, "wandering", 5, 3 * M_PI / 2));
	a->apply ();
        EXPECT_DOUBLE_EQ (b.x (), 13.635533905932739);
	EXPECT_DOUBLE_EQ (b.y (), 11.664466094067262);
	
	a = b.createActionWander ();
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (m));
	m = std::dynamic_pointer_cast <Movement> (a);
	EXPECT_TRUE (*m  == Movement (&b, "wandering", 5, 7 * M_PI / 4));
	a->apply ();
        EXPECT_DOUBLE_EQ (b.x (), 10.1);
	EXPECT_DOUBLE_EQ (b.y (), 15.2);

	a = b.createActionWander ();
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (m));
	m = std::dynamic_pointer_cast <Movement> (a);
	EXPECT_TRUE (*m  == Movement (&b, "wandering", 5, 2 * M_PI));
	a->apply ();
        EXPECT_DOUBLE_EQ (b.x (), 10.1);
	EXPECT_DOUBLE_EQ (b.y (), 20.2);
}

