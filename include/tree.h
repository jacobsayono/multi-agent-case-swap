#ifndef TREE_H
#define TREE_H

#include "node.h"

#include <vector>
#include <unordered_set>

struct VecVecHash;  // forward declaration

class AssignmentTree {
public:
    enum BuildStrategy {  // if want to customize each version, implement inheritance
        DFS,
        BFS
    };

    AssignmentTree(const std::vector<std::vector<int>>& initialAssignment, int maxLevel, BuildStrategy strategy);
    ~AssignmentTree();

    void buildTreeDFS(Node* node, int currentLevel, int maxLevel, std::unordered_set<std::vector<std::vector<int>>, VecVecHash>& seenAssignments);
    void buildTreeBFS(Node* node, int maxLevel, std::unordered_set<std::vector<std::vector<int>>, VecVecHash>& seenAssignments);
    void insertTask(std::vector<std::vector<int>>& assignment, int fromRobot, int taskIndex, int toRobot, int insertPos);
    void deleteTree(Node* node);

private:
    Node* root;
};

#endif  // TREE_H