#include "../include/tree.h"

#include <iostream>
#include <queue>
#include <vector>
#include <unordered_set>

struct VecVecHash {
    // hash function that makes vector<vector<int>> (the assignment configuration) computable for a hash value
    size_t operator()(const std::vector<std::vector<int>>& vv) const {
        size_t hashValue = 0;
        for (size_t i = 0; i < vv.size(); ++i) {
            for (size_t j = 0; j < vv[i].size(); ++j) {
                // combine the hash of the current number with the current hash value
                hashValue = hashValue * 31 + std::hash<int>()(vv[i][j]);  // 31 prime number, close to 2^5 = 32
            }
        }
        return hashValue;  // essentially converts vector<vector<int>> assignment into a hash value for the unordered_set
    }
};

AssignmentTree::AssignmentTree(const std::vector<std::vector<std::vector<float>>>& costs, const std::vector<std::vector<int>>& rootAssignment, int maxLevel, BuildStrategy strategy) : root(new Node(costs, rootAssignment)), costs(costs) {
    /*
        - the first parameter tells the container what type of elements it will hold
        - the second parameter provides the mechanism for generating hash values from elements of that type, which is crucial for the container's internal organization, lookup, insertion, and deletion operations
    */
    std::unordered_set<std::vector<std::vector<int>>, VecVecHash> seenAssignments;

    seenAssignments.insert(rootAssignment); // Add the root assignment

    if (strategy == DFS) {
        buildTreeDFS(root, 0, maxLevel, seenAssignments);
    } else if (strategy == BFS) {
        buildTreeBFS(root, maxLevel, seenAssignments);
    }
}

AssignmentTree::~AssignmentTree() {
    deleteTree(root);
}

void AssignmentTree::buildTreeDFS(Node* node, int currentLevel, int maxLevel, std::unordered_set<std::vector<std::vector<int>>, VecVecHash>& seenAssignments) {
    if (currentLevel == maxLevel) {
        std::cout << "============ max layer arrived ============" << std::endl << std::endl;
        return;
    }
    std::cout << std::endl;
    std::cout << "============ new layer ============" << std::endl;
    for (int i = 0; i < node->assignment.size(); ++i) {  // iterate thru number of robots in multi-agent system
        for (int j = 0; j < node->assignment[i].size(); ++j) {  // iterate thru number of tasks in robot i
            for (int k = 0; k < node->assignment.size(); ++k) {  // iterate thru number of other robots
                if (k != i) {  // because at robot i = 1, robot k = 0 must be checked. skip iteration when k == i
                    for (int l = 0; l <= node->assignment[k].size(); ++l) {  // iterate thru number of tasks in robot k
                        // first create a copy of the current assignment to modify
                        std::vector<std::vector<int>> newAssignment = node->assignment;
                        // modify the copied assignment
                        insertTask(newAssignment, i, j, k, l);

                        // check if the new assignment is already seen
                        if (seenAssignments.find(newAssignment) == seenAssignments.end()) {  // if not found, find() returns an iterator equal to end(), indicating the element is absent, so proceed
                            Node* newNode = new Node(costs, newAssignment);
                            node->children.push_back(newNode);
                            seenAssignments.insert(newAssignment);  // mark this new assignment as seen

                            // recursively build the tree from the new node, DFS
                            buildTreeDFS(newNode, currentLevel + 1, maxLevel, seenAssignments);
                        }
                    }
                }
            }
        }
    }
}


void AssignmentTree::buildTreeBFS(Node* node, int maxLevel, std::unordered_set<std::vector<std::vector<int>>, VecVecHash>& seenAssignments) {
    std::queue<std::pair<Node*, int>> nodeQueue; // queue to hold nodes and their current levels

    nodeQueue.push(std::make_pair(root, 0));

    int currentProcessingLevel = -1; // initialize to -1 to ensure the first level (0) is logged


    while (!nodeQueue.empty()) {
        std::pair<Node*, int> front = nodeQueue.front();
        nodeQueue.pop();
        Node* currentNode = front.first;
        int currentLevel = front.second;

        // Check for new level
        if (currentLevel > currentProcessingLevel) {
            currentProcessingLevel = currentLevel;
            std::cout << "============ current layer: " << currentProcessingLevel + 1 << " ============" << std::endl;
        }

        if (currentLevel >= maxLevel) {
            continue; // skip further processing if maxLevel is reached
        }
        for (size_t i = 0; i < currentNode->assignment.size(); ++i) {
            for (size_t j = 0; j < currentNode->assignment[i].size(); ++j) {
                for (size_t k = 0; k < currentNode->assignment.size(); ++k) {
                    if (k != i) {
                        for (size_t l = 0; l <= currentNode->assignment[k].size(); ++l) {
                            std::vector<std::vector<int>> newAssignment = currentNode->assignment;
                            insertTask(newAssignment, i, j, k, l); // modify the assignment as per your logic

                            // check if this new assignment has already been seen
                            if (seenAssignments.insert(newAssignment).second) {
                                // if not seen, create a new node, add it to the queue and seenAssignments
                                Node* newNode = new Node(costs, newAssignment);
                                currentNode->children.push_back(newNode);
                                nodeQueue.push(std::make_pair(newNode, currentLevel + 1));
                                seenAssignments.insert(newAssignment); // mark as seen
                            }
                        }
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
        assignment[toRobot].push_back(task);  // place at last spot; logic works well if robot has empty number of tasks
    }
}

void AssignmentTree::deleteTree(Node* node) {
    for (Node* child : node->children) {  // make sure to delete every dynamically allocated node once we traverse to find lowest makespan & sum_of_costs
        deleteTree(child);
    }
    delete node;
}