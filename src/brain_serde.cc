#include "brain_serde.h"

/*
 * Read in voxels from voxels file.
 */
std::vector<Voxel> read_in_voxels(const std::string& file_name) {
    std::fstream fs(file_name, std::ios::in);
    std::vector<Voxel> voxels;

    // File consists of different named regions, so keep track of current region.
    std::string region;
    std::size_t region_voxels = 0;
    std::size_t num_voxels = 0;
    std::string line;

    // Iterate over all lines in file.
    while (fs) {
	if (!getline(fs, line)) break;
	if (region_voxels) { // Check if we have voxels left to process. If so, then we read in voxel data. If not, we read in the next region info.
	    std::size_t comma1 = line.find(',');
	    std::size_t comma2 = line.find(',', comma1 + 1);

	    // Parse line containing voxel coordinates.
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

/*
 * Read in edges from edges file.
 */
std::vector<Edge> read_in_edges(const std::string& file_name) {
    std::fstream fs(file_name, std::ios::in);
    std::vector<Edge> edges;

    std::string line;
    getline(fs, line); // Ignore first line (header).

    // Iterate over all lines in file.
    while (fs) {
	if (!getline(fs, line)) break;
	std::size_t comma1 = line.find(',');
	std::size_t comma2 = line.find(',', comma1 + 1);

	// Read in one edge per line.
	edges.push_back(Edge{
		static_cast<std::size_t>(std::stoi(line.substr(0, comma1))),
		static_cast<std::size_t>(std::stoi(line.substr(comma1 + 1, comma2 - comma1 - 1))),
		std::stod(line.substr(comma2 + 1, std::string::npos))
	    });
    }

    return edges;
}

/*
 * Write out graph in adjacency matrix form for visualization. Non-existant edges have weight 0.0, as there will be not be significant correlation between those voxels.
 */
void write_out_graph(const std::string& file_name, const Graph<std::size_t, double>& graph, const std::vector<Voxel>& voxels) {
    std::fstream fs(file_name, std::ios::out);

    for (Voxel v1 : voxels) {
	for (Voxel v2 : voxels) {
	    fs << std::to_string(graph.get_edge(v1.index, v2.index).value_or(0.0)) << " ";
	}
	fs << "\n";
    }
}

/*
 * Write out ranks for each voxel for visualization.
 */
void write_out_ranks(const std::string& file_name, const std::unordered_map<std::size_t, double>& ranks, const std::vector<Voxel>& voxels) {
    std::fstream fs(file_name, std::ios::out);

    for (Voxel v : voxels) {
	fs << std::to_string(ranks.at(v.index)) << " ";
    }
}
