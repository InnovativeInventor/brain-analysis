## Introduction (Brief Summary)

We are interested in how the human reacts differently when we are imagining natural scene images versus viewing the actual images themselves. The brain activity dataset was obtained from a public OpenNeuro database (Shen, 2018). We extracted the connectivity between different regions of the brain for the 2 conditions: imagining pictures vs. viewing images, by calculating the Pearson's R correlation for the activity of these regions as time series. The brain regions that we are interested in forms the human scene network that was proposed to encode the concept as well as spatial informations embedded in natural scenes (Baldassano, 2016). PPA is the main scene processing region largely for categories. OPA is responsible for low-level spatial information for the feeling of a space in the scene image. IPS is a more complex and integrative region that pools information together. RSC is more responsible for location or navigational information, and mostly interestingly, the hippocampus or the memory region that encodes these scene images into our memory bank ultimately.


<img width="302" alt="Screen Shot 2022-05-05 at 11 07 09 PM" src="https://user-images.githubusercontent.com/70292345/167065308-aade18e6-602d-45c8-8b5b-a86c404eedb3.png">


We obtained 2 graphs corresponding to brain's reaction under the 2 conditions and performed 2 algorithms, PageRank and Girvan-Newman on both to make a comparison. Based on the known property of the general structure of the brain that more anterior (in the front) part are usually responsible for more abstract and higher level of processing, we hypothesize that we may see higher importance and stronger community structure in the anterior regions when people are imagining pictures compared to when they are viewing the same set of pictures. More generally, our question concerns how the human brain reacts differently for imagining versus perceiving the same scene image and we will be examining the difference in terms of conenctivity within the human scene network.

## Methods (Algorithms Used)

### PageRank

We chose PageRank to identify voxel regions in the brain that are most "authoritative". As our correlations between voxel regions in the brain form an undirected graph, voxel regions with a high rank will correspond to regions of the brain that correlate highly with the activations of other regions of the brain

To perform PageRank, we first find the sum of weights on edges coming out of every vertex. This will be used to determine the portion of overall weight coming out of a vertex a particular edge holds. We also initialize the rank of every vertex to be 1 divided by the number of vertices (ranks start at equal value).

Next, we repeat the core update loop for a fixed number of iterations. The core update loop consists of accumulating the ranks of all vertices with edges with a particular vertex, divided by the total amount of weight coming out of each of those vertices. In other words, the rank of a particular vertex is updated to be the sum of the ranks of all adjacent vertices, multiplied by the fraction of those vertices' weight going into the vertex we are updating. Finally, we update all ranks by scaling down the calculated rank slightly and adding a damping factor.

To test for correctness, we took two approaches. First, we tested that the PageRank on small graphs matched exactly results that we computed by hand (the testing graphs in this section are small enough such that this was feasible).

Secondly, we tested for various invariants of PageRank. For example, we tested that PageRank preserves symmetries in various vertex-transitive graphs (such as the cycle graph). That is, we ran PageRank on cycle graphs of various sizes and checked if all the nodes had equal rank after multiple rounds. We tested PageRank on multiple types of cycle graphs, such as unicycles and dicycles (directed vs undirected edges). In this scenario, an invariant of PageRank is that no matter the number of iterations run, the rank of each vertex should be 1 / k. We tested this for multiple sizes k. We also tested that the PageRank of a graph is not affected by normalization. Because the amount of "rank" that accumulates in a vertex depends on the fraction of weight on a particular edge, but not the magnitude of the edge weight, normalizing all the weights of all the edges coming out of all the vertices should have no affect on PageRank. We tested this to make sure our PageRank implementation respected this invariant. These invaraints allowed us to be confident that our PageRank implemnatation was not only correct for small graphs that we could manually check, but also for larger graphs that had invariants that we knew to look for.

### Girvan-Newman

We chose the Girvan-Newman algorithm for finding community structure (Newman & Girvan, 2004). The girvan-newman algorithm partitions the graph into different communities by finding the edges that are most likely to lie in between communities and removing them. The betweenness centrality of each edge is used to determine which edge to be removed and the process is repeated until teh graph partitioning is satisfying as quantified by the modularity quality function. The algorithm can be summarized as:

1. Calculate betweenness centrality for all edges;
2. Remove the edge with highest betweenness centrality score;
3. Calculate the graph's modularity score;
4. If modularity reached the pre-specified satisfying level then stop, else repeat from step 1.

The edge betweenness centrality scores were calculated using the Brandes algorithm (Brandes, 2001). The original algorithm was tailored for calculating vertex betweenness centrality scores for undirected and unweighted graph, so we added variants to calculate for edges in a weighted and undirected graph (Brandes, 2008). The weights in the modification suggested for valued graph indicated the distance between vertices, but the weights of the egges in our graph represents correlations, or how strongly connected each pair of vertex is, so we transformed weights by taking (1-w) to match the algorithm. This tranformation choice aligns with the use of Pearson's R correlation for edges weights, so our original weights can only be between 0 and 1. The modification used to accomodate the calculation of betweenness centrality for edges as opposed to vertices is simply to maintain the list of centralities for edges rather than vertices.

