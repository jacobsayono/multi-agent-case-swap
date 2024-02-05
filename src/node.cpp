#include "../include/node.h"

#include <iostream>
#include <vector>

Node::Node(const std::vector<std::vector<int>>& assignment) : assignment(assignment) {
    std::cout << "new node created" << std::endl;
    for (int i = 0; i < assignment.size(); ++i) {
        std::cout << "robot " << i << std::endl;
        for (int j = 0; j <assignment[i].size(); ++j) {
            std::cout << assignment[i][j] << " ";
        }
        std::cout << std::endl;
    }
}