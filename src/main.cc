#include <iostream>

#include "brain_serde.h"
#include "graph.h"
#include "rank.h"
#include "girvan_newman.h"

int main(int argc, char** argv) {
    auto voxels = read_in_voxels("data/coords.txt");
    auto edges = read_in_edges("data/img_edge_list.txt");

    Graph<std::size_t, double> graph;

    for (Voxel voxel : voxels) {
	graph.insert_vertex(voxel.index);
    }

    for (Edge edge : edges) {
	graph.insert_edge(edge.e, voxels.at(edge.v1).index, voxels.at(edge.v2).index);
    }

    auto ranks = graph.rank(100, 0.85);
    
    write_out_graph("data/test_graph.txt", graph, voxels);
    write_out_ranks("data/test_ranks.txt", ranks, voxels);

    //for (auto& [v, e]: ranks) {
    //    std::cout << v << " " << e << " " << graph.degree(v) << std::endl;
    //}

    std::cout << graph.num_vertices() << " " << graph.num_edges() << "\n";

    graph.girvan_newman(0.3);
    
    return 0;
}
