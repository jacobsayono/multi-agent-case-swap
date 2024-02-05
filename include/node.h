#ifndef NODE_H
#define NODE_H

#include <vector>

class Node {
public:
    Node(const std::vector<std::vector<int>>& assignment);  // initializer list

    float getMakespan() const;
    float getSumOfCosts() const;

private:
    friend class AssignmentTree;

    std::vector<std::vector<int>> assignment;
    std::vector<Node*> children;

    float makespan;
    float sum_of_costs;
};

#endif  /* NODE_H */