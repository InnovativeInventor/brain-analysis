#include "brain_serde.h"

std::vector<Voxel> read_in_voxels(const std::string& file_name) {
    std::fstream fs(file_name, std::ios::in);
    std::vector<Voxel> voxels;

    std::string region;
    std::size_t region_voxels = 0;
    std::size_t num_voxels = 0;
    std::string line;
    while (fs) {
	if (!getline(fs, line)) break;
	if (region_voxels) {
	    std::size_t comma1 = line.find(',');
	    std::size_t comma2 = line.find(',', comma1 + 1);
	    voxels.push_back(Voxel{
		    num_voxels,
		    region,
		    std::stod(line.substr(0, comma1)),
		    std::stod(line.substr(comma1 + 1, comma2 - comma1 - 1)),
		    std::stod(line.substr(comma2 + 1, std::string::npos))
		});
	    --region_voxels;
	    ++num_voxels;
	}
	else {
	    region = line.substr(0, line.find(','));
	    region_voxels = static_cast<std::size_t>(std::stoi(line.substr(line.find(',') + 2, std::string::npos)));
	}
    }
    
    return voxels;
}

std::vector<Edge> read_in_edges(const std::string& file_name) {
    std::fstream fs(file_name, std::ios::in);
    std::vector<Edge> edges;

    std::string line;
    getline(fs, line);
    while (fs) {
	if (!getline(fs, line)) break;
	std::size_t comma1 = line.find(',');
	std::size_t comma2 = line.find(',', comma1 + 1);
	edges.push_back(Edge{
		static_cast<std::size_t>(std::stoi(line.substr(0, comma1))),
		static_cast<std::size_t>(std::stoi(line.substr(comma1 + 1, comma2 - comma1 - 1))),
		std::stod(line.substr(comma2 + 1, std::string::npos))
	    });
    }

    return edges;
}
