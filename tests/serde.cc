#include <sstream>

#include "catch.hpp"

#include "brain_serde.h"
#include "graph.h"
#include "utils.h"

TEST_CASE( "Dataset readable test 1", "[io]" ) {
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

TEST_CASE( "Dataset readable test 2", "[io]" ) {
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

TEST_CASE( "Graph writable test 1", "[graph][io]" ) {
    std::size_t num_voxels = GENERATE(3, 10, 20);
    std::vector<Voxel> voxels;
    for (std::size_t i = 0; i < num_voxels; ++i)
	voxels.push_back({i, "", 0.0, 0.0, 0.0});
    
    Graph<std::size_t, double> graph;
    for (std::size_t i = 0; i < num_voxels; ++i)
	graph.insert_vertex(i);

    std::vector<std::vector<double>> adj_mat(num_voxels, std::vector<double>(num_voxels, 0.0));

    for (std::size_t i = 0; i < num_voxels; ++i) {
	for (std::size_t j = 0; j < num_voxels; ++j) {
	    adj_mat[i][j] = static_cast<double>((std::hash<std::size_t>()(i) ^ std::hash<std::size_t>()(j)) % num_voxels);
	    graph.insert_directed_edge(adj_mat[i][j], j, i);
	}
    }

    write_out_graph("tests/data/test_write_graph1.txt", graph, voxels);
    std::fstream fs("tests/data/test_write_graph1.txt", std::ios::in);
    std::stringstream buf;
    buf << fs.rdbuf();

    std::stringstream correct;
    for (std::size_t i = 0; i < num_voxels; ++i) {
	for (std::size_t j = 0; j < num_voxels; ++j) {
	    correct << std::to_string(adj_mat[i][j]) << " ";
	}
	correct << "\n";
    }
    REQUIRE( buf.str() == correct.str() );
}
