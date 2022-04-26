#include "catch.hpp"

#include "brain_serde.h"
#include "utils.h"

TEST_CASE( "Dataset readable test 1", "[graph][io]" ) {
    auto voxels = read_in_voxels("tests/data/test_voxels1.txt");
    auto edges = read_in_edges("tests/data/test_edges1.txt");

    REQUIRE( voxels.size() != 0 );
    REQUIRE( edges.size() != 0 );
    
    std::vector<Voxel> correct_voxels = {
	{0, "NAME1", 10.0, 2.0, -4.0},
	{1, "NAME1", 2.0, 3.0, 4.0},
	{2, "NAME1", 7.0, 7.0, 7.0},
	{3, "NAME1", -1.0, -10.0, -2.0},
	{4, "NAME1", 1.0, 1.0, 1.0},
	{5, "NAME2", 0.0, 0.0, 0.0},
	{6, "NAME2", 0.0, 0.0, 0.0},
    };

    REQUIRE( voxels == correct_voxels );

    std::vector<Edge> correct_edges = {
	{0, 1, 5.0},
	{3, 6, -2.0},
    };

    REQUIRE( edges == correct_edges );
}

TEST_CASE( "Dataset readable test 2", "[graph][io]" ) {
    auto voxels = read_in_voxels("tests/data/test_voxels2.txt");

    REQUIRE( voxels.size() != 0 );

    std::vector<Voxel> correct_voxels = {
	{0, "A", 1.0, 0.0, 0.0},
	{1, "A", 0.0, 0.0, 0.0},
	{2, "A", 0.0, 0.0, 0.0},
	{3, "B", 0.0, 0.0, 0.0},
	{4, "B", 0.0, 0.0, 0.0},
	{5, "B", 0.0, 0.0, 0.0},
	{6, "B", 0.0, 0.0, 0.0},
	{7, "B", 0.0, 0.0, 0.0},
	{8, "B", 0.0, 0.0, 0.0},
	{9, "B", 0.0, 0.0, 0.0},
	{10, "B", 0.0, 0.0, 0.0},
	{11, "B", 0.0, 0.0, 0.0},
	{12, "B", 0.0, 0.0, 0.0},
	{13, "B", 0.0, 0.0, 0.0},
	{14, "B", 0.0, 0.0, 0.0},
	{15, "B", 0.0, 0.0, 0.0},
	{16, "B", 0.0, 0.0, 0.0},
	{17, "B", 0.0, 0.0, 0.0},
	{18, "B", 0.0, 0.0, 0.0},
	{19, "B", 0.0, 0.0, 0.0},
	{20, "B", 0.0, 0.0, 0.0},
	{21, "B", 0.0, 0.0, 0.0},
	{22, "B", 0.0, 0.0, 0.0},
	{23, "B", 0.0, 0.0, 0.0},
	{24, "B", 0.0, 0.0, 0.0},
	{25, "B", 0.0, 0.0, 0.0},
	{26, "B", 0.0, 0.0, 0.0},
	{27, "B", 0.0, 0.0, 0.0},
	{28, "B", 0.0, 0.0, 0.0},
    };

    REQUIRE( voxels == correct_voxels );
}
