#include <gtest/gtest.h>
#include <blob.h>

TEST (test_03_01_blob_history_t, history)
{
	Blob b1 = CreateBlob ().position (make_pt (10.0, 20.0));
	b1.move (5.0, M_PI, "");
	b1.move (5.0, M_PI, "");
	b1.move (5.0, M_PI / 2, "");

	std::vector<Pt<double>> history = b1.history ();
	EXPECT_EQ (history.size (), 4U);
	EXPECT_DOUBLE_EQ (history[0].x (), 10.0);
 	EXPECT_DOUBLE_EQ (history[0].y (), 20.0);
	EXPECT_DOUBLE_EQ (history[1].x (), 10.0);
 	EXPECT_DOUBLE_EQ (history[1].y (), 15.0);
	EXPECT_DOUBLE_EQ (history[2].x (), 10.0);
 	EXPECT_DOUBLE_EQ (history[2].y (), 10.0);
	EXPECT_DOUBLE_EQ (history[3].x (), 15.0);
 	EXPECT_DOUBLE_EQ (history[3].y (), 10.0);
}

TEST (test_03_01_blob_history_t, history_length)
{
	Blob b1 = CreateBlob ().position (make_pt (10.0, -500.0));
	for (int i = 0; i < 1000; i++)
	{
		b1.move (1.0, 0, "");
	}
	
	std::vector<Pt<double>> history = b1.history ();
	EXPECT_EQ (history.size (), 200U);
	
	EXPECT_DOUBLE_EQ (history[0].x (), 10.0);
 	EXPECT_DOUBLE_EQ (history[0].y (), 301.0);
	
	EXPECT_DOUBLE_EQ (history[199].x (), 10.0);
 	EXPECT_DOUBLE_EQ (history[199].y (), 500.0);
}

