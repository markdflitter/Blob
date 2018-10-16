#include <gtest/gtest.h>
#include <pt.h>

TEST (test_01_00_pt_t, create_empty) {
  	Pt<double> p;
  
	EXPECT_DOUBLE_EQ (p.x (), 0.0);
	EXPECT_DOUBLE_EQ (p.y (), 0.0);
}

TEST (test_01_00_pt_t, create) {
  	Pt<double> p (10.0, 20.0);

  	EXPECT_DOUBLE_EQ (p.x (), 10.0);
  	EXPECT_DOUBLE_EQ (p.y (), 20.0);
}

TEST (test_01_00_pt_t, update) {
 	Pt<double> p (10.0, 20.0);

  	p.x () = 30.0;
  	EXPECT_DOUBLE_EQ (p.x (), 30.0);

  	p.y () = 40.0;
  	EXPECT_DOUBLE_EQ (p.x (), 30.0);
  	EXPECT_DOUBLE_EQ (p.y (), 40.0);
}

TEST (test_01_00_pt_t, make_pt)
{
	auto p = make_pt (10.0, 20.0);
	
	EXPECT_DOUBLE_EQ (p.x (), 10.0);
	EXPECT_DOUBLE_EQ (p.y (), 20.0);
}

