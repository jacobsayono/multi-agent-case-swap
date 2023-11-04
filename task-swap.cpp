#include <iostream>
#include <vector>  // <list> for paths would be better since we can insert/remove at O(1)
#include <algorithm>



// one-swap (robot1 give ONE task to robot2)
// recursive one task assignment

// two-swap (robot1 give ONE task to robot2 but also receives ONE task in return, or robot1 gives TWO tasks to robot2)

// k-swap



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
    return std::min(r1, r2);
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
    std::vector<int>::iterator it = std::find(new_path.begin(), new_path.end(), task_to_remove);
    if (it != new_path.end()) {
        new_path.erase(it);
    }

    // insert task_to_insert at insert_pos
    if (insert_pos <= new_path.size()) {
        new_path.insert(new_path.begin() + insert_pos, task_to_insert);
    }
    else {
        // not sure what to do, but surely we won't have a case where there is an invalid insert position
        std::cout << "something wrong with insert pos" << std::endl;
    }

    // return new_path cost
    return calc_path_cost(num_tasks, new_path, cost);
}

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

// function to swap tasks if it's optimal. returns an array for the swapped task number, makespan diff, and sum_of_costs diff
float* check_swap(int num_tasks, std::vector<int>& path1, std::vector<int>& path2, float cost1[], float cost2[]) {
    // get the cost of each paths
    float path1_cost = calc_path_cost(num_tasks, path1, cost1);
    float path2_cost = calc_path_cost(num_tasks, path2, cost2);

    // get the initial makespan and sum_of_costs
    float makespan = calc_makespan(path1_cost, path2_cost);
    float sum_of_costs = calc_sum_of_costs(path1_cost, path2_cost);

    // initialize array to return
    static float swap_result[3] = {-1, 0, 0};  // if no swap (original paths were already optimal), then return this array

    // track the best swap found
    float best_swap_makespan = makespan;
    float best_swap_sum_of_costs = sum_of_costs;
    float best_task_to_swap = -1;
    float best_path_to_insert = -1;
    float best_insert_position = -1;

    // _ -> 1 -> _ -> 2 -> _

    // iterate through all tasks in path1 and try to move them to path2
    for (int i = 0; i < path1.size(); ++i) {
        int task = path1[i];
        for (int pos = 0; pos <= path2.size(); ++pos) {
            float new_path1_cost = calc_swapped_path_cost(path1, cost1, num_tasks, task, -1, -1);
            float new_path2_cost = calc_swapped_path_cost(path2, cost2, num_tasks, -1, task, pos);
            float new_makespan = calc_makespan(new_path1_cost, new_path2_cost);
            float new_sum_of_costs = calc_sum_of_costs(new_path1_cost, new_path2_cost);

            // check if this i-th swap improves makespan
            if (new_makespan < best_swap_makespan
            // || new_makespan == best_swap_makespan && new_sum_of_costs < best_swap_sum_of_costs
            ) {
                best_swap_makespan = new_makespan;
                best_swap_sum_of_costs = new_sum_of_costs;
                best_task_to_swap = task;
                best_path_to_insert = 1;  // insert into path2
                best_insert_position = pos;
            }
        }
    }

    // iterate through all tasks in path2 and try to move them to path1
    for (int i = 0; i < path2.size(); ++i) {
        int task = path2[i];
        for (int pos = 0; pos <= path1.size(); ++pos) {
            float new_path1_cost = calc_swapped_path_cost(path1, cost1, num_tasks, -1, task, pos);
            float new_path2_cost = calc_swapped_path_cost(path2, cost2, num_tasks, task, -1, -1);
            float new_makespan = calc_makespan(new_path1_cost, new_path2_cost);
            float new_sum_of_costs = calc_sum_of_costs(new_path1_cost, new_path2_cost);

            // check if this i-th swap improves makespan
            if (new_makespan < best_swap_makespan
            // || new_makespan == best_swap_makespan && new_sum_of_costs < best_swap_sum_of_costs
            ) {
                best_swap_makespan = new_makespan;
                best_swap_sum_of_costs = new_sum_of_costs;
                best_task_to_swap = task;
                best_path_to_insert = 0;  // insert into path2
                best_insert_position = pos;
            }
        }
    }

    // if a swap was found that improves makespan
    if (best_task_to_swap != -1) {
        // perform the swap
        if (best_path_to_insert == 0) {
            path1.erase(std::remove(path1.begin(), path1.end(), best_task_to_swap), path1.end());
            path2.insert(path2.begin() + best_insert_position, best_task_to_swap);
        }
        else {  // best_path_to_insert == 1
            path2.erase(std::remove(path2.begin(), path2.end(), best_task_to_swap), path2.end());
            path1.insert(path1.begin() + best_insert_position, best_task_to_swap);
        }

        // update the return array
        swap_result[0] = best_task_to_swap;
        swap_result[1] = best_swap_makespan - makespan;  // change in makespan
        swap_result[2] = best_swap_sum_of_costs - sum_of_costs;  // change in sum_of_costs
    }

    // return the results of the swap
    return swap_result;
    
    
    
    // Example changing task assignment
    // int old_ind = 0;
    // int new_ind = 1;
    // path_r2->insert(path_r2->begin()+new_ind,(*path_r1)[old_ind]);
    // path_r1->erase(path_r1->begin()+old_ind);

    // // Return the following array if no swap is beneficial
    // static float swap[3] = {-1, 0, 0};

    // return swap;
}


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

    // Use a check swaps function to see if a swap is desirable
    float* arr;
    arr = check_swap(num_tasks, path_r1, path_r2, cost_r1, cost_r2);

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
    std::cout << "Best task to swap: " << *(arr+0) << std::endl;
    std::cout << "Change in makespan: " << *(arr+1) << std::endl;
    std::cout << "Change in sum-of-costs: " << *(arr+2) << std::endl;
}