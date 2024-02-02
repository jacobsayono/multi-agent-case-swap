#include "../include/tree.h"

int main() {
    int layer = 2;

    std::vector<std::vector<int>> rootAssignment = {{1},{2}};
    // std::vector<std::vector<int>> rootAssignment = {{1},{2},{3}};
    // std::vector<std::vector<int>> rootAssignment = {{1,2},{3,4}};
    // std::vector<std::vector<int>> rootAssignment = {{1,2},{3,4},{5,6}};

    AssignmentTree tree(rootAssignment, layer, AssignmentTree::DFS);
    // AssignmentTree tree(rootAssignment, layer, AssignmentTree::BFS);
}