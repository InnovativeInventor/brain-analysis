#include "catch.hpp"
#include "graph.h"
#include "rank.h"

TEST_CASE( "Graph is constructable", "[graph][construction]" ) {
  Graph<std::size_t, double> graph;

  REQUIRE (graph.num_vertices() == 0);
  REQUIRE (graph.num_edges() == 0);

  for (size_t i = 0; i < 10; i++) {
    graph.insert_vertex(i);
    REQUIRE (graph.num_vertices() == i+1);
    REQUIRE (graph.num_edges() == 0);
  }

}
