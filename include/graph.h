#pragma once

#include <unordered_map>
#include <functional>
#include <optional>
#include <utility>
#include <list>

#include <iostream>
using std::cout;
using std::endl;

/*
 * Custom hash used so that we can use std::pair as a key for unordered_map.
 */
struct custom_hash {
    template <typename V1, typename V2>
    std::size_t operator() (const std::pair<V1, V2>& pair) const {
	return std::hash<V1>()(pair.first) ^ std::hash<V2>()(pair.second);
    }
};

/*
 * Graph class templatized over vertex and edge data. Edge weight *MUST* be a floating point value (usually double). We would use concepts but we ran into compiler issues on EWS.
 */
template <typename V, typename E>
class Graph {
public:
    /*
     * Core graph functions.
     */
    void insert_vertex(const V&);
    void insert_edge(const E&, const V&, const V&);
    void insert_directed_edge(const E&, const V&, const V&);
    void remove_directed_edge(const V&, const V&);
    unsigned degree(const V&) const;
    
    std::optional<E> get_edge(const V&, const V&) const;
    std::size_t num_vertices() const { return vertices.size(); }
    std::size_t num_edges() const { return edges.size(); }

    void map_vertices(std::function<void (V&)>);
    void map_edges(std::function<void (E&)>);

    void normalize();
    void normalize_edge_weights(const V& v);

    /*
     * Run PageRank on this graph. Non-destructive.
     */
    std::unordered_map<V, E> rank(int, E) const;

    /*
     * Associated functions for Girvan-Newman algorithm.
     */
    std::unordered_map<std::pair<V, V>, E, custom_hash> brandes();
    std::unordered_map<V, int> find_communities();
    E get_weighted_k(const V&);
    E modularity();
    void get_orig_m();

    /*
     * Run Girvan-Newman on this graph. Destructive (this graph object changes after calling this function);
     */
    void girvan_newman(E modularity_thres);

private:
    struct VertexInfo;
    struct EdgeInfo;
    
    /*
     * Adjacency list graph implementation.
     */
    std::unordered_map<V, VertexInfo> vertices;
    std::unordered_map<std::pair<V, V>, EdgeInfo, custom_hash> edges;
    
    /*
     * Contains degree and edge labels.
     */
    struct VertexInfo {
        unsigned deg;
	std::list<std::pair<V, V>> vertex_edges;
    };

    /*
     * Contains edge weight and iterators into vertices' lists corresponding to edge positions.
     */
    struct EdgeInfo {
	E e;
	typename std::list<std::pair<V, V>>::iterator first;
    };

    E orig_m; // number of edges in the original graph (weighted)
};

/*
 * Inserts a vertex into vertices map.
 */
template <typename V, typename E>
void Graph<V, E>::insert_vertex(const V& v) {
    vertices.insert({v, {0, {}}});
}

/*
 * Inserts an edge. This edge is undirected, so equivalent to inserting two directed edges in both directions.
 */
template <typename V, typename E>
void Graph<V, E>::insert_edge(const E& e, const V& v1, const V& v2) {
    insert_directed_edge(e, v1, v2);
    insert_directed_edge(e, v2, v1);
}

/*
 * Inserts a directed edge. Add edge to first vertex's list and add edge to edges map. Also increase degree of first vertex.
 */
template <typename V, typename E>
void Graph<V, E>::insert_directed_edge(const E& e, const V& v1, const V& v2) {
    ++vertices.at(v1).deg;
    vertices.at(v1).vertex_edges.push_front({v1, v2});
    edges.insert({{v1, v2}, EdgeInfo{e, vertices.at(v1).vertex_edges.begin()}});
}

/*
 * Removes a directed edge. Decrease degree of first vertex, remove edge from first vertex's list, and erase edge from edges map.
 */
template <typename V, typename E>
void Graph<V, E>::remove_directed_edge(const V& v1, const V& v2) {
    const auto& edge = edges.at({v1, v2});
    --vertices.at(v1).deg;
    vertices.at(v1).vertex_edges.erase(edge.first);
    edges.erase({v1, v2});
}

/*
 * Get degree.
 */
template <typename V, typename E>
unsigned Graph<V, E>::degree(const V& v) const {
    return vertices.at(v).deg;
}

/*
 * Get an edge weight. Returned in optional because edge might not exist.
 */
template <typename V, typename E>
std::optional<E> Graph<V, E>::get_edge(const V& v1, const V& v2) const {
    if (edges.find({v1, v2}) != edges.end())
	return edges.at({v1, v2}).e;
    else
	return {};
}

/*
 * Map function over all vertices.
 */
template <typename V, typename E>
void Graph<V, E>::map_vertices(std::function<void (V& v)> func) {
    for (auto& v : vertices) {
	func(v.first);
    }
}

/*
 * Map function over all edges.
 */
template <typename V, typename E>
void Graph<V, E>::map_edges(std::function<void (E& e)> func) {
    for (auto& [_, e] : edges) {
	func(e.e);
    }
}
