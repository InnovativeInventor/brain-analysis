#include <iostream>

#include "brain_serde.h"
#include "graph.h"

int main(int argc, char** argv) {
    auto voxels = read_in_voxels("data/coords.txt");
    auto edges = read_in_edges("data/img_edge_list.txt");
    std::cout << edges.size() << " " << edges.at(0).v1 << " " << edges.at(0).v2 << " " << edges.at(0).e << "\n";

    return 0;
}
