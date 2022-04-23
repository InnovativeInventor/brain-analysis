#pragma once

#include <unordered_map>
#include <functional>
#include <optional>
#include <utility>
#include <list>

struct custom_hash {
    template <typename V1, typename V2>
    std::size_t operator() (const std::pair<V1, V2>& pair) const {
	return std::hash<V1>()(pair.first) ^ std::hash<V2>()(pair.second);
    }
};

template <typename V, typename E>
class Graph {
public:
    void insert_vertex(const V&);
    void insert_edge(const E&, const V&, const V&);
    void insert_directed_edge(const E&, const V&, const V&);
    void remove_vertex(const V&);
    void remove_edge(const V&, const V&);
    unsigned degree(const V&);
    
    std::optional<E> get_edge(const V&, const V&);
    std::size_t num_vertices() { return vertices.size(); }
    std::size_t num_edges() { return edges.size(); }

    void map_vertices(std::function<void (V& v)> func);
    void map_edges(std::function<void (E& e)> func);

    void normalize();
    std::unordered_map<V, E> rank(int rounds, E damping);

private:
    struct VertexInfo;
    struct EdgeInfo;
    
    std::unordered_map<V, VertexInfo> vertices;
    std::unordered_map<std::pair<V, V>, EdgeInfo, custom_hash> edges;
    
    struct VertexInfo {
        unsigned deg;
	std::list<std::pair<V, V>> vertex_edges;
    };

    struct EdgeInfo {
	E e;
	typename std::list<std::pair<V, V>>::iterator first;
	typename std::list<std::pair<V, V>>::iterator second;
    };

    void normalize_edge_weights(const V& v);
};

template <typename V, typename E>
void Graph<V, E>::insert_vertex(const V& v) {
    vertices.insert({v, {0, {}}});
}

template <typename V, typename E>
void Graph<V, E>::insert_edge(const E& e, const V& v1, const V& v2) {
    insert_directed_edge(e, v1, v2);
    insert_directed_edge(e, v2, v1);
}

template <typename V, typename E>
void Graph<V, E>::insert_directed_edge(const E& e, const V& v1, const V& v2) {
    ++vertices.at(v1).deg;
    vertices.at(v1).vertex_edges.push_front({v1, v2});
    edges.insert({{v1, v2}, EdgeInfo{e, vertices.at(v1).vertex_edges.begin(), vertices.at(v2).vertex_edges.begin()}});
}

template <typename V, typename E>
void Graph<V, E>::remove_vertex(const V& v) {
    for (auto ve : vertices.at(v).vertex_edges) {
	edges.erase(ve.edge_label);
    }
    vertices.erase(v);
}

template <typename V, typename E>
void Graph<V, E>::remove_edge(const V& v1, const V& v2) {
    const auto& edge = edges.at({v1, v2});
    --vertices.at(v1).deg;
    vertices.at(v1).vertex_edges.erase(edge.first);
    --vertices.at(v2).deg;
    vertices.at(v2).vertex_edges.erase(edge.second);
    edges.erase({v1, v2});
}

template <typename V, typename E>
unsigned Graph<V, E>::degree(const V& v) {
    return vertices.at(v).deg;
}

template <typename V, typename E>
std::optional<E> Graph<V, E>::get_edge(const V& v1, const V& v2) {
    auto& to_check = vertices.at(v1).deg < vertices.at(v2).deg ? v1 : v2;
    for (auto ve : vertices.at(to_check).vertex_edges) {
	if (ve.edge_label == std::make_pair(v1, v2)) return edges.at({v1, v2}).e;
    }
    return {};
}

template <typename V, typename E>
void Graph<V, E>::map_vertices(std::function<void (V& v)> func) {
    for (auto& v : vertices) {
	func(v.first);
    }
}

template <typename V, typename E>
void Graph<V, E>::map_edges(std::function<void (E& e)> func) {
    for (auto& [_, e] : edges) {
	func(e.e);
    }
}
