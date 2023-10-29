#include <iostream>
#include <vector>

// Function to facilitate task swaps between robots
float* checkSwap(size_t num_tasks, float cost1[], float cost2[], std::vector<int> *t1, std::vector<int> *t2) 
{
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

    // Example changing task assignment
    int old_ind = 0;
    int new_ind = 1;
    t2->insert(t2->begin()+new_ind,(*t1)[old_ind]);
    t1->erase(t1->begin()+old_ind);

    // Return the following array is no swap is beneficial
    static float swap[3] = {-1, 0, 0};

    return swap;
}


int main(int argc, char * argv[]) {
    // Start by defining a cost matrix for several robots
    size_t num_tasks = 3;
    float C1[16] = {0, 1, 2.5, 1.5, 
                    1, 0, 2, 2,
                    2.5, 2, 0, 2,
                    1.5, 2, 2, 0};
    float C2[16] = {0, 3, 1.2, 3,
                    3, 0, 2, 2,
                    1.2, 2, 0, 2,
                    3, 2, 2, 0};
    
    // Define a previous task assignment for each robot
    // Note: this is not an optimal assignment
    std::vector<int> t1 = {3,2};
    std::vector<int> t2 = {1};

    // Use a check swaps function to see if a swap is desirable
    float* delta;
    delta = checkSwap(num_tasks, C1, C2, &t1, &t2);

    // Print task assignments
    std::cout << "Printing robot 1 assignment\n";
    for (int i=0; i<t1.size(); i++) {
        std::cout << t1[i] << std::endl;
    }
    std::cout << "Printing robot 2 assignment\n";
    for (int i=0; i<t2.size(); i++) {
        std::cout << t2[i] << std::endl;
    }

    // Print the change in makespan and sum-of-costs
    std::cout << "Best task to swap: " << *(delta+0) << std::endl;
    std::cout << "Change in makespan: " << *(delta+1) << std::endl;
    std::cout << "Change in sum-of-costs: " << *(delta+2) << std::endl;
}