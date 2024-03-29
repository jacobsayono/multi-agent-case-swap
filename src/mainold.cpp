#include <iostream>
#include <vector>  // <list> for assignments would be better since we can insert/remove at O(1)
#include <algorithm>

#include "../include/calc.h"
#include "../include/swap.h"
#include "../include/grid.h"

// one-swap (robot1 give ONE task to robot2) -- recursive one task assignment
// two-swap (robot1 give ONE task to robot2 but also receives ONE task in return, or robot1 gives TWO tasks to robot2)
// k-swap -- exponential complexity

int main(int argc, char** argv) {
    /*
    cost matrix = [num_tasks+1 x num_tasks+1]
        R0      T1      T2      T3
    R0  r0r0    r0t1    r0t2    r0t3
    T1          t1t1    t1t2    t1t3
    T2                  t2t2    t2t3
    T3                          t3t3
    */

    /*
    int num_tasks = 3;
    int matrix_size = (num_tasks+1) * (num_tasks+1);

    float cost_r1[matrix_size] = {0, 1, 2.5, 1.5, 
                                  1, 0, 2, 2,
                                  2.5, 2, 0, 2,
                                  1.5, 2, 2, 0};
    float cost_r2[matrix_size] = {0, 3, 1.2, 3,
                                  3, 0, 2, 2,
                                  1.2, 2, 0, 2,
                                  3, 2, 2, 0};

    std::vector<std::vector<float>> cost_r0 = {{0, 1, 2.5, 1.5},
                                               {1, 0, 2, 2},
                                               {2.5, 2, 0, 2},
                                               {1.5, 2, 2, 0}};
    std::vector<std::vector<float>> cost_r1 = {{0, 3, 1.2, 3},
                                               {3, 0, 2, 2},
                                               {1.2, 2, 0, 2},
                                               {3, 2, 2, 0}};

    // define a previous task assignment for each robot
    // note: this is not an optimal assignment
    std::vector<int> assignment_r0 = {3,2};
    std::vector<int> assignment_r1 = {1};
    */

    /*
    TODO: make robot and task position initialization input from txt file
    Future work: use map generation (with walls) as environment and djikstra's algorithm to find cost matrix to each task
    */

    // depth search value
    int k = 5;

    // TODO: scale for m robots
    // robot and task positions
    std::vector<Position> robots = {{1, 1}, {8, 8}};
    std::vector<Position> tasks = {{7, 5}, {3, 4}, {5, 2}, {6, 7}};
    // TODO: set this using dijkstra's algorithm
    // initial task assignment
    std::vector<std::vector<int>> assignment = {{1, 2}, {3, 4}};  // robotA: task1, task2; robotB: task3, task4;

    // std::vector<Position> robots = {{1, 1}, {8, 8}, {1, 8}};
    // std::vector<Position> tasks = {{7, 5}, {3, 4}, {5, 2}, {5, 7}, {2, 6}, {8, 3}};
    // std::vector<std::vector<int>> assignment = {{1, 2}, {3, 4}, {5, 6}};

    int num_robots = robots.size();
    int num_tasks = tasks.size();

    std::cout << "Number of robots: " << num_robots << std::endl;
    std::cout << "Number of tasks: " << num_tasks << std::endl;
    std::cout << std::endl;

    // grid size
    const int grid_width = 10;
    const int grid_height = 10;

    // print the grid
    std::cout << "Grid Visualization:" << std::endl;
    print_grid(robots, tasks, grid_width, grid_height);
    std::cout << std::endl;

    // hold cost matrix for each robot
    std::vector<std::vector<std::vector<float>>> cost_matrices(num_robots);

    // hold initial assignment cost for each robot
    std::vector<float> initial_assignment_cost(num_robots);

    // print cost matrices
    for (int r = 0; r < num_robots; ++r) {
        cost_matrices[r] = generate_cost_matrix(robots[r], tasks);
        std::cout << "Cost matrix for robot " << static_cast<char>('A' + r) << ": " << std::endl;
        print_cost_matrix(cost_matrices[r], robots[r]);
    }

    std::cout << std::endl;

    // print initial task assignments
    for (int r = 0; r < num_robots; ++r) {
        std::cout << "Initial task assignments for robot " << static_cast<char>('A' + r) << " at " << "(" << robots[r].x << "," << robots[r].y << "): ";
        for (int i = 0; i < assignment[r].size(); ++i) {
            std::cout << assignment[r][i] << " ";
        }
        std::cout << std::endl;
    }

    // print each robot assignment cost
    for (int r = 0; r < num_robots; ++r) {
        initial_assignment_cost[r] = calc_path_cost(num_tasks, assignment[r], cost_matrices[r]);
        std::cout << "Current robot " << static_cast<char>('A' + r) << " assignment cost: " << initial_assignment_cost[r] << std::endl;
    }

    // calculate current makespan and sum of costs after all robots are processed
    float current_makespan = 0;
    float current_sum_of_costs = 0;
    for (int r = 0; r < num_robots; ++r) {
        current_makespan = std::max(current_makespan, initial_assignment_cost[r]);
        current_sum_of_costs += initial_assignment_cost[r];
    }

    std::cout << "Current makespan: " << current_makespan << std::endl;
    std::cout << "Current sum-of-costs: " << current_sum_of_costs << std::endl;

    // use the one_swap() function recursively to see if a swap is desirable
    SwapResult arr = k_swap(num_robots, num_tasks, assignment, cost_matrices, k, current_makespan, current_sum_of_costs);

    /*
    TODO: implement add_task() online using argc and argv
    TODO: implement heuristic() and measure optimality
    */

    return 0;
}
