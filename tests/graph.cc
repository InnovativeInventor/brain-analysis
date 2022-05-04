#include "catch.hpp"

#include "utils.h"
#include "graph.h"
#include "rank.h"

TEST_CASE( "Graph construction 1", "[graph]" ) {
    Graph<std::size_t, double> graph;

    graph.insert_vertex(0);
    graph.insert_vertex(1);
    graph.insert_vertex(2);

    REQUIRE( graph.num_vertices() == 3 );

    graph.insert_edge(1.0, 0, 1);
    graph.insert_edge(2.0, 0, 2);
    graph.insert_directed_edge(3.0, 1, 2);

    REQUIRE( graph.num_edges() == 5 );
    REQUIRE( graph.degree(0) == 2 );

    graph.remove_directed_edge(0, 1);

    REQUIRE( graph.degree(0) == 1 );
    REQUIRE( graph.num_edges() == 4 );

    double counter = 0.0;
    graph.map_edges([&](double e) { counter += e; });
    REQUIRE( counter == 8.0 );
}

TEST_CASE( "Graph construction 2", "[graph]" ) {
    size_t graph_size = GENERATE(10, 30, 100);

    Graph<std::size_t, double> graph;

    for (std::size_t i = 0; i < graph_size; ++i) {
	graph.insert_vertex(i);
    }

    REQUIRE( graph.num_vertices() == graph_size );

    double correct_counter = 0.0;
    for (std::size_t i = 0; i < graph_size; ++i) {
	for (std::size_t j = 0; j < graph_size; ++j) {
	    if (i == j) continue;
	    double val = (double) ((std::hash<std::size_t>()(i) ^ std::hash<std::size_t>()(j)) % graph_size);
	    graph.insert_directed_edge(val, i, j);
	    correct_counter += val;
	}
    }

    REQUIRE( graph.num_edges() == (graph_size - 1) * graph_size );

    double map_counter = 0.0;
    graph.map_edges([&](double e) { map_counter += e; });
    REQUIRE( map_counter == correct_counter );

    double get_counter = 0.0;
    for (std::size_t i = 0; i < graph_size; ++i) {
	REQUIRE( graph.degree(i) == graph_size - 1 );
	for (std::size_t j = 0; j < graph_size; ++j) {
	    get_counter += graph.get_edge(i, j).value_or(0.0);
	}
    }
    REQUIRE( get_counter == correct_counter );

    graph.normalize();
    double norm_counter = 0.0;
    for (std::size_t i = 0; i < graph_size; ++i) {
	REQUIRE( graph.degree(i) == graph_size - 1 );
	for (std::size_t j = 0; j < graph_size; ++j) {
	    norm_counter += graph.get_edge(i, j).value_or(0.0);
	}
    }
    REQUIRE_SIMILAR( norm_counter, (double) (graph_size) );
}
