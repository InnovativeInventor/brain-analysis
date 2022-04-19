#include <iostream>

#include "brain_serde.h"
#include "graph.h"
#include "rank.h"

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

    for (Voxel voxel : voxels) {
        graph.normalize_edge_weights(voxel.index);
    }

    std::cout << graph.num_vertices() << " " << graph.num_edges() << "\n";

    return 0;
}
