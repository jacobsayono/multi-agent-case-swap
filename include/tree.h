#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <vector>
#include <iostream>

class AssignmentTree {
public:
    AssignmentTree(const std::vector<std::vector<int>>& initialAssignment, int maxLevel);
    ~AssignmentTree();

private:
    struct Node {
        std::vector<std::vector<int>> assignment;
        std::vector<Node*> children;

        /*
        float makespan;
        float sum_of_costs;
        */

        Node(const std::vector<std::vector<int>>& assignment);  // initializer list
    };

    Node* root;

    void buildTree(Node* node, int currentLevel, int maxLevel);
    void insertTask(std::vector<std::vector<int>>& assignment, int fromRobot, int taskIndex, int toRobot, int insertPos);
    void deleteTree(Node* node);
};

#endif  // TREE_H