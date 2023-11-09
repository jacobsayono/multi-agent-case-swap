#include <iostream>
#include <list>  // <list> for paths would be better since we can insert/remove at O(1)
#include <algorithm>
#include <chrono>  // benchmark performance

// one-swap (robot1 give ONE task to robot2) -- recursive one task assignment
// two-swap (robot1 give ONE task to robot2 but also receives ONE task in return, or robot1 gives TWO tasks to robot2)
// k-swap -- exponential complexity

// calculate total cost of a path for a robot
float calc_path_cost(int num_tasks, const std::list<int>& path, const float* cost) {
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
        return 0;
    }

    auto it = path.begin();
    total_path_cost = cost[*it];
    int from_here = *it;

    for (++it; it != path.end(); ++it) {
        int to_here = *it;
        total_path_cost += cost[from_here * (num_tasks + 1) + to_here];
        from_here = to_here;
    }
    return total_path_cost;
}

// calculate makespan, i.e., lowest path cost out of all the paths
float calc_makespan(float r1, float r2) {
    // note: if want to check minimum of multiple paths > 2,
    // modify this function to store those values in a list and use std::min_element(vec.begin(), vec.end())
    return std::max(r1, r2);
}

// calculate sum_of_costs of all paths in the mission
float calc_sum_of_costs(float r1, float r2) {
    return r1 + r2;
}

// calculate new path cost after a potential swap
float calc_swapped_path_cost(const std::list<int>& original_path, const float* cost, int num_tasks, int task_to_remove, int task_to_insert, int insert_pos) {
    // create copy of original path
    std::list<int> new_path = original_path;

    // remove task_to_remove if it is in the path
    if (task_to_remove != -1) {
        new_path.remove(task_to_remove); // List provides direct remove operation
    }

    // insert task_to_insert at insert_pos
    if (task_to_insert != -1) {
            auto it = new_path.begin();
            std::advance(it, insert_pos); // Advance the iterator to the correct insertion point
            new_path.insert(it, task_to_insert);
        }
    else {
        // not sure what to do, but surely we won't have a case where there is an invalid insert position
        std::cerr << "insert position out of bounds" << std::endl;
        return -1;
    }

    // return new_path cost
    return calc_path_cost(num_tasks, new_path, cost);
}

struct SwapResult {
    int task_to_swap;
    float makespan_diff;
    float sum_of_costs_diff;
    int swap_path;
};

// check the new path costs after a task swap and update the best swap if it is better
SwapResult evaluate_task_swap(int num_tasks, const std::list<int>& path_from, const std::list<int>& path_to, const float cost_from[], const float cost_to[], int task_to_swap, float initial_makespan, float initial_sum_of_costs, float& best_makespan_diff, float& best_sum_of_costs_diff, int& best_insert_position, int from_path_id) {
    SwapResult result = {-1, 0.0f, 0.0f};
    float new_path_from_cost = calc_swapped_path_cost(path_from, cost_from, num_tasks, task_to_swap, -1, -1);

    for (int insert_pos = 0; insert_pos <= path_to.size(); ++insert_pos) {
        float new_path_to_cost = calc_swapped_path_cost(path_to, cost_to, num_tasks, -1, task_to_swap, insert_pos);
        float new_makespan = calc_makespan(new_path_from_cost, new_path_to_cost);
        float new_sum_of_costs = calc_sum_of_costs(new_path_from_cost, new_path_to_cost);

        float makespan_diff = new_makespan - initial_makespan;
        float sum_of_costs_diff = new_sum_of_costs - initial_sum_of_costs;

        if (makespan_diff < best_makespan_diff || (makespan_diff == best_makespan_diff && sum_of_costs_diff < best_sum_of_costs_diff)) {
            best_makespan_diff = makespan_diff;
            best_sum_of_costs_diff = sum_of_costs_diff;
            best_insert_position = insert_pos;
            result.task_to_swap = task_to_swap;
            result.makespan_diff = makespan_diff;
            result.sum_of_costs_diff = sum_of_costs_diff;
            result.swap_path = from_path_id;
        }
    }

    return result;
}

