// recursive function to perform k swaps
SwapResult k_swap(int num_tasks, std::vector<int>& path1, std::vector<int>& path2, const float cost1[], const float cost2[], int k) {
    if (k <= 0) {
        return SwapResult{-1, 0.0f, 0.0f, -1}; // no swap needed or possible
    }

    // perform one swap
    SwapResult result = one_swap(num_tasks, path1, path2, cost1, cost2);

    // if a swap was made, recurse with k-1
    if (result.task_to_swap != -1) {
        SwapResult recursiveResult = k_swap(num_tasks, path1, path2, cost1, cost2, k - 1);
        // choose the result with the best makespan or sum_of_costs improvement
        if (recursiveResult.makespan_diff < result.makespan_diff || 
            (recursiveResult.makespan_diff == result.makespan_diff && recursiveResult.sum_of_costs_diff < result.sum_of_costs_diff)) {
            return recursiveResult;
        }
    }

    return result; // return the result of the current swap
}
