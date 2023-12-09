#ifndef CALC_H
#define CALC_H

#include <iostream>
#include <vector>  // <list> for paths would be better since we can insert/remove at O(1)
#include <algorithm>
#include <chrono>  // benchmark performance

// calculate the total cost of a path for a robot
float calc_path_cost(int num_tasks, const std::vector<int>& path, const std::vector<std::vector<float>>& cost_matrix);

// calculate the makespan, i.e., the longest path cost among all paths
float calc_makespan(float path_cost1, float path_cost2);

// calculate the sum of costs of all paths in a mission
float calc_sum_of_costs(float path_cost1, float path_cost2);

// calculate the new path cost after a potential task swap
float calc_swapped_path_cost(const std::vector<int>& original_path, const std::vector<std::vector<float>>& cost_matrix,
                             int num_tasks, int task_to_remove, int task_to_insert, int insert_pos);


#endif // CALC_H