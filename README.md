# Multi-Agent Case Swap

## Preliminary test scripts are saved in this repository.
`one_swap()` searches through all possible one-swaps between two robots and returns the best one.

`k_swap()` uses one-swap recursively to test all k-number of swaps.

`add_task()` simulates an online task assignment that generates a random cost matrix to this new task. (TODO)

`heuristic()` uses euclidean properties to find potential tasks that can be swapped. (TODO)

##### Bug fixes & further implementation:
TODO: fix arr output so that it stores best task to swap for every recursive step

TODO: implement add_task() online using argc and argv

TODO: implement heuristic() and measure optimality

##### Secondary features:
TODO: make robot and task position initialization input from txt file.

Future work: use map generation (with walls) as environment and djikstra's algorithm to find cost matrix to each task.

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

Another example (top left robot 0, bottom right robot 1; numbered the task for better visualization):

![Grid2](assets/grid2.png)

