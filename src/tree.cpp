#include "../include/tree.h"

/*
assignment_tree
    |_ node
        |_ robot_number
            |_ tasks_assigned -> create a child node for every task picked and placed everywhere
                |_ makespan
                |_ sum_of_costs
        |_ child_node -> rinse and repeat node structure (recursive)
*/

/*
traverse the tree using either:
    - DFS
    - BFS
*/

AssignmentTree::AssignmentTree(const std::vector<std::vector<int>>& initialAssignment, int maxLevel) {
    std::unordered_set<std::vector<std::vector<int>>, VecVecHash> seenAssignments;
    root = new Node(initialAssignment);
    seenAssignments.insert(initialAssignment); // Add the root assignment
    buildTree(root, 0, maxLevel, seenAssignments);
}

AssignmentTree::~AssignmentTree() {
    deleteTree(root);
}

AssignmentTree::Node::Node(const std::vector<std::vector<int>>& assignment) : assignment(assignment) {}

void AssignmentTree::buildTree(Node* node, int currentLevel, int maxLevel, std::unordered_set<std::vector<std::vector<int>>, VecVecHash>& seenAssignments) {
    if (currentLevel == maxLevel) {
        return;
    }

    for (int i = 0; i < node->assignment.size(); ++i) {  // iterate thru number of robots in multi-agent system
        for (int j = 0; j < node->assignment[i].size(); ++j) {  // iterate thru number of tasks in robot i
            for (int k = 0; k < node->assignment.size(); ++k) {  // iterate thru number of other robots
                if (k != i) {  // because at robot i = 1, robot k = 0 must be checked
                    for (int l = 0; l <= node->assignment[k].size(); ++l) {  // iterate thru number of tasks in robot k
                        Node* newNode = new Node(node->assignment);
                        insertTask(newNode->assignment, i, j, k, l);
                        node->children.push_back(newNode);

                        buildTree(newNode, currentLevel + 1, maxLevel, seenAssignments);
                    }
                }
            }
        }
    }
}

void AssignmentTree::insertTask(std::vector<std::vector<int>>& assignment, int fromRobot, int taskIndex, int toRobot, int insertPos) {
    int task = assignment[fromRobot][taskIndex];
    assignment[fromRobot].erase(assignment[fromRobot].begin() + taskIndex);  // first erase the task that we are currently picking out from a robot

    if (insertPos <= assignment[toRobot].size()) {
        assignment[toRobot].insert(assignment[toRobot].begin() + insertPos, task);  // then we insert into a spot
    } else {
        assignment[toRobot].push_back(task);  // place at last spot
    }
}

void AssignmentTree::deleteTree(Node* node) {
    for (Node* child : node->children) {  // make sure to delete every dynamically allocated node once we traverse to find lowest makespan & sum_of_costs
        deleteTree(child);
    }
    delete node;
}