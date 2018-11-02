#include <gtest/gtest.h>
#include <blob.h>

TEST (test_05_02_blob_canSmell_t, can_not_smell)
{
	Blob b1 = CreateBlob ().position (make_pt (3.0, 4.0)).smell (8.0).lifespan (100U).maxHunger (100U).HP (100U);
	Blob b2 = CreateBlob ().position (make_pt (6.0, 8.0)).smell (8.0).lifespan (100U).maxHunger (100U).HP (100U);

	EXPECT_FALSE (b1.canSmell (b2));
	EXPECT_FALSE (b2.canSmell (b1));
}

TEST (test_05_02_blob_canSmell_t, can_not_smell_yet)
{
	Blob b1 = CreateBlob ().position (make_pt (3.0, 4.0)).smell (5.0).lifespan (100U).maxHunger (100U).HP (100U);
	Blob b2 = CreateBlob ().position (make_pt (6.0, 8.0)).smell (4.0).lifespan (100U).maxHunger (100U).HP (100U);

	EXPECT_FALSE (b1.canSmell (b2));
}

TEST (test_05_02_blob_canSmell_t, can_smell)
{
	Blob b1 = CreateBlob ().position (make_pt (3.0, 4.0)).smell (10.0).lifespan (100U).maxHunger (100U).HP (100U);
	Blob b2 = CreateBlob ().position (make_pt (6.0, 8.0)).smell (8.0).lifespan (100U).maxHunger (100U).HP (100U);

	EXPECT_TRUE (b1.canSmell (b2));
	EXPECT_FALSE (b2.canSmell (b1));
}

TEST (test_05_02_blob_canSmell_t, can_smell_each_other)
{
	Blob b1 = CreateBlob ().position (make_pt (3.0, 4.0)).smell (10.0).lifespan (100U).maxHunger (100U).HP (100U);
	Blob b2 = CreateBlob ().position (make_pt (6.0, 8.0)).smell (10.0).lifespan (100U).maxHunger (100U).HP (100U);

	EXPECT_TRUE (b1.canSmell (b2));
	EXPECT_TRUE (b2.canSmell (b1));
}

