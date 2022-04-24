#include "catch.hpp"

#include "utils.h"
#include "graph.h"
#include "rank.h"

#include<iostream>
TEST_CASE( "Manually constructed trivial pagerank test 1" "[graph][pagerank]" ) {
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
