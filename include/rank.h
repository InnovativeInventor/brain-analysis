#pragma once

#include "graph.h"

template <typename V, typename E>
void Graph<V, E>::normalize() {
    for (auto& v : vertices) {
	normalize_edge_weights(v.first);
    }
}

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

template <typename V, typename E> // E must be floating point
std::unordered_map<V, E> Graph<V, E>::rank(int rounds, E damping) {
    size_t size = vertices.size();
    std::unordered_map<V, E> ranks, sums;
    for (auto [vertex, vertex_info] : vertices) {
	ranks.insert({vertex, 1.0 / static_cast<E>(size)});

	auto sum = E();
	for (auto ve : vertex_info.vertex_edges) {
	    auto& edge = edges.at({ve.first, ve.second});
	    sum += edge.e;
	}
	sums.insert({vertex, sum});
    }

    for (int i = 0; i < rounds; ++i) {
	for (auto& [vertex, vertex_info] : vertices) {
	    for (auto ve : vertex_info.vertex_edges) {
	        auto& edge = edges.at({ve.first, ve.second});
		ranks.at(ve.second) = (1.0 - damping) + damping * (ranks.at(ve.first) * edge.e / sums.at(vertex));
	    }
	}
    }

    return ranks;
}
