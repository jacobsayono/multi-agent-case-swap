#ifndef NODE_H
#define NODE_H

#include <vector>

class Node {
public:
    Node(const std::vector<std::vector<std::vector<float>>>& costs, const std::vector<std::vector<int>>& assignment);

    void calcMetrics();  // calc makespan and sumOfCosts
    float getMakespan() const;
    float getSumOfCosts() const;

private:
    friend class AssignmentTree;

    std::vector<std::vector<int>> assignment;
    std::vector<Node*> children;
    
    const std::vector<std::vector<std::vector<float>>>& costs; // reference to the costs matrix via tree class
    float makespan;
    float sumOfCosts;
};

#endif  /* NODE_H */