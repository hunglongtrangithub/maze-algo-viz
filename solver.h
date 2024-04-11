#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include <functional>
#include <vector>

struct Cell;

// Choose a default heuristic for A* search
inline std::function<int(int, int, int, int)> defaultHeuristic =
    [](int x1, int y1, int x2, int y2) -> int {
  return abs(x1 - x2) + abs(y1 - y2); // Manhattan distance
};
bool findPathAStar(
    std::vector<std::vector<char>> &maze, int x, int y,
    std::function<int(int, int, int, int)> heuristic = defaultHeuristic);
bool findPathDijkstra(std::vector<std::vector<char>> &maze, int x, int y);
bool findPathBFS(std::vector<std::vector<char>> &maze, int x, int y);
bool findPathDFS(std::vector<std::vector<char>> &maze, int startX, int startY);

#endif // MAZE_SOLVER_H
