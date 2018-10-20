#include <gtest/gtest.h>
#include "blob.h"
#include "rnd.h"

namespace
{
	double fixed_angle (double previousMoveDirection)
	{
		return previousMoveDirection + M_PI / 4.0;
	};
}

TEST (test_11_07_blob_chooseNextAction_wander_t, chooses_to_wander_in_a_circle)
{
	Blob b = CreateBlob ().position (make_pt (10.1, 20.2)).speed (10.0).moveDirectionFn (fixed_angle).HP (100U).starvationLevel (1000U).lifespan (1000U);
	std::vector<Blob> blobs;
	
	std::shared_ptr<Action> m = b.chooseNextAction (blobs);
	m->apply ();
	Pt <double> p1 = b.history ().back ();
        EXPECT_DOUBLE_EQ (p1.x (), 13.63553390593273000);
	EXPECT_DOUBLE_EQ (p1.y (), 23.73553390593272737);

	m = b.chooseNextAction (blobs);
        m->apply ();
	Pt <double> p2 = b.history ().back ();
	EXPECT_DOUBLE_EQ (p2.x (), 18.635533905932739);
	EXPECT_DOUBLE_EQ (p2.y (), 23.735533905932737);
	
	m = b.chooseNextAction (blobs);
	m->apply ();
	Pt <double> p3 = b.history ().back ();
        EXPECT_DOUBLE_EQ (p3.x (), 22.171067811865477);
	EXPECT_DOUBLE_EQ (p3.y (), 20.2);

	m = b.chooseNextAction (blobs);
	m->apply ();
	Pt <double> p4 = b.history ().back ();
        EXPECT_DOUBLE_EQ (p4.x (), 22.171067811865477);
	EXPECT_DOUBLE_EQ (p4.y (), 15.2);

	m = b.chooseNextAction (blobs);
	m->apply ();
	Pt <double> p5 = b.history ().back ();
        EXPECT_DOUBLE_EQ (p5.x (), 18.635533905932739);
	EXPECT_DOUBLE_EQ (p5.y (), 11.664466094067262);

	m = b.chooseNextAction (blobs);
	m->apply ();
	Pt <double> p6 = b.history ().back ();
        EXPECT_DOUBLE_EQ (p6.x (), 13.635533905932739);
	EXPECT_DOUBLE_EQ (p6.y (), 11.664466094067262);

	m = b.chooseNextAction (blobs);
	m->apply ();
	Pt <double> p7 = b.history ().back ();
        EXPECT_DOUBLE_EQ (p7.x (), 10.1);
	EXPECT_DOUBLE_EQ (p7.y (), 15.2);
	
	m = b.chooseNextAction (blobs);
	m->apply ();
	Pt <double> p8 = b.history ().back ();
        EXPECT_DOUBLE_EQ (p8.x (), 10.1);
	EXPECT_DOUBLE_EQ (p8.y (), 20.2);
}

TEST (test_11_07_blob_chooseNextAction_wander_t, chooses_to_wander_randomly)
{
	std::mt19937 gen {0};
	std::normal_distribution<> dist{0.0, 40.0};

	auto rnd_angle = [gen, dist](double previousMoveDirection) mutable 
	{
		return previousMoveDirection + (2.0 * M_PI * dist (gen) / 360.0);
	};

	Blob b = CreateBlob ().position (make_pt (10.1, 20.2)).speed (10.0).moveDirectionFn (rnd_angle).HP (100U).starvationLevel (1000U).lifespan (1000U);
	Pt <double> p1 = b.history ().back ();
	EXPECT_DOUBLE_EQ (p1.x (), 10.1);
	EXPECT_DOUBLE_EQ (p1.y (), 20.2);

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

	Blob b = CreateBlob ().position (make_pt (10.1, 20.2)).speed (10.0).moveDirectionFn (rnd_angle).HP (100U).starvationLevel (1000U).lifespan (1000U);
	Pt <double> p1 = b.history ().back ();
	EXPECT_DOUBLE_EQ (p1.x (), 10.1);
	EXPECT_DOUBLE_EQ (p1.y (), 20.2);

	std::vector<Blob> blobs;
	std::shared_ptr<Action> m = b.chooseNextAction (blobs);
	m->apply ();
	Pt <double> p2 = b.history ().back ();
	EXPECT_DOUBLE_EQ (p2.x (), 13.630087041046963);
	EXPECT_DOUBLE_EQ (p2.y (), 23.74097239224373);

	m = b.chooseNextAction (blobs);
	m->apply ();
	Pt <double> p3 = b.history ().back ();
	EXPECT_DOUBLE_EQ (p3.x (), 17.824580514716409);
	EXPECT_DOUBLE_EQ (p3.y (), 26.46241033923874);
	
	m = b.chooseNextAction (blobs);
	m->apply ();
	Pt <double> p4 = b.history ().back ();
	EXPECT_DOUBLE_EQ (p4.x (), 22.14851480285758);
	EXPECT_DOUBLE_EQ (p4.y (), 28.9731059145399569);
}

TEST (test_11_07_blob_chooseNextAction_wander_t, different_blobs_wander_independently)
{
	Blob b1 = CreateBlob ().speed (10.0).moveDirectionFn (RandomMove (1.0, 40.0)).HP (100U).starvationLevel (100U).lifespan (100U);
	Blob b2 = CreateBlob ().speed (10.0).moveDirectionFn (RandomMove (1.0, 40.0)).HP (100U).starvationLevel (100U).lifespan (100U);;
	Blob b3 = CreateBlob ().speed (10.0).moveDirectionFn (RandomMove (2.0, 40.0)).HP (100U).starvationLevel (100U).lifespan (100U);;
	
	std::vector<Blob> blobs;
	b1.chooseNextAction (blobs)->apply ();
	b2.chooseNextAction (blobs)->apply ();
	b3.chooseNextAction (blobs)->apply ();
	
	Pt <double> p1 = b1.history ().back ();
	Pt <double> p2 = b2.history ().back ();
	Pt <double> p3 = b3.history ().back ();

	EXPECT_DOUBLE_EQ (p1.x (), p2.x ());
	EXPECT_DOUBLE_EQ (p1.y (), p2.y ());
	EXPECT_NE (p1.x (), p3.x ());
	EXPECT_NE (p1.y (), p3.y ());
}

