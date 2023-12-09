#include "../include/calc.h"

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