#include <iostream>

#include "brain_serde.h"
#include "graph.h"

int main(int argc, char** argv) {
    auto voxels = read_in_voxels("data/coords.txt");

    std::cout << voxels.size() << "\n";
    for (auto [re, vs] : voxels) {
	std::cout << re << " " << vs.size() << "\n";
    }

    return 0;
}
