#include <unordered_map>
#include <fstream>
#include <string>
#include <vector>

struct Voxel {
    float x;
    float y;
    float z;
};

std::unordered_map<std::string, std::vector<Voxel>> read_in_voxels(const std::string& file_name);
