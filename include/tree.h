#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <vector>
#include <unordered_set>

class AssignmentTree {
public:
    AssignmentTree(const std::vector<std::vector<int>>& initialAssignment, int maxLevel);
    ~AssignmentTree();

private:
    struct Node {
        std::vector<std::vector<int>> assignment;  // convert to a string format (set?)
        std::vector<Node*> children;

        float makespan;
        float sum_of_costs;

        Node(const std::vector<std::vector<int>>& assignment);  // initializer list
    };

    // hash function for a vector of vectors (the assignment configuration)
    struct VecVecHash {
        size_t operator()(const std::vector<std::vector<int>>& vv) const {
            size_t hashValue = 0;
            for (size_t i = 0; i < vv.size(); ++i) {
                for (size_t j = 0; j < vv[i].size(); ++j) {
                    // combine the hash of the current number with the current hash value
                    hashValue = hashValue * 31 + std::hash<int>()(vv[i][j]);  // 31 prime number
                }
            }
            return hashValue;
        }
    };


    Node* root;

    void buildTree(Node* node, int currentLevel, int maxLevel, std::unordered_set<std::vector<std::vector<int>>, VecVecHash>& seenAssignments);
    void insertTask(std::vector<std::vector<int>>& assignment, int fromRobot, int taskIndex, int toRobot, int insertPos);
    void deleteTree(Node* node);
    void compareWithSavedConfig();

    float getMakespan();
    float getSumOfCosts();
};

#endif  // TREE_H