// function to perform a one swap if it's optimal. returns an array for the swapped task number, makespan diff, and sum_of_costs diff
// _ -> 1 -> _ -> 2 -> _
// Function to find the best task swap between two paths
SwapResult one_swap(
    int num_tasks, 
    std::list<int>& path1, 
    std::list<int>& path2, 
    const float cost1[], 
    const float cost2[]) 
{
    float initial_makespan = calc_makespan(calc_path_cost(num_tasks, path1, cost1), calc_path_cost(num_tasks, path2, cost2));
    float initial_sum_of_costs = calc_sum_of_costs(calc_path_cost(num_tasks, path1, cost1), calc_path_cost(num_tasks, path2, cost2));

    float best_makespan_diff = std::numeric_limits<float>::max();
    float best_sum_of_costs_diff = std::numeric_limits<float>::max();
    int best_task_to_swap = -1;
    int best_swap_path = -1;
    int best_insert_position = -1;

    int index = 0;
    for (auto it = path1.begin(); it != path1.end(); ++it, ++index) {
        int task_to_swap = *it;
        // Evaluate potential swap
        SwapResult potential_swap = evaluate_task_swap(num_tasks, path1, path2, cost1, cost2, task_to_swap, initial_makespan, initial_sum_of_costs, best_makespan_diff, best_sum_of_costs_diff, best_insert_position, 1);
        // Update best swap if the potential swap is better
        if (potential_swap.task_to_swap != -1) {
            best_task_to_swap = task_to_swap;
            best_swap_path = 1;
        }
    }

    // Iterate over path2 using an iterator
    index = 0;
    for (auto it = path2.begin(); it != path2.end(); ++it, ++index) {
        int task_to_swap = *it;
        // Evaluate potential swap
        SwapResult potential_swap = evaluate_task_swap(num_tasks, path2, path1, cost2, cost1, task_to_swap, initial_makespan, initial_sum_of_costs, best_makespan_diff, best_sum_of_costs_diff, best_insert_position, 2);
        // Update best swap if the potential swap is better
        if (potential_swap.task_to_swap != -1) {
            best_task_to_swap = task_to_swap;
            best_swap_path = 2;
        }
    }

    // Perform the best swap if found
    if (best_task_to_swap != -1) {
        if (best_swap_path == 1) {
            // Remove the task from path1 and add it to path2 at the best position
            path1.remove(best_task_to_swap);
            auto it_path2 = path2.begin();
            std::advance(it_path2, best_insert_position);
            path2.insert(it_path2, best_task_to_swap);
        } else {
            // Remove the task from path2 and add it to path1 at the best position
            path2.remove(best_task_to_swap);
            auto it_path1 = path1.begin();
            std::advance(it_path1, best_insert_position);
            path1.insert(it_path1, best_task_to_swap);
        }
    }

    // Return the best swap result
    return SwapResult{best_task_to_swap, best_makespan_diff, best_sum_of_costs_diff};
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

// k-swap
SwapResult recursive_fcn(int counter, int limit) {
    if (counter > limit) {
        return SwapResult{-1, 0.0f, 0.0f, -1};
    }
    return recursive_fcn(counter+1, limit);
}

// refactor code
// for n paths, n matrix_costs
// increase efficient memory using pointers
// condense into one function for repetitive code
// benchmark between <list> and <list> for robot path made up of task #s

int main(int argc, char * argv[]) {
    auto start_time = std::chrono::high_resolution_clock::now();

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
    std::list<int> path_r1 = {3,2};
    std::list<int> path_r2 = {1};

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
    for (auto it = path_r1.begin(); it != path_r1.end(); ++it) {
        std::cout << *it << std::endl;
    }

    std::cout << "Printing robot 2 assignment\n";
    for (auto it = path_r2.begin(); it != path_r2.end(); ++it) {
        std::cout << *it << std::endl;
    }


    std::cout << std::endl;

    // Print the change in makespan and sum-of-costs
    std::cout << "Best task to swap: " << arr.task_to_swap << std::endl;
    std::cout << "Change in makespan: " << arr.makespan_diff << std::endl;
    std::cout << "Change in sum-of-costs: " << arr.sum_of_costs_diff << std::endl;
    std::cout << std::endl;

    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
    std::cout << "Total execution time: " << elapsed.count() << " milliseconds." << std::endl;


    return 0;
}