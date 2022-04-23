#include "catch.hpp"

#include "utils.h"
#include "graph.h"
#include "rank.h"

Graph<std::size_t, double> construct_cycle_digraph(size_t cycle_size, double weight) {
  Graph<std::size_t, double> graph;

  REQUIRE (graph.num_vertices() == 0);
  REQUIRE (graph.num_edges() == 0);

  for (size_t i = 0; i < cycle_size; i++) {
    graph.insert_vertex(i);

    REQUIRE (graph.num_vertices() == i+1);
  }

  for (size_t i = 0; i < cycle_size-1; i++) {
    REQUIRE (graph.num_edges() == i);

    graph.insert_directed_edge(weight, i, i+1);

    REQUIRE (graph.num_edges() == i+1);
  }

  graph.insert_directed_edge(weight, cycle_size-1, 0); // complete the cycle

  REQUIRE (graph.num_edges() == cycle_size);

  return graph;
}

Graph<std::size_t, double> construct_cycle_graph(size_t cycle_size, double weight) {
  Graph<std::size_t, double> graph;

  REQUIRE (graph.num_vertices() == 0);
  REQUIRE (graph.num_edges() == 0);

  for (size_t i = 0; i < cycle_size; i++) {
    graph.insert_vertex(i);

    REQUIRE (graph.num_vertices() == i+1);
  }

  for (size_t i = 0; i < cycle_size-1; i++) {
    REQUIRE (graph.num_edges() == i*2);

    graph.insert_edge(weight, i, i+1);

    REQUIRE (graph.num_edges() == (i+1)*2);
  }

  graph.insert_edge(weight, cycle_size-1, 0); // complete the cycle

  REQUIRE (graph.num_edges() == cycle_size*2);

  return graph;
}

TEST_CASE( "Cycle digraph is constructable", "[graph][construction][directed]" ) {
  size_t cycle_size = GENERATE(10, 100);
  double weight = GENERATE(0.0, 0.5, 1.0);
  construct_cycle_digraph(cycle_size, weight);
}

TEST_CASE( "Cycle graph is constructable with insert_edge()", "[graph][construction][undirected]" ) {
  size_t cycle_size = GENERATE(10, 100);
  double weight = GENERATE(0.0, 0.5, 1.0);
  construct_cycle_graph(cycle_size, weight);
}

// The cycle graph is vertex-transitive, so the pagerank value of each vertex
// should reflect this symmetry. 
TEST_CASE( "Cycle graph is vertex-transitive with pagerank", "[pagerank][sym_cycle_graph]" ) {
  size_t cycle_size = GENERATE(10, 100);
  double weight = GENERATE(0.0, 0.5, 1.0);
  auto graph = construct_cycle_graph(cycle_size, weight);

  graph.normalize();
  auto ranks = graph.rank(100, 0.85);

  auto first_rank = ranks[0];
  for (auto& [v, e]: ranks) {
    REQUIRE_SIMILAR (e, first_rank);
  }
}

// The directed cycle graph is also vertex-transitive, so the pagerank value of each vertex
// should also reflect this symmetry. 
TEST_CASE( "Cycle digraph is vertex-transitive with pagerank", "[pagerank][sym_cycle_digraph]" ) {
  size_t cycle_size = GENERATE(10, 100);
  double weight = GENERATE(0.0, 0.5, 1.0);
  auto graph = construct_cycle_digraph(cycle_size, weight);

  graph.normalize();
  auto ranks = graph.rank(100, 0.85);

  auto first_rank = ranks[0];
  for (auto& [v, e]: ranks) {
    REQUIRE_SIMILAR (e, first_rank);
  }
}
