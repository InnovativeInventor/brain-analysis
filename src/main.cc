#include <iostream>

#include "brain_serde.h"
#include "graph.h"
#include "rank.h"
#include "girvan_newman.h"

int main(int argc, char** argv) {
    if (argc != 6) {
	std::cerr << "Usage: ./main <input voxels> <input edges> <output ranks> <output girvan-newman> <girvan-newman modularity threshold>" << "\n";
	return 1;
    }
    std::string input_voxels(argv[1]);
    std::string input_edges(argv[2]);
    std::string output_ranks(argv[3]);
    std::string output_girvan_newman(argv[4]);
    double girvan_newman_mod(std::stod(argv[5]));
    
    auto voxels = read_in_voxels(input_voxels);
    auto edges = read_in_edges(input_edges);

    std::cout << "Creating graph.\n";
    Graph<std::size_t, double> graph;

    for (Voxel voxel : voxels) {
	graph.insert_vertex(voxel.index);
    }

    for (Edge edge : edges) {
	graph.insert_edge(edge.e, voxels.at(edge.v1).index, voxels.at(edge.v2).index);
    }

    std::cout << "Running PageRank.\n";
    auto ranks = graph.rank(100, 0.85);
    write_out_ranks(output_ranks, ranks, voxels);

    std::cout << "Running Girvan-Newman.\n";
    graph.girvan_newman(girvan_newman_mod, true);
    write_out_graph(output_girvan_newman, graph, voxels);
    
    return 0;
}
