# Heuristic Graph Camera Placement (Minimal Vertex Cover)

This program implements a **reverse greedy heuristic** to find a Minimal Vertex Cover in a graph. This accomplished by removing cameras from vertices, by priority of lower degrees, only if a camera is redundant.

For every camera removed from a vertice, mark every neighbor as having a dependent edge.

The redundancy then is easily checked by checking if the current vertex was marked as having a dependent edge.

Since this algorithm only iterate through neighbors once for every vertice, the total amount of iterations (in the worst case) is 2*E, making the algorithm complexity O(V + E) and achieving better results than most minimal cover algorithms.

## Input
It is expected a .gml file named "sjdr", which will contain a list of vertices and edges that will be processed.


## Output
The program first outputs wether the produced set of cameras is minimal (which should always be true). Then, all cameras with their respective vertex number and roads covered. Finally, the number of cameras placed.