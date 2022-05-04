

## The output and correctness of each algorithm

### PageRank

### Girvan-Newman
We chose the Girvan-Newman algorithm for finding community structure. First, we used the Brandes algorithm to compute the edge betweenness centrality. Betweenness centrality for an edge refers to the proportion of shortest paths that goes through the edge. For each node, we use a BFS algorithm to find the shortest paths between the node and all the other nodes. The edge betweenness centrality score is then calculated by counting the number of times the edge appears in the path between the nodes divided by the total number of paths between the nodes. In other words, an edge with a high betweenness centrality score would mean that the edge is often passed through, thus having a higher influence.

Next, we also computed the modularity score, which is often used as a measure of how good the clustering is. We used modularity score to optimize community detection. The modularity score is calculated by summing the probability that the edge is within a cluster minus the probability a random edge is within a certain cluster. High modularity indicates a larger number of edges within the group we expect by chance.

Using the Girvan-Newman algorithm, once we compute the edge betweenness centrality scores for all the edges, we remove the edge with the highest betweenness centrality. After each edge removal, we recompute the modularity score. We continue removing the edges, from the edges with the highest betweenness centrality, until we reach a modularity score of 0.3.

To test the correctness of the algorithms, first, we made sure that our algorithm calculates the modularity score and assigns the clusters correctly. We created a small graph and calculated the modularity score by hand and made sure that it matches the score calculated by our algorithm. We also checked to make sure that nodes within the same cluster have the same community membership value.

Next, to ensure that our Brandes algorithm is working correctly, we created a few smaller graphs to test our algorithm. We computed the betweenness centrality score manually and made sure that it matches the score calculated by the algorithm.

Lastly, we also tested our Girvan-Newman algorithm. The Girvan-Newman algorithm starts by calculating the modularity score, deletes the edge with the highest betweenness centrality score, and recalculates the modularity score. We manually identified the edge with the highest betweenness centrality score and made sure that it matches the edge removed by the Girvan-Newman algorithm.


## The answer to leading question

