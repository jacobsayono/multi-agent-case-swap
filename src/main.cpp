#include "../include/tree.h"

int main() {
  /* cost matrix for each robot (number of robots and tasks must match with rootAssignment) */
  std::vector<std::vector<std::vector<float>>> costs = {{  // robot0
                                                          {0, 1, 2.5, 1.5},
                                                          {1, 0, 2, 2},
                                                          {2.5, 2, 0, 2},
                                                          {1.5, 2, 2, 0}},
                                                        { // robot1
                                                          {0, 3, 1.2, 3},
                                                          {3, 0, 2, 2},
                                                          {1.2, 2, 0, 2},
                                                          {3, 2, 2, 0}}};

  /* initial task assignment configuration */
  std::vector<std::vector<int>> rootAssignment = {{1},{2}};
  // std::vector<std::vector<int>> rootAssignment = {{1},{2},{3}};
  // std::vector<std::vector<int>> rootAssignment = {{1,2},{3,4}};
  // std::vector<std::vector<int>> rootAssignment = {{1,2},{3,4},{5,6}};

  /* depth search value */
  int layer = 2;

  /* generate all possible assignment configuration */
  AssignmentTree tree(rootAssignment, layer, AssignmentTree::BFS);
  // AssignmentTree tree(rootAssignment, layer, AssignmentTree::BFS);
}