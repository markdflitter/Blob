#include <gtest/gtest.h>
#include <blob.h>

TEST (test_11_01_blob_prioritiseOptions, no_options)
{
	std::vector<Option> v;
	Blob b = CreateBlob ();
	std::vector <Option> r = b.prioritiseOptions (v);

	EXPECT_TRUE (r.empty ());
}

TEST (test_11_01_blob_prioritiseOptions, various_options)
{
	std::vector<Option> v {
		Option (attack, 10.0, 0),
		Option (attack, 20.0, 0),
		Option (attack, 5.0, 0),
		Option (attack, -4.0, 0),
		Option (attack, 12.0, 0)
	};

	Blob b = CreateBlob ();
	std::vector <Option> r = b.prioritiseOptions (v);

	EXPECT_EQ (r.size (), 5U);
	EXPECT_DOUBLE_EQ (r[0].weight (), -4.0);
	EXPECT_DOUBLE_EQ (r[1].weight (), 5.0);
	EXPECT_DOUBLE_EQ (r[2].weight (), 10.0);
	EXPECT_DOUBLE_EQ (r[3].weight (), 12.0);
	EXPECT_DOUBLE_EQ (r[4].weight (), 20.0);
}

