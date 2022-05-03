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
std::unordered_map<V, E> Graph<V, E>::rank(int rounds, E damping) const {
    size_t size = vertices.size();
    std::unordered_map<V, E> ranks_1, ranks_2, sums;
    for (auto [vertex, vertex_info] : vertices) {
	ranks_1.insert({vertex, static_cast<E>(1.0) / static_cast<E>(size)});
	ranks_2.insert({vertex, static_cast<E>(0.0)});

	auto sum = E();
	for (auto ve : vertex_info.vertex_edges) {
	    auto& edge = edges.at({ve.first, ve.second});
	    sum += edge.e;
	}
	sums.insert({vertex, sum});
    }

    for (int i = 0; i < rounds; ++i) {
	for (auto& [_, r] : ranks_2) {
	    r = 0.0;
	}
	for (auto& [vertex, vertex_info] : vertices) {
	    for (auto ve : vertex_info.vertex_edges) {
	        auto& edge = edges.at({ve.first, ve.second});
		ranks_2.at(ve.second) += ranks_1.at(ve.first) * edge.e / sums.at(vertex);
	    }
	}
	for (auto& [_, r] : ranks_2) {
	    r = damping * r + 1.0 - damping;
	}
	std::swap(ranks_1, ranks_2);
    }

    return ranks_1;
}
