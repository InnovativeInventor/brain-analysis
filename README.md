## Neuroimaging Analysis
See [/documents/proposal.md](/documents/proposal.md) for the leading question and [/data/data_preprc/README_preproc.md](/data/data_preproc/README_preproc.md) for specific details on how data were preprocessed. In short, we've used correlation data between activations of different brain regions to analyze the brain activity of individuals who are viewing images vs individuals who are imagining the same set of images.

## Directory Structure
The C++ code is located in the ``src``, ``include``, and ``tests`` directories. The graph and algorithm code is in ``src`` and ``include``, and the testing code is in the ``tests`` directory. Our pre-processed input data is in the ``data`` directory. The pre-processed input data are the ``coords.txt``, ``img_edge_list.txt``, and ``pcp_edge_list.txt`` files. These files represent the voxel regions and coordinates in the brain, the imagination correlations between voxels, and the perception correlations between voxels, respectively. ``coords.txt`` forms a common set of vertices across both cases, and the edge lists represent the case-specific edge weights. Most documents are located in the ```documents``` folder. This includes the report, which is at [/documents/results.md](/documents/results.md).

## Running
To build the main executable, simply run:
```
make
```

To run the main executable on the input data with the same parameters as we did on real-world data, simple run:
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

## Testing
To run our test suite, simply run:
```
make exe_test
```

Alternatively, you can run:
```
make test
./test
```

## Analysis Workflow
TODO: fill this out
