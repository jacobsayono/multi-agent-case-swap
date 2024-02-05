#include "../include/node.h"

#include <iostream>
#include <vector>

Node::Node(const std::vector<std::vector<std::vector<float>>>& costs, const std::vector<std::vector<int>>& assignment) : costs(costs), assignment(assignment), makespan(0), sumOfCosts(0) {
    std::cout << "[New node created]" << std::endl;
    for (int i = 0; i < assignment.size(); ++i) {
        std::cout << "robot " << i << ":" << std::endl;
        for (int j = 0; j <assignment[i].size(); ++j) {
            std::cout << assignment[i][j] << " ";
        }
        std::cout << std::endl;
    }

    calcMetrics();
}

void Node::calcMetrics() {
    for (int robot = 0; robot < assignment.size(); ++robot) {
        float robotCost = 0;  // reset cost for each robot iteration
        int prevTask = 0;  // robot itself can be represented by 0

        for (int task = 0; task < assignment[robot].size(); ++task) {
            int currentTask = assignment[robot][task];
            robotCost += costs[robot][prevTask][currentTask];
            sumOfCosts += costs[robot][prevTask][currentTask];
            prevTask = currentTask;
        }

        if (robotCost > makespan) {
            makespan = robotCost;
        }
    }
    std::cout << "Makespan: " << makespan << std::endl;
    std::cout << "Sum of costs: " << sumOfCosts << std::endl;
}

float Node::getMakespan() const {
    return makespan;
}

float Node::getSumOfCosts() const {
    return sumOfCosts;
}

void Node::print() const {
    std::cout << "Tree search complete!" << std::endl;
    std::cout << "Best makespan: " << makespan << std::endl;
    std::cout << "Best sumOfCosts: " << sumOfCosts << std::endl;
    std::cout << std::endl << "Here's the task assignment configuration:" << std::endl;
    for (size_t i = 0; i < assignment.size(); ++i) {
        std::cout << "robot " << i << ":" << std::endl;
        for (size_t j = 0; j < assignment[i].size(); ++j) {
            std::cout << assignment[i][j] << " ";
        }
        std::cout << "\n";
    }
}