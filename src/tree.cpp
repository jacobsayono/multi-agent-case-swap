#include "../include/tree.h"

AssignmentTree::AssignmentTree(const std::vector<std::vector<int>>& initialAssignment, int maxLevel) {
    root = new Node(initialAssignment);
    buildTree(root, 0, maxLevel);
}

AssignmentTree::~AssignmentTree() {
    deleteTree(root);
}

AssignmentTree::Node::Node(const std::vector<std::vector<int>>& assignment) : assignment(assignment) {}

void AssignmentTree::buildTree(Node* node, int currentLevel, int maxLevel) {
    if (currentLevel == maxLevel) {
        return;
    }

    for (int i = 0; i < node->assignment.size(); ++i) {
        for (int j = 0; j < node->assignment[i].size(); ++j) {
            for (int k = 0; k < node->assignment.size(); ++k) {
                if (k != i) {
                    for (int l = 0; l <= node->assignment[k].size(); ++l) {
                        Node* newNode = new Node(node->assignment);
                        insertTask(newNode->assignment, i, j, k, l);
                        node->children.push_back(newNode);

                        buildTree(newNode, currentLevel + 1, maxLevel);
                    }
                }
            }
        }
    }
}

void AssignmentTree::insertTask(std::vector<std::vector<int>>& assignment, int fromRobot, int taskIndex, int toRobot, int insertPos) {
    int task = assignment[fromRobot][taskIndex];
    assignment[fromRobot].erase(assignment[fromRobot].begin() + taskIndex);

    if (insertPos <= assignment[toRobot].size()) {
        assignment[toRobot].insert(assignment[toRobot].begin() + insertPos, task);
    } else {
        assignment[toRobot].push_back(task);
    }
}

void AssignmentTree::deleteTree(Node* node) {
    for (Node* child : node->children) {
        deleteTree(child);
    }
    delete node;
}