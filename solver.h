#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include <functional>
#include <vector>

struct Cell;

int defaultHeuristic(int x1, int y1, int x2, int y2);
bool findPathAStar(
    std::vector<std::vector<char>> &maze, int startX, int startY, int goalX, int goalY,
   int (*heuristic)(int, int, int, int) = defaultHeuristic);
bool findPathDijkstra(std::vector<std::vector<char>> &maze, int startX, int startY, int goalX, int goalY);
bool findPathBFS(std::vector<std::vector<char>> &maze, int startX, int startY, int goalX, int goalY);
bool findPathDFS(std::vector<std::vector<char>> &maze, int startX, int startY, int goalX, int goalY);

#endif // MAZE_SOLVER_H
