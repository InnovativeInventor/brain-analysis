#include "graph.h"

template <typename V, typename E> requires std::integral<E>
void Graph<V, E>::normalize_edge_weights(const V& v) {
    auto sum = E();

    for (auto ve : vertices.at(v).vertex_edges) {
	auto& edge = edges.at({ve.edge_label.first, ve.edge_label.second});
	sum += edge.e;
    }

    for (auto ve : vertices.at(v).vertex_edges) {
	auto& edge = edges.at({ve.edge_label.first, ve.edge_label.second});
	edge.e /= sum;
    }
}
