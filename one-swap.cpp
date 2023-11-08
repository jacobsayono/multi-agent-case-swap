#include <iostream>
#include <vector>  // <list> for paths would be better since we can insert/remove at O(1)
#include <algorithm>



// one-swap (robot1 give ONE task to robot2) -- recursive one task assignment
// two-swap (robot1 give ONE task to robot2 but also receives ONE task in return, or robot1 gives TWO tasks to robot2)
// k-swap -- exponential complexity

// calculate total cost of a path for a robot
float calc_path_cost(int num_tasks, const std::vector<int>& path, const float* cost) {
    float total_path_cost = 0;

    /*
             R0   T1   T2   T3
        R0 [  0    1    2    3 ]
        T1 [  4    5    6    7 ]
        T2 [  8    9   10   11 ]
        T3 [  12  13   14   15 ]
    */

    // formula to get index in array: from_here * (num_tasks+1) + to_here
    // e.g.: R0 to T3 = 0 * (3+1) * 3 = 3
    // e.g.: T3 to T2 = 3 * (3+1) * 2 = 14

    if (path.empty()) {
        total_path_cost = 0;
    }
    else if (path.size() == 1) {
        total_path_cost = cost[path[0]];
    }
    else {
        total_path_cost = cost[path[0]];
        for (int i = 0; i < path.size() - 1; ++i) {
            total_path_cost += cost[path[i] * (num_tasks+1) + path[i+1]];
        }
    }
    return total_path_cost;
}

// calculate makespan, i.e., lowest path cost out of all the paths
float calc_makespan(float r1, float r2) {
    // note: if want to check minimum of multiple paths > 2,
    // modify this function to store those values in a vector and use std::min_element(vec.begin(), vec.end())
    return std::max(r1, r2);
}

// calculate sum_of_costs of all paths in the mission
float calc_sum_of_costs(float r1, float r2) {
    return r1 + r2;
}

// calculate new path cost after a potential swap
float calc_swapped_path_cost(const std::vector<int>& original_path, const float* cost, int num_tasks, int task_to_remove, int task_to_insert, int insert_pos) {
    // create copy of original path
    std::vector<int> new_path = original_path;

    // remove task_to_remove if it is in the path
    if (task_to_remove != -1) {
        std::vector<int>::iterator it = std::find(new_path.begin(), new_path.end(), task_to_remove);
        if (it != new_path.end()) {
            new_path.erase(it);
        }
    }

    // insert task_to_insert at insert_pos
    if (task_to_insert != -1) {
        if (insert_pos <= new_path.size()) {
            new_path.insert(new_path.begin() + insert_pos, task_to_insert);
        }
        else {
            // not sure what to do, but surely we won't have a case where there is an invalid insert position
            std::cerr << "insert position out of bounds" << std::endl;
            return -1;
        }
    }

    // return new_path cost
    return calc_path_cost(num_tasks, new_path, cost);
}

struct SwapResult {
    int task_to_swap;
    float makespan_diff;
    float sum_of_costs_diff;
};

/*
    Goal:
    -For each currently assigned task:
        -Find the new path cost for that robot if that task was not assigned to it 
         (Assuming the order of the other tasks does not change)
        -For each robot this task is not assigned to:
            -Check the path cost for that robot with this task inserted at every
             possible point in its current task sequence
            -Save the lowest path cost and the position in the sequence
        -Find a best swap for this task that minimizes the total makespan first
         and sum-of-costs second
    -Select the best task to swap and modify the task assignment sequences. Return
     the task number for the task that was swapped and the change in makespan and 
     sum-of-costs.
    -If no task decreases the makespan (or decreases sum-of-costs without changing
     the makespan), do not change the task assignment and return the array given 
     below

    Note: The task assignments are passed as pointers, you can modify them in this 
    function and they will be changed in main. You should not need to change any 
    code in main.
*/

