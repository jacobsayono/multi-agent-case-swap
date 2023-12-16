#include "../include/grid.h"

// distance between two points (cannot go diagonally)
int manhattan_distance(const Position& p1, const Position& p2) {
    return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}

// visualize grid or robots, tasks, and empty spaces
void print_grid(const std::vector<Position>& robots, const std::vector<Position>& tasks, int grid_width, int grid_height) {
    // place empty spaces (.) on grid
    std::vector<std::string> grid(grid_height, std::string(grid_width, '.'));

    // place robots (X) on grid
    for (int i = 0; i < robots.size(); ++i) {
        if (robots[i].x >= 0 && robots[i].x < grid_width && robots[i].y >= 0 && robots[i].y < grid_height) {
            // grid[robots[i].y][robots[i].x] = 'X';
            grid[robots[i].y][robots[i].x] = 'A' + i;
        }
    }

    // place tasks (O) on grid
    for (int i = 0; i < tasks.size(); ++i) {
        if (tasks[i].x >= 0 && tasks[i].x < grid_width && tasks[i].y >= 0 && tasks[i].y < grid_height) {
            // grid[tasks[i].y][tasks[i].x] = 'O';
            grid[tasks[i].y][tasks[i].x] = '1' + i;
        }
    }

    // print grid
    for (int i = 0; i < grid.size(); ++i) {
        std::cout << grid[i] << std::endl;
    }
}

// generate cost matrix from each manhattan distance between:
// - robot & task
// - task & task
std::vector<std::vector<float>> generate_cost_matrix(const Position& robot, const std::vector<Position>& tasks) {
    int grid_size = tasks.size() + 1;
    std::vector<std::vector<float>> cost_matrix(grid_size, std::vector<float>(grid_size, 0));

    cost_matrix[0][0] = manhattan_distance(robot, robot);
    for (int j = 0; j < tasks.size(); ++j) {
        cost_matrix[0][j + 1] = manhattan_distance(robot, tasks[j]);
        cost_matrix[j + 1][0] = manhattan_distance(tasks[j], robot);
        for (int k = 0; k < tasks.size(); ++k) {
            cost_matrix[j + 1][k + 1] = manhattan_distance(tasks[j], tasks[k]);
        }
    }

    return cost_matrix;
}

// print out the stored 2D vector to check calculations
void print_cost_matrix(const std::vector<std::vector<float>>& cost_matrix, const Position& robot) {
    for (int row = 0; row < cost_matrix.size(); ++row) {
        for (int col = 0; col < cost_matrix[row].size(); ++col) {
            std::cout << cost_matrix[row][col] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}