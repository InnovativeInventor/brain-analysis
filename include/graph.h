#include <unordered_map>
#include <list>

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
    struct EdgeInfo;
    
    struct VertexInfo {
	unsigned deg;
	std::list<
	    std::tuple<
		EdgeSide,
		typename std::list<EdgeInfo>::iterator,
		V
		>
	    > edges;
    };

    struct EdgeInfo {
	E e;
	typename decltype(VertexInfo::edges)::iterator first;
	typename decltype(VertexInfo::edges)::iterator second;
    };
    
    std::unordered_map<V, VertexInfo> vertices;
    std::list<EdgeInfo> edges;
};

template <typename V, typename E>
void Graph<V, E>::insert_vertex(const V& v) {
    vertices.emplace(v, {0, {}});
}

template <typename V, typename E>
void Graph<V, E>::insert_edge(const E& e, const V& v1, const V& v2) {
    edges.emplace_back(e, {}, {});
    ++vertices.at(v1).deg;
    vertices.at(v1).edges.emplace_back(EdgeSide::FIRST, edges.back(), v1);
    ++vertices.at(v2).deg;
    vertices.at(v2).edges.emplace_back(EdgeSide::SECOND, edges.back(), v2);
    edges.back()->first = vertices.at(v1).edges.back();
    edges.back()->second = vertices.at(v2).edges.back();
}

template <typename V, typename E>
void Graph<V, E>::remove_vertex(const V& v) {
    for (auto it = vertices.at(v).edges.front(); it != vertices.at(v).edges.end(); ++it) {
	edges.erase(std::get<1>(*it));
    }
    vertices.erase(v);
}

template <typename V, typename E>
void Graph<V, E>::remove_edge(const V&, const V&) {

}
