#include <fstream>
#include <string>
#include <vector>

struct Voxel {
    std::size_t index;
    std::string region;
    double x;
    double y;
    double z;
};

struct Edge {
    std::size_t v1;
    std::size_t v2;
    double e;
};

std::vector<Voxel> read_in_voxels(const std::string& file_name);

std::vector<Edge> read_in_edges(const std::string& file_name);
