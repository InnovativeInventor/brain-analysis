#include <unordered_map>
#include <list>

template <typename V, typename E>
class Graph {
public:
    void insert_vertex(const V&);
    void insert_edge(const E&, const V&, const V&);
private:
    struct EdgeInfo;
    
    struct VertexInfo {
	unsigned deg;
	std::list<
	    std::pair<
		typename std::list<EdgeInfo>::iterator,
		typename std::unordered_map<V, VertexInfo>::iterator
		>
	    > edge;
    };

    struct EdgeInfo {
	E e;
	typename decltype(VertexInfo::edge)::iterator first;
	typename decltype(VertexInfo::edge)::iterator second;
    };
    
    std::unordered_map<V, VertexInfo> vertices;
    std::list<EdgeInfo> edges;
};
