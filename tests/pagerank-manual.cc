#include "catch.hpp"

#include "utils.h"
#include "graph.h"
#include "rank.h"

TEST_CASE( "Manually constructed trivial pagerank test" "[graph][pagerank]" ) {
    Graph<std::size_t, double> graph;

    graph.insert_vertex(0);
    graph.insert_vertex(1);
    graph.insert_vertex(2);

    graph.insert_directed_edge(0.5, 1, 2);

    auto ranks = graph.rank(1, 0.85);

    std::vector<double> correct_ranks = {0.15, 0.15, (1.0 / 3.0) * 0.85 + 0.15};
    for (std::size_t i = 0; i < 3; ++i)
	REQUIRE_SIMILAR( ranks.at(i), correct_ranks.at(i) );

    graph.normalize();

    auto ranks_norm = graph.rank(1, 0.85);
    for (std::size_t i = 0; i < 3; ++i)
	REQUIRE_SIMILAR( ranks_norm.at(i), ranks.at(i) );
}

TEST_CASE( "Manually constructed trivial convergence pagerank test 1" "[graph][pagerank]" ) {
    Graph<std::size_t, double> graph;

    graph.insert_vertex(0);
    graph.insert_vertex(1);

    graph.insert_directed_edge(1.0, 0, 1);

    auto ranks1 = graph.rank(1, 0.7);
    auto ranks2 = graph.rank(2, 0.7);
    auto ranks3 = graph.rank(10, 0.7);

    REQUIRE_SIMILAR( ranks1[0], 0.3 );
    REQUIRE_SIMILAR( ranks2[0], 0.3 );
    REQUIRE_SIMILAR( ranks3[0], 0.3 );

    REQUIRE( ranks1[1] > ranks2[1] );
    REQUIRE_SIMILAR( ranks2[1], ranks3[1] );
    REQUIRE_SIMILAR( ranks3[1], 0.51 );
}

TEST_CASE( "Manually constructed trivial convergence pagerank test 2" "[graph][pagerank]" ) {
    Graph<std::size_t, double> graph;

    graph.insert_vertex(0);
    graph.insert_vertex(1);
    graph.insert_vertex(2);
    graph.insert_vertex(3);

    graph.insert_directed_edge(0.3, 1, 0);
    graph.insert_directed_edge(0.5, 1, 2);
    graph.insert_directed_edge(0.1, 2, 0);
    graph.insert_directed_edge(0.7, 0, 3);
    graph.insert_directed_edge(0.2, 3, 1);

    auto ranks1 = graph.rank(1, 0.85);
    auto ranks2 = graph.rank(10, 0.85);
    auto ranks3 = graph.rank(100, 0.85);
    for (std::size_t i = 0; i < 4; ++i) { // Pagerank, in this scenario, converge upwards - check that d/dx is positive and that d2/dx2 is negative
	REQUIRE( ranks1.at(i) < ranks2.at(i) );
	REQUIRE( ranks2.at(i) < ranks3.at(i) );
	REQUIRE( ranks3.at(i) - ranks2.at(i) < ranks2.at(i) - ranks1.at(i) );
    }
}
