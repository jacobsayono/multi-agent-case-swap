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
    the tree builds in a BFS manner, cuz it creates layer by layer, and node by node in each layer
*/

/*
    traverse the tree using either:
        - DFS
        - BFS
*/

/*
    keep track of lowest makespan as we build the tree.
*/

AssignmentTree::AssignmentTree(const std::vector<std::vector<int>>& initialAssignment, int maxLevel) {
    std::unordered_set<std::vector<std::vector<int>>, VecVecHash> seenAssignments;
    /*
        - the first parameter tells the container what type of elements it will hold
        - the second parameter provides the mechanism for generating hash values from elements of that type, which is crucial for the container's internal organization, lookup, insertion, and deletion operations
    */

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
                        // first create a copy of the current assignment to modify
                        std::vector<std::vector<int>> newAssignment = node->assignment;
                        // modify the copied assignment
                        insertTask(newAssignment, i, j, k, l);

                        // check if the new assignment is already seen
                        if (seenAssignments.find(newAssignment) == seenAssignments.end()) {  // if not found, find() returns an iterator equal to end(), indicating the element is absent, so proceed
                            Node* newNode = new Node(node->assignment);
                            node->children.push_back(newNode);
                            seenAssignments.insert(newAssignment);  // mark this new assignment as seen

                            // recursively build the tree from the new node, DFS
                            buildTree(newNode, currentLevel + 1, maxLevel, seenAssignments);
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
        assignment[toRobot].push_back(task);  // place at last spot
    }
}

void AssignmentTree::deleteTree(Node* node) {
    for (Node* child : node->children) {  // make sure to delete every dynamically allocated node once we traverse to find lowest makespan & sum_of_costs
        deleteTree(child);
    }
    delete node;
}



/*

#include <queue>

void AssignmentTree::buildTreeBFS(const std::vector<std::vector<int>>& initialAssignment, int maxLevel) {
    std::unordered_set<std::vector<std::vector<int>>, VecVecHash> seenAssignments;
    std::queue<std::pair<Node*, int>> nodeQueue; // Queue to hold nodes and their current levels

    // Create the root node and add it to the queue
    root = new Node(initialAssignment);
    nodeQueue.push(std::make_pair(root, 0));
    seenAssignments.insert(initialAssignment);

    while (!nodeQueue.empty()) {
        auto front = nodeQueue.front();
        nodeQueue.pop();
        Node* currentNode = front.first;
        int currentLevel = front.second;

        if (currentLevel == maxLevel) continue; // Skip further processing if maxLevel is reached

        for (size_t i = 0; i < currentNode->assignment.size(); ++i) {
            for (size_t j = 0; j < currentNode->assignment[i].size(); ++j) {
                for (size_t k = 0; k < currentNode->assignment.size(); ++k) {
                    if (k != i) {
                        for (size_t l = 0; l <= currentNode->assignment[k].size(); ++l) {
                            std::vector<std::vector<int>> newAssignment = currentNode->assignment;
                            insertTask(newAssignment, i, j, k, l); // Modify the assignment as per your logic

                            // Check if this new assignment has already been seen
                            if (seenAssignments.find(newAssignment) == seenAssignments.end()) {
                                // If not seen, create a new node, add it to the queue and seenAssignments
                                Node* newNode = new Node(newAssignment);
                                currentNode->children.push_back(newNode);
                                nodeQueue.push(std::make_pair(newNode, currentLevel + 1));
                                seenAssignments.insert(newAssignment); // Mark as seen
                            }
                        }
                    }
                }
            }
        }
    }
}

*/