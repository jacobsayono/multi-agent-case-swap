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

        if (makespan_diff <= 0 && makespan_diff < best_makespan_diff || (makespan_diff == 0 && (sum_of_costs_diff <= 0 && sum_of_costs_diff < best_sum_of_costs_diff))) {
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
SwapResult one_swap(int num_robots, int num_tasks, std::vector<std::vector<int>>& path, const std::vector<std::vector<std::vector<float>>>& cost) {
    SwapResult potential_swap;
    float best_makespan_diff = std::numeric_limits<float>::max();  // highest possible number to compare to (basically infinity)
    float best_sum_of_costs_diff = std::numeric_limits<float>::max();  // highest possible number to compare to (basically infinity)
    int best_task_to_swap = -1;
    int best_swap_path = -1;
    int best_insert_position = -1;
    for (int r1 = 0; r1 < num_robots - 1; ++r1) {
        for (int r2 = r1 + 1; r2 < num_robots; ++r2) {
            float initial_makespan = calc_makespan(calc_path_cost(num_tasks, path[r1], cost[r1]), calc_path_cost(num_tasks, path[r2], cost[r2]));
            float initial_sum_of_costs = calc_sum_of_costs(calc_path_cost(num_tasks, path[r1], cost[r1]), calc_path_cost(num_tasks, path[r2], cost[r2]));

            // evaluate potential swaps from path1 to path2
            for (int i = 0; i < path[r1].size(); ++i) {
                int task_to_swap = path[r1][i];
                // evaluate potential swap
                SwapResult potential_swap = evaluate_task_swap(num_tasks, path[r1], path[r2], cost[r1], cost[r2], task_to_swap, initial_makespan, initial_sum_of_costs, best_makespan_diff, best_sum_of_costs_diff, best_insert_position, 1);
                // update best swap if the potential swap is better
                if (potential_swap.task_to_swap != -1) {
                    best_task_to_swap = task_to_swap;
                    best_swap_path = 1;
                }
            }

            // evaluate potential swaps from path2 to path1
            for (int i = 0; i < path[r2].size(); ++i) {
                int task_to_swap = path[r2][i];
                // evaluate potential swap
                SwapResult potential_swap = evaluate_task_swap(num_tasks, path[r2], path[r1], cost[r2], cost[r1], task_to_swap, initial_makespan, initial_sum_of_costs, best_makespan_diff, best_sum_of_costs_diff, best_insert_position, 2);
                // update best swap if the potential swap is better
                if (potential_swap.task_to_swap != -1) {
                    best_task_to_swap = task_to_swap;
                    best_swap_path = 2;
                }
            }


            // perform the best swap if found
            if (best_task_to_swap != -1) {
                std::cout << "Best task to swap: " << best_task_to_swap << std::endl;
                
                // use iterator to safely erase in the vector range
                if (best_task_to_swap == 1) {
                    std::vector<int>::iterator it = std::remove(path[r1].begin(), path[r1].end(), best_task_to_swap);
                    if (it != path[r1]. end()) {
                        path[r1].erase(it, path[r1].end());
                    }
                    if (best_insert_position <= path[r2].size()) {
                        path[r2]. insert(path[r2].begin() + best_insert_position, best_task_to_swap);
                    }
                }
                else {
                    std::vector<int>::iterator it = std::remove(path[r2].begin(), path[r2].end(), best_task_to_swap);
                    if (it != path[r2].end()) {
                        path[r2].erase(it, path[r2].end());
                    }
                    if (best_insert_position <= path[r1].size()) {
                        path[r1].insert(path[r1].begin() + best_insert_position, best_task_to_swap);
                    }
                }
            }
            else {
                best_makespan_diff = 0;
                best_sum_of_costs_diff = 0;
                std::cout << "Current task assignments are already most optimal; no further swaps necessary!";
                return SwapResult{best_task_to_swap, best_makespan_diff, best_sum_of_costs_diff};
            }

            // print task assignments
            std::cout << "Printing robot " << static_cast<char>('A' + r1) << " assignment:";
            for (int i=0; i<path[r1].size(); i++) {
                std::cout << " " << path[r1][i];
            }
            std::cout << std::endl;
            std::cout << "Printing robot " << static_cast<char>('A' + r2) << " assignment:";
            for (int i=0; i<path[r2].size(); i++) {
                std::cout << " " << path[r2][i];
            }
            std::cout << std::endl;

            // calculate and print the final path costs, makespan, and sum-of-costs after the swap
            float final_path1_cost = calc_path_cost(num_tasks, path[r1], cost[r1]);
            float final_path2_cost = calc_path_cost(num_tasks, path[r2], cost[r2]);
            float final_makespan = calc_makespan(final_path1_cost, final_path2_cost);
            float final_sum_of_costs = calc_sum_of_costs(final_path1_cost, final_path2_cost);

            std::cout << "Current robot " << static_cast<char>('A' + r1) << " path cost: " << final_path1_cost << std::endl;
            std::cout << "Current robot " << static_cast<char>('A' + r2) << " path cost: " << final_path2_cost << std::endl;
            std::cout << "Current makespan: " << final_makespan << std::endl;
            std::cout << "Current sum-of-costs: " << final_sum_of_costs << std::endl;
            std::cout << "Current change in makespan: " << best_makespan_diff << std::endl;
            std::cout << "Current change in sum-of-costs: " << best_sum_of_costs_diff;
        }
    }
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
SwapResult k_swap(int num_robots, int num_tasks, std::vector<std::vector<int>>& path, const std::vector<std::vector<std::vector<float>>>& cost, int k, const float initial_makespan, const float initial_sum_of_costs) {
    if (k <= 0) {
        std::cout << std::endl;
        std::cout << "There may be more optimal solutions. Please set k to a higher value if needed!" << std::endl;
        return SwapResult{-1, 0.0f, 0.0f, -1}; // no swap needed or possible
    }

    static int i = 1;
    std::cout << std::endl;
    std::cout << "Depth = " << i << std::endl;
    i++;

    SwapResult result;
    for (int r1 = 0; r1 < num_robots - 1; ++r1) {
        for (int r2 = r1 + 1; r2 < num_robots; ++r2) {
            // perform one swap
            SwapResult result = one_swap(num_robots, num_tasks, path, cost);
            std::cout << std::endl;

            // if a swap was made, recurse with k-1
            if (result.task_to_swap != -1) {
                SwapResult recursiveResult = k_swap(num_robots, num_tasks, path, cost, k - 1, initial_makespan, initial_sum_of_costs);
                // choose the result with the best makespan or sum_of_costs improvement
                if (recursiveResult.makespan_diff < result.makespan_diff || 
                    (recursiveResult.makespan_diff == result.makespan_diff && recursiveResult.sum_of_costs_diff < result.sum_of_costs_diff)) {
                    return recursiveResult;
                }
            }
            else {
                float final_makespan = calc_makespan(calc_path_cost(num_tasks, path[r1], cost[r1]), calc_path_cost(num_tasks, path[r2], cost[r2]));
                float final_sum_of_costs = calc_sum_of_costs(calc_path_cost(num_tasks, path[r1], cost[r1]), calc_path_cost(num_tasks, path[r2], cost[r2]));
                std::cout << "Final change in makespan: " << final_makespan - initial_makespan << std::endl;
                std::cout << "Final change sum-of-costs: " << final_sum_of_costs - initial_sum_of_costs << std::endl;
            }
        }
    }
    return result; // return the result of the current swap
}