#include <unordered_map>
#include <optional>
#include <list>

#include <boost/functional/hash.hpp>

enum class EdgeSide {
    FIRST, SECOND
};

template <typename V, typename E>
class Graph {
public:
    void insert_vertex(const V&);
    void insert_edge(const E&, const V&, const V&);
    void remove_vertex(const V&);
    void remove_edge(const V&, const V&);
    std::optional<E> are_adjacent(const V&, const V&);
private:
    struct VertexInfo;
    struct EdgeInfo;
    
    std::unordered_map<V, VertexInfo> vertices;
    std::unordered_map<std::pair<V, V>, EdgeInfo, boost::hash<std::pair<V, V>>> edges;

    struct VertexInfoEdgeMember {
	EdgeSide es;
	std::pair<V, V> edge_label;
	V v;
    };
    
    struct VertexInfo {
        unsigned deg;
	std::list<VertexInfoEdgeMember> vertex_edges;
    };

    struct EdgeInfo {
	E e;
	typename std::list<VertexInfoEdgeMember>::iterator first;
	typename std::list<VertexInfoEdgeMember>::iterator second;
    };
};

template <typename V, typename E>
void Graph<V, E>::insert_vertex(const V& v) {
    vertices.insert({v, {0, {}}});
}

template <typename V, typename E>
void Graph<V, E>::insert_edge(const E& e, const V& v1, const V& v2) {
    ++vertices.at(v1).deg;
    vertices.at(v1).vertex_edges.push_front(VertexInfoEdgeMember{EdgeSide::FIRST, {v1, v2}, v1});
    ++vertices.at(v2).deg;
    vertices.at(v2).vertex_edges.push_front(VertexInfoEdgeMember{EdgeSide::SECOND, {v1, v2}, v2});
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
    vertices.at(v1).vertex_edges.erase(edge->first);
    --vertices.at(v2).deg;
    vertices.at(v2).vertex_edges.erase(edge->second);
    edges.erase({v1, v2});
}

template <typename V, typename E>
std::optional<E> Graph<V, E>::are_adjacent(const V& v1, const V& v2) {
    auto& to_check = vertices.at(v1).deg < vertices.at(v2).deg ? v1 : v2;
    for (auto ve : vertices.at(to_check).vertex_edges) {
	if (ve.edge_label == std::make_pair(v1, v2)) return edges.at({v1, v2}).e;
    }
    return {};
}
