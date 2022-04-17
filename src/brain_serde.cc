#include "brain_serde.h"

std::unordered_map<std::string, std::vector<Voxel>> read_in_voxels(const std::string& file_name) {
    std::fstream fs(file_name, std::ios::in);
    std::unordered_map<std::string, std::vector<Voxel>> voxels;

    std::string region;
    std::size_t num_voxels = 0;
    while (fs) {
	std::string line;
	if (!getline(fs, line)) break;
	if (num_voxels) {
	    std::size_t comma1 = line.find(',');
	    std::size_t comma2 = line.find(',', comma1 + 1);
	    voxels[region].push_back(Voxel{
		    std::stof(line.substr(0, comma1)),
		    std::stof(line.substr(comma1 + 1, comma2 - comma1 - 1)),
		    std::stof(line.substr(comma2 + 2, std::string::npos))
		});
	    --num_voxels;
	}
	else {
	    region = line.substr(0, line.find(','));
	    num_voxels = static_cast<std::size_t>(std::stoi(line.substr(line.find(',') + 2, std::string::npos)));
	}
    }
    
    return voxels;
}
