#include "option.h"
#include <gtest/gtest.h>
#include "blob.h"

TEST (test_02_02_option_t, action)
{
	Option o (attack, 0.0, nullptr);
	EXPECT_EQ (o.action (), attack); 
}

TEST (test_02_02_option_t, weight)
{
	Option o (attack, 0.5, nullptr);
	EXPECT_DOUBLE_EQ (o.weight (), 0.5);
}

TEST (test_02_02_option_t, target)
{
	Blob b = CreateBlob ();
	Option o (attack, 0.0, &b);
	EXPECT_EQ (o.target (), &b); 
}

TEST (test_02_02_option_t, matches_same)
{
	Blob b = CreateBlob ();
	Option o (attack, 0.0, &b);
	EXPECT_TRUE (o.matches (o));
}

TEST (test_02_02_option_t, matches)
{
	Blob b = CreateBlob ();
	Option o1 (attack, 0.0, &b);
	Option o2 (attack, 0.0, &b);
	EXPECT_TRUE (o1.matches (o2));
}

TEST (test_02_02_option_t, matches_different_weight)
{
	Blob b = CreateBlob ();
	Option o1 (attack, 0.0, &b);
	Option o2 (attack, 0.5, &b);
	EXPECT_TRUE (o1.matches (o2));
}

TEST (test_02_02_option_t, does_not_match_different_action)
{
	Blob b = CreateBlob ();
	Option o1 (attack, 0.0, &b);
	Option o2 (flee, 0.0, &b);
	EXPECT_FALSE (o1.matches (o2));
}

TEST (test_02_02_option_t, does_not_match_different_target)
{
	Blob b1 = CreateBlob ();
	Blob b2 = CreateBlob ();
	Option o1 (attack, 0.0, &b1);
	Option o2 (attack, 0.0, &b2);
	EXPECT_FALSE (o1.matches (o2));
}

TEST (test_02_02_option_t, does_not_match_both_different)
{
	Blob b1 = CreateBlob ();
	Blob b2 = CreateBlob ();
	Option o1 (attack, 0.0, &b1);
	Option o2 (flee, 0.0, &b2);
	EXPECT_FALSE (o1.matches (o2));
}

