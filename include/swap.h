#ifndef SWAP_H
#define SWAP_H

#include <iostream>
#include <vector>  // <list> for paths would be better since we can insert/remove at O(1)
#include <algorithm>
#include <chrono>  // benchmark performance

struct SwapResult {
    int task_to_swap;
    float makespan_diff;
    float sum_of_costs_diff;
    int swap_path;
};

SwapResult evaluate_task_swap(int num_tasks, const std::vector<int>& path_from, const std::vector<int>& path_to, const std::vector<std::vector<float>>& cost_from,const std::vector<std::vector<float>>& cost_to,
                              int task_to_swap, float initial_makespan, float initial_sum_of_costs, float& best_makespan_diff, float& best_sum_of_costs_diff, int& best_insert_position, int from_path_id);
SwapResult one_swap(int num_robots, int num_tasks, std::vector<std::vector<int>>& path, const std::vector<std::vector<std::vector<float>>>& cost);
SwapResult k_swap(int num_robots, int num_tasks, std::vector<std::vector<int>>& path, const std::vector<std::vector<std::vector<float>>>& cost, int k, const float initial_makespan, const float initial_sum_of_costs);

#endif // SWAP_H