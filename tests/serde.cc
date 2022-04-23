#include "catch.hpp"

#include "brain_serde.hpp"

TEST_CASE( "Dataset readable", "[graph][io]" ) {
    auto voxels = read_in_voxels("tests/data/test_voxels1.txt");
    auto edges = read_in_edges("tests/data/test_edges1.txt");

    REQUIRE( voxels.size() != 0 );
    REQUIRE( edges.size() != 0 );
}