// function to perform a one swap if it's optimal. returns an array for the swapped task number, makespan diff, and sum_of_costs diff
// _ -> 1 -> _ -> 2 -> _
SwapResult one_swap(int num_tasks, std::vector<int>& path1, std::vector<int>& path2, const float cost1[], const float cost2[]) {
    // get the cost of each path
    float path1_cost = calc_path_cost(num_tasks, path1, cost1);
    float path2_cost = calc_path_cost(num_tasks, path2, cost2);

    // get the initial makespan and sum_of_costs
    float initial_makespan = calc_makespan(path1_cost, path2_cost);
    float initial_sum_of_costs = calc_sum_of_costs(path1_cost, path2_cost);

    // initialize variables to track the best swap found
    float best_makespan_diff = std::numeric_limits<float>::max();
    float best_sum_of_costs_diff = std::numeric_limits<float>::max();
    int best_task_to_swap = -1;
    int best_swap_path = -1;  // 1 for path1 to path2, 2 for path2 to path1
    int best_insert_position = -1;

    SwapResult result = {-1, 0.0f, 0.0f};

    // check each task in path1 to see if moving it to path2 improves makespan or sum_of_costs
    for (int i = 0; i < path1.size(); ++i) {
        int task_to_swap = path1[i];
        // calculate new path cost for robot 1 without the task
        float new_path1_cost = calc_swapped_path_cost(path1, cost1, num_tasks, task_to_swap, -1, -1);
        // try inserting the task at every position in robot 2's path
        for (int insert_pos = 0; insert_pos <= path2.size(); ++insert_pos) {
            float new_path2_cost = calc_swapped_path_cost(path2, cost2, num_tasks, -1, task_to_swap, insert_pos);
            float new_makespan = calc_makespan(new_path1_cost, new_path2_cost);
            float new_sum_of_costs = calc_sum_of_costs(new_path1_cost, new_path2_cost);

            // check if this swap improves makespan and/or sum_of_costs
            if (new_makespan < initial_makespan ||
                (new_makespan == initial_makespan && new_sum_of_costs < initial_sum_of_costs)) {
                // check if this is the best swap found so far based on makespan and sum_of_costs
                float makespan_diff = new_makespan - initial_makespan;
                float sum_of_costs_diff = new_sum_of_costs - initial_sum_of_costs;
                if (makespan_diff < best_makespan_diff ||
                    (makespan_diff == best_makespan_diff && sum_of_costs_diff < best_sum_of_costs_diff)) {
                    // update the best swap found so far
                    best_makespan_diff = makespan_diff;
                    best_sum_of_costs_diff = sum_of_costs_diff;
                    best_task_to_swap = task_to_swap;
                    best_swap_path = 1;  // indicates task moved from path1 to path2
                    best_insert_position = insert_pos;
                }
            }
        }
    }

    // repeat the above loop for each task in path2 to see if moving it to path1 improves makespan or sum_of_costs
    for (int i = 0; i < path2.size(); ++i) {
        int task_to_swap = path2[i];
        // calculate new path cost for robot 2 without the task
        float new_path2_cost = calc_swapped_path_cost(path2, cost2, num_tasks, task_to_swap, -1, -1);
        // try inserting the task at every position in robot 1's path
        for (int insert_pos = 0; insert_pos <= path1.size(); ++insert_pos) {
            float new_path1_cost = calc_swapped_path_cost(path1, cost1, num_tasks, -1, task_to_swap, insert_pos);
            float new_makespan = calc_makespan(new_path1_cost, new_path2_cost);
            float new_sum_of_costs = calc_sum_of_costs(new_path1_cost, new_path2_cost);

            // check if this swap improves makespan and/or sum_of_costs
            if (new_makespan < initial_makespan ||
                (new_makespan == initial_makespan && new_sum_of_costs < initial_sum_of_costs)) {
                // check if this is the best swap found so far based on makespan and sum_of_costs
                float makespan_diff = new_makespan - initial_makespan;
                float sum_of_costs_diff = new_sum_of_costs - initial_sum_of_costs;
                if (makespan_diff < best_makespan_diff ||
                    (makespan_diff == best_makespan_diff && sum_of_costs_diff < best_sum_of_costs_diff)) {
                    // update the best swap found so far
                    best_makespan_diff = makespan_diff;
                    best_sum_of_costs_diff = sum_of_costs_diff;
                    best_task_to_swap = task_to_swap;
                    best_swap_path = 2;  // indicates task moved from path2 to path1
                    best_insert_position = insert_pos;
                }
            }
        }
    }


    // if a swap was found that improves makespan or sum_of_costs, perform the swap and update the result
    if (best_task_to_swap != -1) {
        if (best_swap_path == 1) {
            path1.erase(std::remove(path1.begin(), path1.end(), best_task_to_swap));
            path2.insert(path2.begin() + best_insert_position, best_task_to_swap);
        } else {
            path2.erase(std::remove(path2.begin(), path2.end(), best_task_to_swap));
            path1.insert(path1.begin() + best_insert_position, best_task_to_swap);
        }

        result.task_to_swap = best_task_to_swap;
        result.makespan_diff = best_makespan_diff;
        result.sum_of_costs_diff = best_sum_of_costs_diff;
        return result;
    }

    // if no beneficial swap was found, return the default result indicating no swap
    return result;
}

