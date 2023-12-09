#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <vector>
#include <cmath>
#include <string>

struct Position {
    int x, y;
};

int manhattan_distance(const Position& p1, const Position& p2);
void print_grid(const std::vector<Position>& robots, const std::vector<Position>& tasks, int grid_width, int grid_height);
void cost_matrix(const Position& robot, const std::vector<Position>& tasks);

#endif // POSITION_H