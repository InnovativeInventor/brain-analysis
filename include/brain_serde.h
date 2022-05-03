#pragma once
    
#include <fstream>
#include <string>
#include <vector>

#include "graph.h"

// All the information needed to describe a voxel region of the brain.
struct Voxel {
    std::size_t index;
    std::string region;
    double x;
    double y;
    double z;
};

inline bool operator==(const Voxel& v1, const Voxel& v2) {
    return v1.index == v2.index
	&& v1.region == v2.region
	&& v1.x == v2.x
	&& v1.y == v2.y
	&& v1.z == v2.z;
}

// All the information needed to describe the correlation between two voxels.
struct Edge {
    std::size_t v1;
    std::size_t v2;
    double e;
};

inline bool operator==(const Edge& e1, const Edge& e2) {
    return e1.v1 == e2.v1
	&& e1.v2 == e2.v2
	&& e1.e == e2.e;
}

std::vector<Voxel> read_in_voxels(const std::string& file_name);

std::vector<Edge> read_in_edges(const std::string& file_name);

void write_out_graph(const std::string& file_name, const Graph<std::size_t, double>& graph, const std::vector<Voxel>& voxels);
