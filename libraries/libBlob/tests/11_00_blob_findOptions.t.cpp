#include <gtest/gtest.h>
#include <blob.h>

TEST (test_11_00_blob_findOptions_t,
  returns_nothing_if_alone)
{
  	Blob b = CreateBlob ();
	std::vector<Blob> blobs;
	auto actions = b.findOptions (blobs);
	
	EXPECT_TRUE (actions.empty ());
}

TEST (test_11_00_blob_findOptions_t,
  does_not_consider_self)
{
	std::vector<Blob> blobs {CreateBlob ().smell (100.0)};
	auto actions = blobs[0].findOptions (blobs);
	
	EXPECT_TRUE (actions.empty ());
}

TEST (test_11_00_blob_findOptions_t,
  does_not_consider_the_dead)
{
	std::vector<Blob> blobs {CreateBlob ().smell (100.0), CreateBlob ()};
	blobs[1].kill ();
	auto actions = blobs[0].findOptions (blobs);
	
	EXPECT_TRUE (actions.empty ());
}

TEST (test_11_00_blob_findOptions_t,
  does_not_consider_others_in_other_squares_or_outside_smell_range)
{
	std::vector<Blob> blobs {CreateBlob ().smell (282.0).damage (100U).lifespan (100U).maxHunger (100U), 
		                 CreateBlob ().position (make_pt (100.0, 100.0)).HP (5U).lifespan (100U).maxHunger (100U)};
	auto actions = blobs[0].findOptions (blobs);
	
	EXPECT_TRUE (actions.empty ());
}

TEST (test_11_00_blob_findOptions_t,
  adds_flee_and_attack_actions_for_others_in_same_square)
{
	std::vector<Blob> blobs {CreateBlob ().HP (100U).lifespan (100U).maxHunger (100U), CreateBlob ().HP (100U).lifespan (100U).maxHunger (100U)};
	auto actions = blobs[0].findOptions (blobs);
	
	EXPECT_EQ (actions.size (), 2U);
	EXPECT_EQ (actions[0].action (), attack);
	EXPECT_EQ (actions[0].target (), &blobs[1]);
	EXPECT_EQ (actions[1].action (), flee);
	EXPECT_EQ (actions[1].target (), &blobs[1]);
}


TEST (test_11_00_blob_findOptions_t,
  adds_flee_and_attack_options_for_others_in_smell_range)
{
	std::vector<Blob> blobs {CreateBlob ().smell (10.0).HP (100U).lifespan (100U).maxHunger (100U),
				 CreateBlob ().position (make_pt (3.0, 4.0)).HP (100U).lifespan (100U).maxHunger (100U)};
	auto actions = blobs[0].findOptions (blobs);
	
	EXPECT_EQ (actions.size (), 2U);
	EXPECT_EQ (actions[0].action (), attack);
	EXPECT_EQ (actions[0].target (), &blobs[1]);
	EXPECT_EQ (actions[1].action (), flee);
	EXPECT_EQ (actions[1].target (), &blobs[1]);
}

TEST (test_11_00_blob_findOptions_t,
  adds_flee_and_attack_options_for_more_than_1_other_in_same_square)
{
	std::vector<Blob> blobs {CreateBlob ().HP (100U).lifespan (100U).maxHunger (100U),
				 CreateBlob ().HP (100U).lifespan (100U).maxHunger (100U),
				 CreateBlob ().HP (100U).lifespan (100U).maxHunger (100U)};
	auto actions = blobs[0].findOptions (blobs);
	
	EXPECT_EQ (actions.size (), 4U);
	EXPECT_EQ (actions[0].action (), attack);
	EXPECT_EQ (actions[0].target (), &blobs[1]);
	EXPECT_EQ (actions[1].action (), flee);
	EXPECT_EQ (actions[1].target (), &blobs[1]);
	EXPECT_EQ (actions[2].action (), attack);
	EXPECT_EQ (actions[2].target (), &blobs[2]);
	EXPECT_EQ (actions[3].action (), flee);
	EXPECT_EQ (actions[3].target (), &blobs[2]);
}

TEST (test_11_00_blob_findOptions_t,
  adds_flee_and_attack_options_for_more_than_1_other_in_smell_range)
{
	std::vector<Blob> blobs {CreateBlob ().smell (10.0).HP (100U).lifespan (100U).maxHunger (100U),
			         CreateBlob ().position (make_pt (1.0, 1.0)).HP (100U).lifespan (100U).maxHunger (100U),
				 CreateBlob ().position (make_pt (2.0, 2.0)).HP (100U).lifespan (100U).maxHunger (100U)};
	auto actions = blobs[0].findOptions (blobs);
	
	EXPECT_EQ (actions.size (), 4U);
	EXPECT_EQ (actions[0].action (), attack);
	EXPECT_EQ (actions[0].target (), &blobs[1]);
	EXPECT_EQ (actions[1].action (), flee);
	EXPECT_EQ (actions[1].target (), &blobs[1]);
	EXPECT_EQ (actions[2].action (), attack);
	EXPECT_EQ (actions[2].target (), &blobs[2]);
	EXPECT_EQ (actions[3].action (), flee);
	EXPECT_EQ (actions[3].target (), &blobs[2]);
}

TEST (test_11_00_blob_findOptions_t,
  adds_flee_and_attack_options_all_relevant_others)
{
	std::vector<Blob> blobs {CreateBlob ().smell (10.0).HP (100U).lifespan (100U).maxHunger (100U),
				 CreateBlob ().HP (100U).lifespan (100U).maxHunger (100U),
				 CreateBlob ().HP (100U).lifespan (100U).maxHunger (100U),
			         CreateBlob ().position (make_pt (1.0, 1.0)).HP (100U).lifespan (100U).maxHunger (100U),
				 CreateBlob ().position (make_pt (2.0, 2.0)).HP (100U).lifespan (100U).maxHunger (100U)};
	auto actions = blobs[0].findOptions (blobs);
	
	EXPECT_EQ (actions.size (), 8U);
	EXPECT_EQ (actions[0].action (), attack);
	EXPECT_EQ (actions[0].target (), &blobs[1]);
	EXPECT_EQ (actions[1].action (), flee);
	EXPECT_EQ (actions[1].target (), &blobs[1]);
	EXPECT_EQ (actions[2].action (), attack);
	EXPECT_EQ (actions[2].target (), &blobs[2]);
	EXPECT_EQ (actions[3].action (), flee);
	EXPECT_EQ (actions[3].target (), &blobs[2]);
	EXPECT_EQ (actions[4].action (), attack);
	EXPECT_EQ (actions[4].target (), &blobs[3]);
	EXPECT_EQ (actions[5].action (), flee);
	EXPECT_EQ (actions[5].target (), &blobs[3]);
	EXPECT_EQ (actions[6].action (), attack);
	EXPECT_EQ (actions[6].target (), &blobs[4]);
	EXPECT_EQ (actions[7].action (), flee);
	EXPECT_EQ (actions[7].target (), &blobs[4]);
}

