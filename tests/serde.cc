#include "catch.hpp"
#include "brain_serde.hpp"

TEST_CASE( "Dataset readable", "[graph][io]" ) {
    auto voxels = read_in_voxels("data/coords.txt");
    auto edges = read_in_edges("data/img_edge_list.txt");

    REQUIRE( voxels.size() != 0 );
    REQUIRE( edges.size() != 0 );
}
