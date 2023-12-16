#include "../include/swap.h"
#include "../include/calc.h"

// check the new path costs after a task swap and update the best swap if it is better
SwapResult evaluate_task_swap(int num_tasks, const std::vector<int>& path_from, const std::vector<int>& path_to, const std::vector<std::vector<float>>& cost_from,const std::vector<std::vector<float>>& cost_to,
                              int task_to_swap, float initial_makespan, float initial_sum_of_costs, float& best_makespan_diff, float& best_sum_of_costs_diff, int& best_insert_position, int from_path_id) {
    SwapResult result = {-1, 0.0f, 0.0f};
    float new_path_from_cost = calc_swapped_path_cost(path_from, cost_from, num_tasks, task_to_swap, -1, -1);

    for (int insert_pos = 0; insert_pos <= path_to.size(); ++insert_pos) {
        float new_path_to_cost = calc_swapped_path_cost(path_to, cost_to, num_tasks, -1, task_to_swap, insert_pos);
        float new_makespan = calc_makespan(new_path_from_cost, new_path_to_cost);
        float new_sum_of_costs = calc_sum_of_costs(new_path_from_cost, new_path_to_cost);

        float makespan_diff = new_makespan - initial_makespan;
        float sum_of_costs_diff = new_sum_of_costs - initial_sum_of_costs;

        if (makespan_diff <= 0 && sum_of_costs_diff <= 0) {
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
// function to find the best task swap between two paths
SwapResult one_swap(int num_tasks, std::vector<int>& path1, std::vector<int>& path2, const std::vector<std::vector<float>>& cost1, const std::vector<std::vector<float>>& cost2) {
    float initial_makespan = calc_makespan(calc_path_cost(num_tasks, path1, cost1), calc_path_cost(num_tasks, path2, cost2));
    float initial_sum_of_costs = calc_sum_of_costs(calc_path_cost(num_tasks, path1, cost1), calc_path_cost(num_tasks, path2, cost2));

    float best_makespan_diff = std::numeric_limits<float>::max();  // highest possible number to compare to (basically infinity)
    float best_sum_of_costs_diff = std::numeric_limits<float>::max();  // highest possible number to compare to (basically infinity)
    int best_task_to_swap = -1;
    int best_swap_path = -1;
    int best_insert_position = -1;

    // evaluate potential swaps from path1 to path2
    for (int i = 0; i < path1.size(); ++i) {
        int task_to_swap = path1[i];
        // evaluate potential swap
        SwapResult potential_swap = evaluate_task_swap(num_tasks, path1, path2, cost1, cost2, task_to_swap, initial_makespan, initial_sum_of_costs, best_makespan_diff, best_sum_of_costs_diff, best_insert_position, 1);
        // update best swap if the potential swap is better
        if (potential_swap.task_to_swap != -1) {
            best_task_to_swap = task_to_swap;
            best_swap_path = 1;
        }
    }

    // evaluate potential swaps from path2 to path1
    for (int i = 0; i < path2.size(); ++i) {
        int task_to_swap = path2[i];
        // evaluate potential swap
        SwapResult potential_swap = evaluate_task_swap(num_tasks, path2, path1, cost2, cost1, task_to_swap, initial_makespan, initial_sum_of_costs, best_makespan_diff, best_sum_of_costs_diff, best_insert_position, 2);
        // update best swap if the potential swap is better
        if (potential_swap.task_to_swap != -1) {
            best_task_to_swap = task_to_swap;
            best_swap_path = 2;
        }
    }


    // perform the best swap if found
    if (best_task_to_swap != -1) {
        std::cout << "Best task to swap: " << best_task_to_swap << std::endl;
        if (best_swap_path == 1) {
            // remove the task from path1 and add it to path2 at the best position
            path1.erase(std::remove(path1.begin(), path1.end(), best_task_to_swap));
            path2.insert(path2.begin() + best_insert_position, best_task_to_swap);
            std::cout << "Move this task from robot A to robot B" << std::endl;
        }
        else {
            // remove the task from path2 and add it to path1 at the best position
            path2.erase(std::remove(path2.begin(), path2.end(), best_task_to_swap));
            path1.insert(path1.begin() + best_insert_position, best_task_to_swap);
            std::cout << "Move this task from robot B to robot A" << std::endl;

        }
    }
    else {
        best_makespan_diff = 0;
        best_sum_of_costs_diff = 0;
        std::cout << "Current task assignments are already most optimal; no further swaps necessary!";
        return SwapResult{best_task_to_swap, best_makespan_diff, best_sum_of_costs_diff};
    }

    // print task assignments
    std::cout << "Printing robot A assignment:";
    for (int i=0; i<path1.size(); i++) {
        std::cout << " " << path1[i];
    }
    std::cout << std::endl;
    std::cout << "Printing robot B assignment:";
    for (int i=0; i<path2.size(); i++) {
        std::cout << " " << path2[i];
    }
    std::cout << std::endl;

    // calculate and print the final path costs, makespan, and sum-of-costs after the swap
    float final_path1_cost = calc_path_cost(num_tasks, path1, cost1);
    float final_path2_cost = calc_path_cost(num_tasks, path2, cost2);
    float final_makespan = calc_makespan(final_path1_cost, final_path2_cost);
    float final_sum_of_costs = calc_sum_of_costs(final_path1_cost, final_path2_cost);

    std::cout << "Current robot A path cost: " << final_path1_cost << std::endl;
    std::cout << "Current robot B path cost: " << final_path2_cost << std::endl;
    std::cout << "Current makespan: " << final_makespan << std::endl;
    std::cout << "Current sum-of-costs: " << final_sum_of_costs << std::endl;
    std::cout << "Current change in makespan: " << best_makespan_diff << std::endl;
    std::cout << "Current change in sum-of-costs: " << best_sum_of_costs_diff;

    // return the best swap result
    return SwapResult{best_task_to_swap, best_makespan_diff, best_sum_of_costs_diff};
}

/*
for task 1 moving to r1:
    r1: {1, 3, 2}, r2: {}
    r1: {3, 1, 2}, r2: {}
    r1: {3, 2, 1}, r2: {}

for task 2 moving to r2:
    r1: {3}, r2: {2, 1}
    r1: {3}, r2: {1, 2}

for task 3 moving to r2:
    r1: {2}, r2: {3, 1}
    r1: {2}, r2: {1, 3}
*/

// perform k swaps (recursive)
SwapResult k_swap(int num_tasks, std::vector<int>& path1, std::vector<int>& path2, const std::vector<std::vector<float>>& cost1, const std::vector<std::vector<float>>& cost2, int k, const float initial_makespan, const float initial_sum_of_costs) {
    if (k <= 0) {
        std::cout << std::endl;
        std::cout << "ERROR: Please set depth value k > 0" << std::endl;
        return SwapResult{-1, 0.0f, 0.0f, -1}; // no swap needed or possible
    }

    static int i = 1;
    std::cout << std::endl;
    std::cout << "Depth = " << i << std::endl;

    // perform one swap
    SwapResult result = one_swap(num_tasks, path1, path2, cost1, cost2);
    i++;
    std::cout << std::endl;

    // if a swap was made, recurse with k-1
    if (result.task_to_swap != -1) {
        SwapResult recursiveResult = k_swap(num_tasks, path1, path2, cost1, cost2, k - 1, initial_makespan, initial_sum_of_costs);
        // choose the result with the best makespan or sum_of_costs improvement
        if (recursiveResult.makespan_diff < result.makespan_diff || 
            (recursiveResult.makespan_diff == result.makespan_diff && recursiveResult.sum_of_costs_diff < result.sum_of_costs_diff)) {
            return recursiveResult;
        }
    }
    else {
        float final_makespan = calc_makespan(calc_path_cost(num_tasks, path1, cost1), calc_path_cost(num_tasks, path2, cost2));
        float final_sum_of_costs = calc_sum_of_costs(calc_path_cost(num_tasks, path1, cost1), calc_path_cost(num_tasks, path2, cost2));
        std::cout << "Final change in makespan: " << final_makespan - initial_makespan << std::endl;
        std::cout << "Final change sum-of-costs: " << final_sum_of_costs - initial_sum_of_costs << std::endl;
    }

    return result; // return the result of the current swap
}