/*
For task 1 moving to r1:
    r1: {1, 3, 2}, r2: {}
    r1: {3, 1, 2}, r2: {}
    r1: {3, 2, 1}, r2: {}

For task 2 moving to r2:
    r1: {3}, r2: {2, 1}
    r1: {3}, r2: {1, 2}

For task 3 moving to r2:
    r1: {2}, r2: {3, 1}
    r1: {2}, r2: {1, 3}
*/

int main(int argc, char * argv[]) {
    // Start by defining a cost matrix for several robots
    int num_tasks = 3;
    int matrix_size = (num_tasks+1) * (num_tasks+1);

    /*
    cost matrix = [num_tasks+1 x num_tasks+1]
        R0      T1      T2      T3
    R0  r0r0    r0t1    r0t2    r0t3
    T1          t1t1    t1t2    t1t3
    T2                  t2t2    t2t3
    T3                          t3t3
    */

    float cost_r1[matrix_size] = {0, 1, 2.5, 1.5, 
                                  1, 0, 2, 2,
                                  2.5, 2, 0, 2,
                                  1.5, 2, 2, 0};
    float cost_r2[matrix_size] = {0, 3, 1.2, 3,
                                  3, 0, 2, 2,
                                  1.2, 2, 0, 2,
                                  3, 2, 2, 0};
    
    // Define a previous task assignment for each robot
    // Note: this is not an optimal assignment
    std::vector<int> path_r1 = {3,2};
    std::vector<int> path_r2 = {1};

        // Calculate and print the initial path costs, makespan, and sum-of-costs
    float initial_path1_cost = calc_path_cost(num_tasks, path_r1, cost_r1);
    float initial_path2_cost = calc_path_cost(num_tasks, path_r2, cost_r2);
    float initial_makespan = calc_makespan(initial_path1_cost, initial_path2_cost);
    float initial_sum_of_costs = calc_sum_of_costs(initial_path1_cost, initial_path2_cost);

    std::cout << "Initial r1 path cost: " << initial_path1_cost << std::endl;
    std::cout << "Initial r2 path cost: " << initial_path2_cost << std::endl;
    std::cout << "Initial makespan: " << initial_makespan << std::endl;
    std::cout << "Initial sum-of-costs: " << initial_sum_of_costs << std::endl;

    std::cout << std::endl;

    // Use a one_swap function to see if a swap is desirable
    SwapResult arr = one_swap(num_tasks, path_r1, path_r2, cost_r1, cost_r2);

    // Calculate and print the final path costs, makespan, and sum-of-costs after the swap
    float final_path1_cost = calc_path_cost(num_tasks, path_r1, cost_r1);
    float final_path2_cost = calc_path_cost(num_tasks, path_r2, cost_r2);
    float final_makespan = calc_makespan(final_path1_cost, final_path2_cost);
    float final_sum_of_costs = calc_sum_of_costs(final_path1_cost, final_path2_cost);

    std::cout << "Final r1 path cost: " << final_path1_cost << std::endl;
    std::cout << "Final r2 path cost: " << final_path2_cost << std::endl;
    std::cout << "Final makespan: " << final_makespan << std::endl;
    std::cout << "Final sum-of-costs: " << final_sum_of_costs << std::endl;

    std::cout << std::endl;

    // Print task assignments
    std::cout << "Printing robot 1 assignment\n";
    for (int i=0; i<path_r1.size(); i++) {
        std::cout << path_r1[i] << std::endl;
    }
    std::cout << "Printing robot 2 assignment\n";
    for (int i=0; i<path_r2.size(); i++) {
        std::cout << path_r2[i] << std::endl;
    }

    // Print the change in makespan and sum-of-costs
    std::cout << "Best task to swap: " << arr.task_to_swap << std::endl;
    std::cout << "Change in makespan: " << arr.makespan_diff << std::endl;
    std::cout << "Change in sum-of-costs: " << arr.sum_of_costs_diff << std::endl;

    return 0;
}