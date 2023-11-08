#include <iostream>
#include <vector>

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
    // e.g.: R0 to T3 = 0 * (3+1) + 3 = 3
    // e.g.: T3 to T2 = 3 * (3+1) + 2 = 14

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

int main() {
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

        std::cout << "Initial r1 path cost: " << initial_path1_cost << std::endl;
        std::cout << "Initial r2 path cost: " << initial_path2_cost << std::endl;
}