#ifndef TREE_H
#define TREE_H

#include "node.h"

#include <vector>
#include <unordered_set>
#include <limits>

struct VecVecHash;  // forward declaration

class AssignmentTree {
public:
    enum BuildStrategy {  // if want to customize each version within AssignmentTree constructor, use inheritance
        DFS,
        BFS
    };

    AssignmentTree(const std::vector<std::vector<std::vector<float>>>& costs, const std::vector<std::vector<int>>& initialAssignment, int maxLevel, BuildStrategy strategy);
    ~AssignmentTree();

    Node* createNode(const std::vector<std::vector<int>>& assignment);
    void buildTreeDFS(Node* node, int currentLevel, int maxLevel, std::unordered_set<std::vector<std::vector<int>>, VecVecHash>& seenAssignments);
    void buildTreeBFS(Node* node, int maxLevel, std::unordered_set<std::vector<std::vector<int>>, VecVecHash>& seenAssignments);
    void insertTask(std::vector<std::vector<int>>& assignment, int fromRobot, int taskIndex, int toRobot, int insertPos);
    void deleteTree(Node* node);

    // tracking best makespan and sumOfCosts
    Node* getBestNode() const;

private:
    Node* root;
    const std::vector<std::vector<std::vector<float>>>& costs; // pointer to the costs matrix so that node can access it at the time of node construction

    // tracking best makespan and sumOfCosts
    Node* bestNode;
    float bestMakespan;
    float bestSumOfCosts;
};

#endif  // TREE_H