#include <unordered_map>
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
private:
    struct VertexInfo;
    struct EdgeInfo;
    
    std::unordered_map<V, VertexInfo> vertices;
    std::unordered_map<std::pair<V, V>, EdgeInfo, boost::hash<std::pair<V, V>>> edges;
    
    struct VertexInfo {
	unsigned deg;
	std::list<
	    std::tuple<
		EdgeSide,
		std::pair<V, V>,
		V
		>
	    > vertex_edges;
    };

    struct EdgeInfo {
	E e;
	typename decltype(VertexInfo::vertex_edges)::iterator first;
	typename decltype(VertexInfo::vertex_edges)::iterator second;
    };
};

template <typename V, typename E>
void Graph<V, E>::insert_vertex(const V& v) {
    vertices.emplace(v, {0, {}});
}

template <typename V, typename E>
void Graph<V, E>::insert_edge(const E& e, const V& v1, const V& v2) {
    edges.emplace({v1, v2}, {e, {}, {}});
    ++vertices.at(v1).deg;
    vertices.at(v1).vertex_edges.emplace_back(EdgeSide::FIRST, edges.at({v1, v2}), v1);
    ++vertices.at(v2).deg;
    vertices.at(v2).vertex_edges.emplace_back(EdgeSide::SECOND, edges.at({v1, v2}), v2);
    edges.at({v1, v2})->first = vertices.at(v1).vertex_edges.back();
    edges.at({v1, v2})->second = vertices.at(v2).vertex_edges.back();
}

template <typename V, typename E>
void Graph<V, E>::remove_vertex(const V& v) {
    for (auto it = vertices.at(v).vertex_edges.front(); it != vertices.at(v).vertex_edges.end(); ++it) {
	edges.erase(std::get<1>(*it));
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
