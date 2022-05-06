### PageRank
We chose PageRank to identify voxel regions in the brain that are most "authoritative". As our correlations between voxel regions in the brain form an undirected graph, voxel regions with a high rank will correspond to regions of the brain that correlate highly with the activations of other regions of the brain

To perform PageRank, we first find the sum of weights on edges coming out of every vertex. This will be used to determine the portion of overall weight coming out of a vertex a particular edge holds. We also initialize the rank of every vertex to be 1 divided by the number of vertices (ranks start at equal value).

Next, we repeat the core update loop for a fixed number of iterations. The core update loop consists of accumulating the ranks of all vertices with edges with a particular vertex, divided by the total amount of weight coming out of each of those vertices. In other words, the rank of a particular vertex is updated to be the sum of the ranks of all adjacent vertices, multiplied by the fraction of those vertices' weight going into the vertex we are updating. Finally, we update all ranks by scaling down the calculated rank slightly and adding a damping factor.

To test for correctness, we took two approaches. First, we tested that the PageRank on small graphs matched exactly results that we computed by hand (the testing graphs in this section are small enough such that this was feasible). Second, we tested for various invariants of PageRank. For example, we test that graphs that are just large cycles have equally weighted ranks. That is, we ran PageRank on graphs where node 1 has an edge directed at node 2, node 2 has an edge directed at node 3, and so on, until the edge from node k to node 1 (k is the size of the graph). We tested PageRank on both unicycles and dicycles (directed vs undirected edges). In this scenario, an invariant of PageRank is that no matter the number of iterations run, the rank of each vertex should be 1 / k. We tested this for multiple sizes k. We also tested that the PageRank of a graph is not affected by normalization. Because the amount of "rank" that accumulates in a vertex depends on the fraction of weight on a particular edge, but not the magnitude of the edge weight, normalizing all the weights of all the edges coming out of all the vertices should have no affect on PageRank. We tested this to make sure our PageRank implementation respected this invariant.

### Girvan-Newman
We chose the Girvan-Newman algorithm for finding community structure. First, we used the Brandes algorithm to compute the edge betweenness centrality. Betweenness centrality for an edge refers to the proportion of shortest paths that goes through the edge. For each node, we use a BFS algorithm to find the shortest paths between the node and all the other nodes. The edge betweenness centrality score is then calculated by counting the number of times the edge appears in the path between the nodes divided by the total number of paths between the nodes. In other words, an edge with a high betweenness centrality score would mean that the edge is often passed through, thus having a higher influence.

Next, we computed the modularity score, which is often used as a measure of how good the clustering is. We used modularity score to optimize community detection. The modularity score is calculated by summing the probability that the edge is within a cluster minus the probability a random edge is within a certain cluster. High modularity indicates a larger number of edges within the group we expect by chance.

Using the Girvan-Newman algorithm, once we compute the edge betweenness centrality scores for all the edges, we remove the edge with the highest betweenness centrality. After each edge removal, we recompute the modularity score. We continue removing the edges, from the edges with the highest betweenness centrality, until we reach a modularity score of 0.3.

To test the correctness of the algorithms, first, we made sure that our algorithm calculates the modularity score and assigns the clusters correctly. We created a small graph and calculated the modularity score by hand and made sure that it matches the score calculated by our algorithm. We also checked to make sure that nodes within the same cluster have the same community membership value.

Next, to ensure that our Brandes algorithm is working correctly, we created a few smaller graphs to test our algorithm. We computed the betweenness centrality score manually and made sure that it matches the score calculated by the algorithm.

Lastly, we also tested our Girvan-Newman algorithm. The Girvan-Newman algorithm starts by calculating the modularity score, deletes the edge with the highest betweenness centrality score, and recalculates the modularity score. We manually identified the edge with the highest betweenness centrality score and made sure that it matches the edge removed by the Girvan-Newman algorithm.


## The answer to leading question

### PageRank
Based on our results from the PageRank algorithm, we managed to detect the brain regions that are important in both Imagery (i.e. imagining natural scenes) and Perception (i.e. viewing natural scenes) conditions. Interestingly, we found that the important brain areas are different between these two conditions. In the Imagery condition, the most important brain areas are located towards the back of the brain, whereas in the Perception condition, the most important brain areas are located at the middle part of the brain. 

