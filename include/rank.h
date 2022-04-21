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
    std::unordered_map<V, E> ranks;
    for (auto [vertex, _] : vertices)
	ranks.insert({vertex, 1.0 / static_cast<E>(size)});

    /*
    for (int i = 0; i < rounds; i++) {
	for (size_t v = 0; v < ranks.size(); v++) {
	    auto sum = E();
            for (auto ve : vertices.at(v).vertex_edges) {
	        auto& edge = edges.at({ve.edge_label.first, ve.edge_label.second});
		sum += edge.e*ranks[v];
	    }
	    ranks[v] += sum;
	}
    }
    */
    for (int i = 0; i < rounds; ++i) {
	for (auto& [_, vertex_info] : vertices) {
	    auto sum = E();
	    for (auto ve : vertex_info.vertex_edges) {
	        auto& edge = edges.at({ve.first, ve.second});
		sum += edge.e;
	    }
	    for (auto ve : vertex_info.vertex_edges) {
	        auto& edge = edges.at({ve.first, ve.second});
		ranks.at(ve.second) += edge.e / sum;
	    }
	}
	for (auto& [_, e] : ranks) {
	    e *= damping;
	}
    }

    // TODO: finish impl, with damping
    return ranks;
}