To calculate betweenness centrality scores, we not only need to obtain all pair-wise shortest paths, but also need to find out for a specific edge the number of shortest path that it lies on, or to sum up all the pair-wise dependency for this specific edge. Brandes algorithm improves the running time and storage required by augmenting the regular BFS algorithm so that we can only solve the single source shortest path once for each vertex and simply accumulate the betweenness centrality scores along the way. Our algorithm, therefore, starts with the regular BFS implementation with the modification mentioned above for weighted edges. At the end of each iteration for the specific source vertex, we backtraced to traverse the vertices visited so far to accumulate the dependencies to be added to the centrality scores for each edge.

After the calculation of edge betweenness centrality, we removed the edge with the highest score and calculated the modularity. The modularity score is often used as a measure of how good the graph partitioning is (Newman, 2004). We used modularity score to optimize community detection. The modularity score is calculated by summing the probability that the edge is within a cluster minus the probability a random edge is within a certain cluster. High modularity indicates a larger number of edges within the group we expect by chance. After each edge removal, we recompute the modularity score and repeat the betweenness centrality caluclation and edge removal until we reach a modularity score of 0.3.

To test the correctness of the algorithms, first, we made sure that our algorithm calculates the modularity score and assigns the clusters correctly. We created a small graph and calculated the modularity score by hand and made sure that it matches the score calculated by our algorithm. We also checked to make sure that nodes within the same cluster have the same community membership value.

Next, to ensure that our Brandes algorithm is working correctly, we created a few smaller graphs to test our algorithm. We computed the betweenness centrality score manually and made sure that it matches the score calculated by the algorithm.

Lastly, we also tested our Girvan-Newman algorithm. The Girvan-Newman algorithm starts by calculating the modularity score, deletes the edge with the highest betweenness centrality score, and recalculates the modularity score. We manually identified the edge with the highest betweenness centrality score and made sure that it matches the edge removed by the Girvan-Newman algorithm.

## Results and Conclusions (The answer to leading question)

### PageRank Results

PageRank is a variant for calculation of eigenvector centrality, although scores on undirect graphs resemble the degree centrality and have been applied to a small extent in the field of neuroimaging (Zuo et al., 2012). In the context of human brain connectome, measures related to degree centrality are usually interpreted as reflecting the strength of local connection, while eigenvector centrality scores are regarded as importance on a global level. Pagerank scores in particular were regarded as measuring the importance of a brain region on the global level but appreciating the local context as well because of the random jumps that the algorithm incorporates. We obtained the page rank scores for each vertex in the scene network for the 2 conditions: imagery versus perception.

Our results demonstrated an interesting distinction between the brain's reaction for when people are imagining a picture in their head versus viewing the actual image. The red dots in the brain represent the PageRank scores, with larger size and darker red reflecting higher rank scores. Importantly, the 2 figures were scaled to the same range using min-max scale to reflect the relative relationship. We can observe that in the imagery condition, the distribution of rank scores is more even compared to the perception condition where there seems to exist an important hub in the right PPA and Hippocampus (right middle blob). This could suggest that connections within the network in general for the imagery condition is more extensive and multi-fold. Intuitively, such observation aligns with the idea that different parts of the brain all had to contribute and work together when we try to generate images from within. However, when we are passively accepting information, there may be less burden for these areas except for the one responsible for encoding the image (Hippocampus, or traditionally known as the memory region). One interesting follow-up would be look at degree centrality and eigenvector centrality separately to see if connectivity changes on both global and local level.


<img width="1023" alt="Screen Shot 2022-05-06 at 4 32 22 PM" src="https://user-images.githubusercontent.com/70292345/167219172-698ae0c6-fd62-40a3-8bc3-f610584d56ea.png">


### Girvan-Newman Results

Another aspect that we were interested in is whether different groups of brain areas are at work for imagery compared to perception conditions. Community detection algorithms are widely applied in the field of human brain connectomes to identify collections of areas that are closely connected to each other. Since the more anterior (in the front) areas of the brain are responsible for more abstract or higher level of processing, we should perhaps see community structures in the front areas such as hippocampus or the memory region for imagery condition, since people are trying to remember or generating the images from within their mind.  We used the girvan-newman algorithm to partition th egraph until reaching a satisfying modularity for both conditions.


<img width="931" alt="Screen Shot 2022-05-06 at 4 34 06 PM" src="https://user-images.githubusercontent.com/70292345/167219336-0b78882f-d049-45fe-9b2f-8ac05cb3024b.png">


