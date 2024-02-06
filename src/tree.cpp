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

AssignmentTree::AssignmentTree(const std::vector<std::vector<std::vector<float>>>& costs, const std::vector<std::vector<int>>& rootAssignment, int maxLevel, BuildStrategy strategy) : root(new Node(costs, rootAssignment)), costs(costs), bestMakespan(std::numeric_limits<float>::max()), bestSumOfCosts(std::numeric_limits<float>::max()) {
    /*
        - the first parameter tells the container what type of elements it will hold
        - the second parameter provides the mechanism for generating hash values from elements of that type, which is crucial for the container's internal organization, lookup, insertion, and deletion operations
    */
    std::unordered_set<std::vector<std::vector<int>>, VecVecHash> seenAssignments;

    seenAssignments.insert(rootAssignment); // Add the root assignment

    if (strategy == DFS) {
        std::cout << "GENERATING DEPTH-FIRST-SEARCH TREE..." << std::endl;
        buildTreeDFS(root, 0, maxLevel, seenAssignments);
    } else if (strategy == BFS) {
        std::cout << "GENERATING BREADTH-FIRST-SEARCH TREE..." << std::endl << std::endl;
        buildTreeBFS(root, maxLevel, seenAssignments);
    }
}

AssignmentTree::~AssignmentTree() {
    deleteTree(root);
}

Node* AssignmentTree::createNode(const std::vector<std::vector<int>>& assignment) {
    Node* newNode = new Node(costs, assignment);

    // tracking best makespan (primary) and sumOfCosts (secondary)
    float newNodeMakespan = newNode->getMakespan();
    float newNodeSumOfCosts = newNode->getSumOfCosts();
    if (newNodeMakespan < bestMakespan || (newNodeMakespan == bestMakespan && newNodeSumOfCosts < bestSumOfCosts)) {
        bestMakespan = newNodeMakespan;
        bestSumOfCosts = newNodeSumOfCosts;
        bestNode = newNode;
    }
    return newNode;
}

void AssignmentTree::buildTreeDFS(Node* node, int currentLevel, int maxLevel, std::unordered_set<std::vector<std::vector<int>>, VecVecHash>& seenAssignments) {
    if (currentLevel == maxLevel) {
        std::cout << "╚════════════ max layer arrived ════════════╝" << std::endl;
        return;
    }
    std::cout << std::endl << "╔════════════════ new layer ════════════════╗" << std::endl;
    for (int fromRobot = 0; fromRobot < node->assignment.size(); ++fromRobot) {  // iterate thru number of robots in multi-agent system
        for (int taskIndex = 0; taskIndex < node->assignment[fromRobot].size(); ++taskIndex) {  // iterate thru number of tasks in fromRobot
            for (int toRobot = 0; toRobot < node->assignment.size(); ++toRobot) {  // iterate thru number of other robots
                if (toRobot != fromRobot) {  // because at fromRobot = 1, toRobot = 0 must be checked. and we skip iteration when they overlap
                    for (int insertPos = 0; insertPos <= node->assignment[toRobot].size(); ++insertPos) {  // iterate thru number of tasks in robot k
                        // first create a copy of the current assignment to modify
                        std::vector<std::vector<int>> newAssignment = node->assignment;
                        // modify the copied assignment
                        insertTask(newAssignment, fromRobot, taskIndex, toRobot, insertPos);

                        // check if the new assignment is already seen
                        if (seenAssignments.find(newAssignment) == seenAssignments.end()) {  // if not found, find() returns an iterator equal to end(), indicating the element is absent, so proceed
                            Node* newNode = createNode(newAssignment);

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

    nodeQueue.push(std::make_pair(node, 0));  // pair helps to identify the node's layer

    int currentProcessingLevel = -1; // initialize to -1 to ensure the first level (0) is logged


    while (!nodeQueue.empty()) {
        std::pair<Node*, int> front = nodeQueue.front();
        nodeQueue.pop();
        Node* currentNode = front.first;
        int currentLevel = front.second;

        // check for new level
        if (currentLevel > currentProcessingLevel) {
            currentProcessingLevel = currentLevel;
            std::cout << "╔════════════ current layer: " << currentProcessingLevel + 1 << " ════════════╗" << std::endl;
        }

        if (currentLevel >= maxLevel) {
            continue; // skip further processing if maxLevel is reached
        }
        for (int fromRobot = 0; fromRobot < currentNode->assignment.size(); ++fromRobot) {  // iterate thru number of robots in multi-agent system
            for (int taskIndex = 0; taskIndex < currentNode->assignment[fromRobot].size(); ++taskIndex) {  // iterate thru number of tasks in fromRobot
                for (int toRobot = 0; toRobot < currentNode->assignment.size(); ++toRobot) {  // iterate thru number of other robots
                    if (toRobot != fromRobot) {  // because at robot i = 1, robot k = 0 must be checked. then skip iteration when k == i
                        for (int insertPos = 0; insertPos <= currentNode->assignment[toRobot].size(); ++insertPos) {  // iterate thru number of tasks in robot k
                            // first create a copy of the current assignment to modify
                            std::vector<std::vector<int>> newAssignment = currentNode->assignment;
                            // modify the copied assignment
                            insertTask(newAssignment, fromRobot, taskIndex, toRobot, insertPos);

                            // check if this new assignment has already been seen
                            if (seenAssignments.insert(newAssignment).second) {
                                // if not seen, create a new node, add it to the queue and seenAssignments
                                Node* newNode = createNode(newAssignment);

                                currentNode->children.push_back(newNode);
                                nodeQueue.push(std::make_pair(newNode, currentLevel + 1));
                                seenAssignments.insert(newAssignment); // mark this new assignment as seen
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

Node* AssignmentTree::getBestNode() const {
    return bestNode;
}