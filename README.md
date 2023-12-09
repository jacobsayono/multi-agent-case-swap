# Multi-Agent Case Swap

## Preliminary test scripts are saved in this repository.
`one_swap()` searches through all possible one-swaps between two robots and returns the best one.

`k_swap()` uses one-swap recursively to test all k-number of swaps.

`add_task()` simulates an online task assignment that generates a random cost matrix to this new task. (TODO)

`heuristic.cpp` uses euclidean properties to find potential tasks that can be swap. (TODO)

## Example using 3 robots (X), 2 tasks (O) with cost from each robot to each task.

![Desc](assets/1-swap.jpg)

Results for best 1-swap:

![Result](assets/results.png)

## Example using grid implementation with (x, y) coordinates.

Robots (X) and tasks (O) are initialized.
Let the tasks beneath each robot be assigned to that robot.

Now, let the last test be assigned to the robot on the left (far away from it).
Clearly, an optimal swap would be to give this task to the robot on the right.

Results verify our hypothesis:

![Grid](assets/grid.png)
