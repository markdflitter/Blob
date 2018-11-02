#include <gtest/gtest.h>
#include <blob.h>

namespace
{
	double fixed_angle (double previousMoveDirection)
	{
		return previousMoveDirection + M_PI / 4.0;
	};
}

TEST (test_11_07_blob_chooseNextAction_wander_t, chooses_to_wander_in_a_circle)
{
	Blob b = CreateBlob ().position (make_pt (10.1, 20.2)).speed (5.0).moveDirectionFn (fixed_angle).HP (100U).maxHunger (100U).lifespan (10U);
	std::vector<Blob> blobs;
	
	std::shared_ptr<Action> m = b.chooseNextAction (blobs);
	m->apply ();
        EXPECT_DOUBLE_EQ (b.x (), 13.63553390593273000);
	EXPECT_DOUBLE_EQ (b.y (), 23.73553390593272737);

	m = b.chooseNextAction (blobs);
        m->apply ();
	EXPECT_DOUBLE_EQ (b.x (), 18.635533905932739);
	EXPECT_DOUBLE_EQ (b.y (), 23.735533905932737);
	
	m = b.chooseNextAction (blobs);
	m->apply ();
        EXPECT_DOUBLE_EQ (b.x (), 22.171067811865477);
	EXPECT_DOUBLE_EQ (b.y (), 20.2);

	m = b.chooseNextAction (blobs);
	m->apply ();
        EXPECT_DOUBLE_EQ (b.x (), 22.171067811865477);
	EXPECT_DOUBLE_EQ (b.y (), 15.2);

	m = b.chooseNextAction (blobs);
	m->apply ();
        EXPECT_DOUBLE_EQ (b.x (), 18.635533905932739);
	EXPECT_DOUBLE_EQ (b.y (), 11.664466094067262);

	m = b.chooseNextAction (blobs);
	m->apply ();
        EXPECT_DOUBLE_EQ (b.x (), 13.635533905932739);
	EXPECT_DOUBLE_EQ (b.y (), 11.664466094067262);

	m = b.chooseNextAction (blobs);
	m->apply ();
        EXPECT_DOUBLE_EQ (b.x (), 10.1);
	EXPECT_DOUBLE_EQ (b.y (), 15.2);
	
	m = b.chooseNextAction (blobs);
	m->apply ();
        EXPECT_DOUBLE_EQ (b.x (), 10.1);
	EXPECT_DOUBLE_EQ (b.y (), 20.2);
}

TEST (test_11_07_blob_chooseNextAction_wander_t, chooses_to_wander_randomly)
{
	std::mt19937 gen {0};
	std::normal_distribution<> dist{0.0, 40.0};

	auto rnd_angle = [gen, dist](double previousMoveDirection) mutable 
	{
		return previousMoveDirection + (2.0 * M_PI * dist (gen) / 360.0);
	};

	Blob b = CreateBlob ().position (make_pt (10.1, 20.2)).speed (5.0).moveDirectionFn (rnd_angle).HP (100U).maxHunger (100U).lifespan (10U);
	EXPECT_DOUBLE_EQ (b.x (), 10.1);
	EXPECT_DOUBLE_EQ (b.y (), 20.2);

	std::vector<Blob> blobs;
	std::shared_ptr<Action> a = b.chooseNextAction (blobs);
	ASSERT_TRUE (std::dynamic_pointer_cast <Movement> (a));
	std::shared_ptr <Movement> m (std::dynamic_pointer_cast <Movement> (a));
	EXPECT_DOUBLE_EQ (m->_angleInRadians, 0.78385874166005687);
}

TEST (test_11_07_blob_chooseNextAction_wander_t, wanders_randomly)
{
	std::mt19937 gen {0};
	std::normal_distribution<> dist{0.0, 40.0};

	auto rnd_angle = [gen, dist](double previousMoveDirection) mutable 
	{
		return previousMoveDirection + (2.0 * M_PI * dist (gen) / 360.0);
	};

	Blob b = CreateBlob ().position (make_pt (10.1, 20.2)).speed (5.0).moveDirectionFn (rnd_angle).HP (100U).maxHunger (100U).lifespan (10U);
	EXPECT_DOUBLE_EQ (b.x (), 10.1);
	EXPECT_DOUBLE_EQ (b.y (), 20.2);

	std::vector<Blob> blobs;
	std::shared_ptr<Action> m = b.chooseNextAction (blobs);
	m->apply ();
	EXPECT_DOUBLE_EQ (b.x (), 13.630087041046963);
	EXPECT_DOUBLE_EQ (b.y (), 23.74097239224373);

	m = b.chooseNextAction (blobs);
	m->apply ();
	EXPECT_DOUBLE_EQ (b.x (), 17.824580514716409);
	EXPECT_DOUBLE_EQ (b.y (), 26.46241033923874);
	
	m = b.chooseNextAction (blobs);
	m->apply ();
	EXPECT_DOUBLE_EQ (b.x (), 22.14851480285758);
	EXPECT_DOUBLE_EQ (b.y (), 28.9731059145399569);
}

TEST (test_11_07_blob_chooseNextAction_wander_t, different_blobs_wander_independently)
{
	Blob b1 = CreateBlob ().speed (10.0).moveDirectionFn (RandomMove (1.0, 40.0)).HP (100U).maxHunger (100U).lifespan (100U);
	Blob b2 = CreateBlob ().speed (10.0).moveDirectionFn (RandomMove (1.0, 40.0)).HP (100U).maxHunger (100U).lifespan (100U);;
	Blob b3 = CreateBlob ().speed (10.0).moveDirectionFn (RandomMove (2.0, 40.0)).HP (100U).maxHunger (100U).lifespan (100U);;
	
	std::vector<Blob> blobs;
	b1.chooseNextAction (blobs)->apply ();
	b2.chooseNextAction (blobs)->apply ();
	b3.chooseNextAction (blobs)->apply ();

	EXPECT_DOUBLE_EQ (b1.x (), b2.x ());
	EXPECT_DOUBLE_EQ (b1.y (), b2.y ());
	EXPECT_NE (b1.x (), b3.x ());
	EXPECT_NE (b1.y (), b3.y ());
}

