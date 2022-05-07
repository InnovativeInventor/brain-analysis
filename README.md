## Neuroimaging Analysis
See [/documents/proposal.md](/documents/proposal.md) for the leading question and [/data/data_preprc/README_preproc.md](/data/data_preproc/README_preproc.md) for specific details on how data were preprocessed. In short, we've used correlation data between activations of different brain regions to analyze the brain activity of individuals who are viewing images vs individuals who are imagining the same set of images.

## Directory Structure
The C++ code is located in the ``src``, ``include``, and ``tests`` directories. The graph and algorithm code is in ``src`` and ``include``, and the testing code is in the ``tests`` directory. Our pre-processed input data is in the ``data`` directory. The pre-processed input data are the ``coords.txt``, ``img_edge_list.txt``, and ``pcp_edge_list.txt`` files. These files represent the voxel regions and coordinates in the brain, the imagination correlations between voxels, and the perception correlations between voxels, respectively. ``coords.txt`` forms a common set of vertices across both cases, and the edge lists represent the case-specific edge weights. Most documents are located in the ```documents``` folder. This includes the report, which is at [/documents/results.md](/documents/results.md).

## Visualizations
Here is a link to our video presentation on Google Drive: [https://drive.google.com/file/d/1Kw3xhOIBkqIjx52e55vL-cNjkgboE2CC/view?usp=sharing](https://drive.google.com/file/d/1Kw3xhOIBkqIjx52e55vL-cNjkgboE2CC/view?usp=sharing).
Here is a link to our video presentation on Mediaspace:
[https://mediaspace.illinois.edu/media/t/1_0dss4z2s](https://mediaspace.illinois.edu/media/t/1_0dss4z2s).
The visualizations html files can be reproduce by directly running [/src/visualization.py](/src/visualization.py), although there are 4 public packages required: nilearn, sklearn, numpy and matplotlib, all of which can be installed using pip.

## Running
To build the main executable, run:
```
make
```

To run the main executable on the input data with the same parameters as we did on real-world data, run:
```
make exe
```

To run the main executable on custom data, run the following:
```
./main <input voxels> <input edges> <output original graph> <output ranks> <output girvan-newman> <girvan-newman modularity threshold>
```
where the following describe the arguments:
- input voxels: input list of voxel regions in brain
- input edges: input list of edges corresponding to correlations between voxel regions in the brain (undirected)
- output original graph: for visualization purposes, we output the initially constructed graph as an adjacency matrix; this file is where that output adjacency matrix is written to
- output ranks: output file where PageRank results are written to
- output girvan-newman: output file where Girvan-Newman results are written to
- girvan-newman modularity threshold: threshold at which to stop Girvan-Newman calculations (0.3-0.8 is what's used in literature, should be between 0 and 1)

Running `make exe` to reproduce our results is [fully equivalent](https://github-dev.cs.illinois.edu/cs225-sp22/llchong2-zhenans2-rarbore2-myfan3/blob/d1b58a97e88690fd942f8e15c05f10595cddb93f/Makefile#L33-L35) to running `./main` with the following arguments:
```
./main data/coords.txt data/img_edge_list.txt data/results/img_orig.txt data/results/img_ranks.txt data/results/img_girvan_newman.txt 0.3
./main data/coords.txt data/pcp_edge_list.txt data/results/pcp_orig.txt data/results/pcp_ranks.txt data/results/pcp_girvan_newman.txt 0.3
```

## Testing
To run our test suite, run:
```
make exe_test
```

Alternatively, you can run:
```
make test
./test
```

We test the following:
- Serialization / deserialization code (deserialize input voxels / edges, serialize output ranks / graph)
- Graph code (all graph functions in our adjacency list implementation)
- PageRank code (manually constructed examples that we've calculated the results of by hand, also test invariants on non-specificly sized graphs)
- Girvan-Newman code (test helper functions for Girvan-Newman, test main Girvan-Newman function, all with manually constructed examples)

We use the catch2 testing framework, and the following are tags that you can test specifically:
- io
- graph
- construction
- pagerank
- girvan_newman
