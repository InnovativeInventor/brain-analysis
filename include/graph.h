#include <vector>
#include <list>

enum class EdgeSide {
    FIRST, SECOND
};

template <typename V, typename E>
class Graph {
public:
    void insert_vertex(const V&);
    void insert_edge(const E&, std::size_t v1, std::size_t v2);
private:
    struct EdgeInfo;
    
    struct VertexInfo {
	V v;
	unsigned deg;
	std::list<
	    std::tuple<
		EdgeSide,
		typename std::list<EdgeInfo>::iterator,
		typename std::vector<VertexInfo>::iterator
		>
	    > edges;
    };

    struct EdgeInfo {
	E e;
	typename decltype(VertexInfo::edge)::iterator first;
	typename decltype(VertexInfo::edge)::iterator second;
    };
    
    std::vector<VertexInfo> vertices;
    std::list<EdgeInfo> edges;
};

template <typename V, typename E>
void Graph<V, E>::insert_vertex(const V& v) {
    vertices.emplace_back(v, 0, {});
}

template <typename V, typename E>
void Graph<V, E>::insert_edge(const E& e, std::size_t v1, std::size_t v2) {
    edges.emplace_back(e, vertices.begin() + v1, vertices.begin() + v2);
    ++vertices.at(v1).deg;
    vertices.at(v1).edges.emplace_back(EdgeSide::FIRST, edges.back(), vertices.begin() + v1);
    ++vertices.at(v2).deg;
    vertices.at(v2).edges.emplace_back(EdgeSide::SECOND, edges.back(), vertices.begin() + v2);
}
