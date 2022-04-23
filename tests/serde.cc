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
