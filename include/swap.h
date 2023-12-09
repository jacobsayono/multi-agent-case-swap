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

SwapResult one_swap(int num_tasks, std::vector<int>& path1, std::vector<int>& path2, const float cost1[], const float cost2[]);
SwapResult k_swap(int num_tasks, std::vector<int>& path1, std::vector<int>& path2, const float cost1[], const float cost2[], int k);

#endif // SWAP_H