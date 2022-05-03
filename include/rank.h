#pragma once

#include "graph.h"

/*
 * Normalize all edge weights on all vertices. Used to test invariant of PageRank.
 */
template <typename V, typename E>
void Graph<V, E>::normalize() {
    for (auto& v : vertices) {
	normalize_edge_weights(v.first);
    }
}

/*
 * Normalize all edge weights coming from a particular vertex.
 */
template <typename V, typename E>
void Graph<V, E>::normalize_edge_weights(const V& v) {
    auto sum = E();

    for (auto ve : vertices.at(v).vertex_edges) {
	auto& edge = edges.at({ve.first, ve.second});
	sum += edge.e;
    }

    for (auto ve : vertices.at(v).vertex_edges) {
	auto& edge = edges.at({ve.first, ve.second});
	edge.e /= sum;
    }
}

/*
 * PageRank algorithm. Runs algorithm for a specified number of rounds. Returns a rank for each vertex in the graph (map from vertex to rank).
 */
template <typename V, typename E>
std::unordered_map<V, E> Graph<V, E>::rank(int rounds, E damping) const {
    size_t size = vertices.size();
    // We have 2 ranks maps, as while performing one iteration, we don't want ranks we've changed in the current iteration to affect other calculations within the same iteration.
    // "sums" map stores the sum of edge weights coming out of a vertex, used later to determine proportion going through any one edge.
    std::unordered_map<V, E> ranks_1, ranks_2, sums;
    for (auto [vertex, vertex_info] : vertices) {
	ranks_1.insert({vertex, static_cast<E>(1.0) / static_cast<E>(size)}); // Ranks start equally proportioned amongst vertices.
	ranks_2.insert({vertex, static_cast<E>(0.0)});

	auto sum = E();
	for (auto ve : vertex_info.vertex_edges) {
	    auto& edge = edges.at({ve.first, ve.second});
	    sum += edge.e;
	}
	sums.insert({vertex, sum});
    }

    // Run algorithm for "rounds" iterations.
    for (int i = 0; i < rounds; ++i) {
	// Zero out destination map.
	for (auto& [_, r] : ranks_2) {
	    r = 0.0;
	}

	// Peform core PageRank calculation.
	for (auto& [vertex, vertex_info] : vertices) {
	    for (auto ve : vertex_info.vertex_edges) {
	        auto& edge = edges.at({ve.first, ve.second});
		ranks_2.at(ve.second) += ranks_1.at(ve.first) * edge.e / sums.at(vertex);
	    }
	}

	// Add damping.
	for (auto& [_, r] : ranks_2) {
	    r = damping * r + 1.0 - damping;
	}

	// Swap maps for next iteration (ranks_2 is always our working map, ranks_1 is previous iterations finished map).
	std::swap(ranks_1, ranks_2);
    }

    return ranks_1;
}
