# Multi-Agent Case Swap

## Build using CMake.
```
git clone https://github.com/jacobsayono/multi-agent-case-swap.git
cd multi-agent-case-swap
mkdir build
cd build
cmake ..
cmake --build .
./brute
```

## Tree structure for DFS & BFS.

![Tree](assets/tree.jpg)

![DFS](assets/DFS.jpg)

![DFSout](assets/DFSout.png)

![BFS](assets/BFS.jpg)

![BFSout](assets/BFSout.png)

## Example using 3 robots (X), 2 tasks (O) with cost from each robot to each task.

![Desc](assets/1-swap.jpg)

Results for best 1-swap:

![Result](assets/results.png)

Results for best k-swap using tree structure:

![Result](assets/brute.png)

## Example using recursive k-swap for grid implementation with (x, y) coordinates.

![Example](assets/milestone.png)

## Tree class.

The tree class builds a tree of nodes. Each node consists of a task assignment configuration, including its makespan and sum of costs. Additionally, the node dynamically allocates new memory to create a layer of children nodes, pointing to their respective addresses in memory. Each layer of the tree represents the depth in which we do k-swaps. The explored task assignment configuration is stored in a hash table to prevent duplicates and allowing the algorithm to terminate.

The goal is to form a DFS/BFS heuristic using a mathematical hypothesis that hints at us in approaching a lower bound on makespan. The first step is to use a brute-force approach to keep track of the minimum makespan as we traverse the tree.