In figure 3, the dots with different colors represent a vertex, or a specific portion of the brain and a line is draw between a pair if they are still connected with the partitioning. we are seeing the strong connectivity or close community structure between the areas in the back for both conditions. This may seem counterintuitive, because the back regions, especially the lower one responds to incoming visual signals. The areas in the back of our brain, in another word, are usually activated when we are actually viewing things. Therefore, our results aligns with the common mind's eye idea positing that when we imagining or visualizing scenes in our mind, the process is indeed similar to actually viewing them, therefore "seeing with mind's eye".


<img width="1001" alt="Screen Shot 2022-05-06 at 4 33 15 PM" src="https://user-images.githubusercontent.com/70292345/167219265-7cc0eabc-c424-44e3-89eb-b24e7a125c5c.png">


Another interesting observation is that the intertalk between the memory areas in the 2 hemispheres is more anterior for the imagery condition. This corresponds to our hypothesis that perhaps when we are imagining, more anterior parts are involved and working harder to help us remember what the images look like. However, there aren't any significant community structures in the hippocampus, PPA and RSC area in the middle of the brain. This could be due to the fact that the images used in the experiment are not all strict scene images, so the actcivities in these regions may not be prominent enough to emerge. In addition, the way that the brain activity data were obtained could result in higher signal loss in the middle bottom portion of the brain, exactly where the hippocampus is in our case.

### Interactive Visualization of Results

Since our results are inherently 3-dimensional and photos can only show a 2-dimesnional slice of our results, we highly recommend interacting our results in a browser. You can zoom in and pan around the brain and see the correlated and uncorrelated regions of the brain from arbitrary angles. This can be viewed locally at the filepath we provided. We have also hosted them on a web server as they are too large for GitHub to display a preview of.

- [/data/results/vis_rank_img.html](http://cs225-final-project.max.fan/vis_rank_img.html): our output from the PageRank algorithm for imagining scenes.

- [/data/results/vis_rank_pcp.html](http://cs225-final-project.max.fan/vis_rank_pcp.html): our output from the PageRank algorithm for percieving scenes.

- [/data/results/vis_graph_img.html](http://cs225-final-project.max.fan/vis_graph_img.html): our output from the Girvan-Newman algorithm for imagining scenes.

- [/data/results/vis_graph_pcp.html](http://cs225-final-project.max.fan/vis_graph_pcp.html): our output from the Girvan-Newman algorithm for percieving scenes.

### General Conclusion
We were able to detect clear differences between when a person viewed versus imagined a scene, particularly when we used the PageRank algorithm. We were surprised that PageRank provided a more distinct and clearer analysis than the Girvan-Newman algorithm than we expected, since we thought that a graph community detection algorithm would be more effective in highlighting the differences between viewing and imagining. PageRank is not typically used in the literature analyzing brain activity correlation, so applying PageRank to analyze brain activity could be an avenue of future work.

To summarize, we used the PageRank algorithm to examine the importance of each brain region in the context of the scene network connectome and Givan-Newman algorithm to detect the potential community structures. We compared the results between the imagining versus perceiving condition to address the question of how the connectivity differs and whether the difference aligns with theoretical inference. Our results answered our leading questions. We observed that the disribution of PageRank scores are more even in the imagery condition and the same visual area communities can be detected in the imagery condition, suggesting that when we need to imagine or generate visual images from within, the multiple brain areas had to work harder collaboratively compared to when we only need to passively view the imagines. The differential location of the intertalk between hippocampus: anterior for imagery versus posterior for perception also aligns with our hypothesis for more anterior involvement in the imagery condition.

The effect size of our results, however, comes with some caveats. In particular, we wish that we were able to draw more meaningful conclusions about the patterns we saw in the activity data. One limitation that could have weakened our analysis is that the image set used in the original experiment was not stricly scene images. Therefore, we may not have been getting the maximal activity from the scene-related areas. One possible followup is to perform a whole brain analysis instead to see whether the dynamics could be different for other areas, such as the objects cortex. The scale of graph, however, for the whole brain will be enormous compared to the scene network alone, so we may need to add other optimizations such as using a fibonacci heap in the Brandes algorithm to implement the priority queue to speed up the analysis.

### References

Baldassano C., Fei-Fei L., Esteva A., Beck DM. Two distinct Scene-Processing networks connecting vision and memory. *eNeuro*, *3*(5). doi:10.1523/ENEURO.0178-16.

Brandes, U. (2001) A faster algorithm for betweenness centrality,*The Journal of Mathematical Sociology, 25*(2), 163-177, doi:10.1080/0022250X.2001.9990249

Brandes U. (2008) On variants of shortest-path betweenness centrality and their generic computation.  *Social Networks, 30*(2):136-145.doi:10.1016/j.socnet.2007.11.001

Newman MEJ. & Girvan M. (2004) Finding and evaluating community structure in networks.  *Physical Review E - Statistical, Nonlinear, and Soft Matter Physics, 69*(2 2):026113-1-026113-15. doi:10.1103/PhysRevE.69.026113

Newman MEJ. (2004) Analysis of weighted networks. *Physical Review E., 70*(056131), doi:https://doi.org/10.1103/PhysRevE.70.056131
