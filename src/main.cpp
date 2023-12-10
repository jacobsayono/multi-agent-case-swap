#include <iostream>
#include <vector>  // <list> for paths would be better since we can insert/remove at O(1)
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
    std::vector<int> path_r0 = {3,2};
    std::vector<int> path_r1 = {1};
    */

    /*
    TODO: make robot and task position initialization input from txt file
    Future work: use map generation (with walls) as environment and djikstra's algorithm to find cost matrix to each task
    */

    // robot and task positions
    std::vector<Position> robots = {{1, 1}, {8, 8}};
    std::vector<Position> tasks = {{7, 5}, {3, 4}, {5, 2}};

    int num_tasks = tasks.size();

    // initial task assignment
    std::vector<int> path_r0 = {1,2};
    std::vector<int> path_r1 = {3};

    std::cout << "Initial task assignments for robot 0: ";
    for (int i = 0; i < path_r0.size(); ++i) {
        std::cout << path_r0[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Initial task assignments for robot 1: ";
    for (int i = 0; i < path_r1.size(); ++i) {
        std::cout << path_r1[i] << " ";
    }
    std::cout << std::endl << std:: endl;

    // grid size
    const int grid_width = 10;
    const int grid_height = 10;

    // print the grid
    std::cout << "Grid Visualization:" << std::endl;
    print_grid(robots, tasks, grid_width, grid_height);
    std::cout << std::endl;

    const int grid_size = tasks.size() + 1;

    std::vector<std::vector<float>> cost_r0 = generate_cost_matrix(robots[0], tasks);
    std::vector<std::vector<float>> cost_r1 = generate_cost_matrix(robots[1], tasks);

    print_cost_matrix(cost_r0, robots[0]);
    print_cost_matrix(cost_r1, robots[1]);

    // calculate and print the initial path costs, makespan, and sum-of-costs
    float initial_path1_cost = calc_path_cost(num_tasks, path_r0, cost_r0);
    float initial_path2_cost = calc_path_cost(num_tasks, path_r1, cost_r1);
    float initial_makespan = calc_makespan(initial_path1_cost, initial_path2_cost);
    float initial_sum_of_costs = calc_sum_of_costs(initial_path1_cost, initial_path2_cost);

    std::cout << "Initial r0 path cost: " << initial_path1_cost << std::endl;
    std::cout << "Initial r1 path cost: " << initial_path2_cost << std::endl;
    std::cout << "Initial makespan: " << initial_makespan << std::endl;
    std::cout << "Initial sum-of-costs: " << initial_sum_of_costs << std::endl;
    std::cout << std::endl;

    // depth search value
    int k = 1;

    // use the one_swap() function recursively to see if a swap is desirable
    SwapResult arr = k_swap(num_tasks, path_r0, path_r1, cost_r0, cost_r1, k);

    /*
    TODO: fix arr output so that it stores best task to swap for every recursive step
    TODO: implement add_task() online using argc and argv
    TODO: implement heuristic() and measure optimality
    */

    // calculate and print the final path costs, makespan, and sum-of-costs after the swap
    float final_path1_cost = calc_path_cost(num_tasks, path_r0, cost_r0);
    float final_path2_cost = calc_path_cost(num_tasks, path_r1, cost_r1);
    float final_makespan = calc_makespan(final_path1_cost, final_path2_cost);
    float final_sum_of_costs = calc_sum_of_costs(final_path1_cost, final_path2_cost);

    std::cout << "Final r0 path cost: " << final_path1_cost << std::endl;
    std::cout << "Final r1 path cost: " << final_path2_cost << std::endl;
    std::cout << "Final makespan: " << final_makespan << std::endl;
    std::cout << "Final sum-of-costs: " << final_sum_of_costs << std::endl;
    std::cout << std::endl;

    // print task assignments
    std::cout << "Printing robot 0 assignment\n";
    for (int i=0; i<path_r0.size(); i++) {
        std::cout << path_r0[i] << std::endl;
    }
    std::cout << "Printing robot 1 assignment\n";
    for (int i=0; i<path_r1.size(); i++) {
        std::cout << path_r1[i] << std::endl;
    }

    std::cout << std::endl;

    // print the change in makespan and sum-of-costs
    std::cout << "Best task to swap: " << arr.task_to_swap << std::endl;
    std::cout << "Change in makespan: " << arr.makespan_diff << std::endl;
    std::cout << "Change in sum-of-costs: " << arr.sum_of_costs_diff << std::endl;
    std::cout << std::endl;

    return 0;
}
