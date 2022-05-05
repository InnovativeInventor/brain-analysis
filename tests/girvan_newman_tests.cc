#include "catch.hpp"

#include "utils.h"
#include "brain_serde.h"
#include "graph.h"
#include "girvan_newman.h"

#include <iostream>
#include <unordered_map>
#include <unordered_set>
using std::cout;
using std::endl;
using std::make_pair;


TEST_CASE( "find_communities able to detect subgraphs" "[graph][girvan_newman]" ) {
    Graph<std::size_t, double> graph;

    graph.insert_vertex(0);
    graph.insert_vertex(1);
    graph.insert_vertex(2);

    graph.insert_edge(0.5, 1, 2);
    std::unordered_map<std::size_t, int> communities= graph.find_communities();

    REQUIRE(communities.at(0) != communities.at(1));
    REQUIRE(communities.at(0) != communities.at(2));
    REQUIRE(communities.at(1) == communities.at(2));
}

TEST_CASE( "find_communities able to detect subgraphs, non-specific version" "[graph][girvan_newman]" ) {
    size_t comm_size = GENERATE(3, 5, 10);
    
    Graph<std::size_t, double> graph;

    for (std::size_t i = 0; i < comm_size; ++i) {
	for (std::size_t j = 0; j < comm_size; ++j) {
	    graph.insert_vertex(i * comm_size + j);
	}
    }

    for (std::size_t i = 0; i < comm_size; ++i) {
	for (std::size_t j = 0; j < comm_size; ++j) {
	    for (std::size_t k = 0; k < comm_size; ++k) {
		graph.insert_directed_edge(1.0,
					   i * comm_size + j,
					   i * comm_size + k);
	    }
	}
    }

    std::unordered_set<int> unique;

    auto communities = graph.find_communities();
    for (std::size_t i = 0; i < comm_size; ++i) {
	for (std::size_t j = 0; j < comm_size; ++j) {
	    for (std::size_t k = 0; k < comm_size; ++k) {
		REQUIRE( communities.at(i * comm_size + j) ==
			 communities.at(i * comm_size + k) );
		unique.insert(communities.at(i * comm_size + j));
	    }
	}
    }

    REQUIRE( unique.size() == comm_size );
}

TEST_CASE( "modularity calculates correct mod" "[graph][girvan_newman]" ) {
    Graph<std::size_t, double> graph;

    graph.insert_vertex(0);
    graph.insert_vertex(1);
    graph.insert_vertex(2);

    graph.insert_edge(0.5, 1, 2);
    graph.insert_edge(0.2, 0, 1);

    graph.get_orig_m();
    double m = graph.modularity();
    double correct_m = (0.2 - (0.2 * 0.7)/1.4) + (0 - (0.2 * 0.5)/1.4) + (0.5 - (0.7 * 0.5)/1.4);
    correct_m /= 0.7;

    REQUIRE_SIMILAR(m, correct_m);
}

/**
 * this is a 0--1--2 graph, so each edge are lying on 2 shortest paths, for undirected
 * graph, the final scores doubles, so the correct output is 4 for both of them.
 */
TEST_CASE( "Brandes outputs correct edge betweeness centrality - basic" "[graph][girvan_newman]" ) {
    Graph<int, double> graph;

    graph.insert_vertex(0);
    graph.insert_vertex(1);
    graph.insert_vertex(2);

    graph.insert_edge(0.5, 1, 2);
    graph.insert_edge(0.2, 0, 1);

    std::unordered_map<pair<int, int>, double, custom_hash> edge_bc = graph.brandes();
    for (auto it = edge_bc.begin(); it != edge_bc.end(); ++it){
        REQUIRE_SIMILAR(it->second, 4.0);
    }
}

// helper function to compared whether the returned edge betweenness centrality is as expected.
bool _isSame(std::unordered_map<pair<int, int>, double, custom_hash> & cb1, 
std::unordered_map<pair<int, int>, double, custom_hash> & cb2){
    if (cb1.size() != cb2.size()){return false;}
    for (auto it = cb1.begin(); it != cb2.end(); ++it){
        if (cb2.find(it->first) != cb2.end()){
            if (it->second != cb2.at(it->first)){return false;}
        } else if (cb2.find(make_pair((it->first).second, (it->first).first)) != cb2.end()){
            if (it->second != cb2.at(make_pair((it->first).second, (it->first).first))){return false;}
        } else{return false;}
    }
    return true;
}
/**
 * this is a 1 - 2 - 4 - 5 graph, 
 *           | / |   | / |
 *           0 - 3   6 - 7
 * so the 2-4 edge should have the highest centrality score.
 */
TEST_CASE( "Brandes outputs correct edge betweeness centrality - typical" "[graph][girvan_newman]" ) {
    auto voxels = read_in_voxels("tests/data/test_voxels_gn.txt");
    auto edges = read_in_edges("tests/data/test_edges_gn.txt");

    Graph<int, double> graph;
    for (Voxel voxel : voxels) {
	    graph.insert_vertex(voxel.index);
    }
    for (Edge edge : edges) {
	    graph.insert_edge(edge.e, voxels.at(edge.v1).index, voxels.at(edge.v2).index);
    }

    std::unordered_map<pair<int, int>, double, custom_hash> edge_bc = graph.brandes();
    std::unordered_map<pair<int, int>, double, custom_hash> correct_bc;

    correct_bc = {{make_pair(1, 2), 11.0}, {make_pair(2, 4), 32.0}, {make_pair(4, 5), 15.0},
                  {make_pair(1, 0), 3.0}, {make_pair(2, 0), 10.0}, {make_pair(2, 3), 11.0},
                  {make_pair(4, 6), 15.0}, {make_pair(5, 6), 2.0}, {make_pair(5, 7), 7.0},
                  {make_pair(0, 3), 3.0}, {make_pair(6, 7), 7.0}};

    REQUIRE(_isSame(edge_bc, correct_bc));

}

/**
 * this is a 1 - 2 - 4 - 5 graph, 
 *           | / |   | / |
 *           0 - 3   6 - 7
 * so the 2-4 edge should be removed after the girvan-newman algorithm
 */
TEST_CASE( "Girvan_newman removing the edge with highest centrality" "[graph][girvan_newman]" ) {
    auto voxels = read_in_voxels("tests/data/test_voxels_gn.txt");
    auto edges = read_in_edges("tests/data/test_edges_gn.txt");

    Graph<int, double> graph;
    for (Voxel voxel : voxels) {
	    graph.insert_vertex(voxel.index);
    }
    for (Edge edge : edges) {
	    graph.insert_edge(edge.e, voxels.at(edge.v1).index, voxels.at(edge.v2).index);
    }
    graph.get_orig_m();
    double prev_modularity = graph.modularity();

    graph.girvan_newman(0.5, false);

    double new_modularity = graph.modularity();

    //check whether modularity rose;
    REQUIRE(new_modularity > 0.5);
    REQUIRE(new_modularity > prev_modularity);

    //check whether the edge with the highest centrality was removed
    REQUIRE(graph.get_edge(2, 4)==std::nullopt);
    REQUIRE(graph.get_edge(4, 2)==std::nullopt);
}

