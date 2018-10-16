cmake_minimum_required (VERSION 2.6)
project (libBlob)

INCLUDE_DIRECTORIES (./include)

include (GoogleTest)

find_package (GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
enable_testing()

add_executable (test_libBlob "")
target_sources (test_libBlob 
	PRIVATE
		"tests/test_libBlob.m.cpp"
		"tests/00_00_template.t.cpp"
		"tests/01_00_pt.t.cpp"
		"tests/02_00_movement.t.cpp"
		"tests/02_01_fight.t.cpp"
		"tests/02_02_option.t.cpp"
		"tests/03_00_blob.t.cpp"
		"tests/03_01_blob_history.t.cpp"
		"tests/04_00_blob_distance.t.cpp"
		"tests/05_00_blob_isInRange.t.cpp"
		"tests/05_01_blob_isInSameSquare.t.cpp"
		"tests/05_02_blob_canSmell.t.cpp"
		"tests/06_00_blob_angle.t.cpp"
		"tests/07_00_blob_kill.t.cpp"
		"tests/07_01_blob_move.t.cpp"
		"tests/07_02_blob_fight.t.cpp"
		"tests/07_03_blob_attack.t.cpp"
		"tests/08_00_blob_age.t.cpp"
		"tests/08_01_blob_hunger.t.cpp"
		"tests/09_00_blob_flee.t.cpp"
		"tests/09_01_blob_hunt.t.cpp"
		"tests/09_02_blob_wander.t.cpp"
		"tests/10_00_blob_relativeDifference.t.cpp"
		"tests/10_01_blob_distanceWeight.t.cpp"
		"tests/10_02_blob_attackWeight.t.cpp"
		"tests/10_03_blob_fleeWeight.t.cpp"
		"tests/11_00_blob_findOptions.t.cpp"
		"tests/11_01_blob_prioritiseOptions.t.cpp"
		"tests/11_02_blob_selectBestOption_same_square.t.cpp"
		"tests/11_03_blob_selectBestOption_in_range.t.cpp"
		"tests/11_04_blob_selectBestOption_wander.t.cpp"
		"tests/11_05_blob_selectBestOption_aggression.t.cpp"
		"tests/11_06_blob_chooseNextAction.t.cpp"
		"tests/11_07_blob_chooseNextAction_wander.t.cpp"
		"tests/12_00_blob_tired.t.cpp"
)

target_link_libraries (test_libBlob ${GTEST_LIBRARIES} pthread)

gtest_discover_tests (test_libBlob)
