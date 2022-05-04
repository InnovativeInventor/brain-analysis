## Neuroimaging Analysis
See [/documents/proposal.md](/documents/proposal.md) for the leading question. In short, we've used correlation data between activations of different brain regions to analyze the brain activity of individuals who are viewing images vs individuals who are imagining images.

## Directory Structure
The C++ code is located in the ``src``, ``include``, and ``tests`` directories. The graph and algorithm code is in ``src`` and ``include``, and the testing code is in the ``tests`` directory. Our pre-processed input data is in the ``data`` directory. The pre-processed input data are the ``coords.txt``, ``img_edge_list.txt``, and ``pcp_edge_list.txt`` files. These files represent the voxel regions and coordinates in the brain, the imagination correlations between voxels, and the perception correlations between voxels, respectively. ``coords.txt`` forms a common set of vertices across both cases, and the edge lists represent the case-specific edge weights.

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
