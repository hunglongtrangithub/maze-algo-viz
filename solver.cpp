#include "definitions.h"
#include "utils.h"
#include <functional>
#include <queue>
#include <stack>
#include <limits>

// Helper structure to keep track of positions and their predecessors
struct Cell {
  int x, y;
  Cell *prev;
  int cost;
  Cell(int x, int y, Cell *prev, int cost = 0)
      : x(x), y(y), prev(prev), cost(cost) {}
  // Overload the > operator
  bool operator>(const Cell &other) const { return cost > other.cost; }
};

struct AStarCell {
  int x, y;
  AStarCell *prev;
  int cost;
  int f; // total cost
  AStarCell(int x, int y, AStarCell *prev, int cost, int heuristic)
      : x(x), y(y), prev(prev), cost(cost), f(cost + heuristic) {}
  // Overload the > operator
  bool operator>(const AStarCell &other) const { return f > other.f; }
};

// Choose a default heuristic for A* search
int defaultHeuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);  // Example heuristic (Manhattan distance)
}
bool findPathAStar(std::vector<std::vector<char> > &maze, int startX, int startY, int goalX, int goalY,
                   int (*heuristic)(int, int, int, int)) {
  int rows = maze.size();
  int cols = maze[0].size();

  // set up priority queue with custom comparator
  std::priority_queue<AStarCell *, std::vector<AStarCell *>, std::greater<AStarCell *> > queue;
  // set up costs matrix
  std::vector<std::vector<int> > costs(
      rows, std::vector<int>(cols, std::numeric_limits<int>::max()));
  // set up visited matrix
  std::vector<std::vector<bool> > visited(rows, std::vector<bool>(cols, false));

  // Enqueue start position with cost 0
  queue.push(new AStarCell(startX, startY, nullptr, 0, heuristic(startX, startY, goalX, goalY)));
  costs[startY][startX] = 0;

  int dx[4] = {1, -1, 0, 0};
  int dy[4] = {0, 0, 1, -1};

  while (!queue.empty()) {
    AStarCell *current = queue.top();
    queue.pop();
    int x = current->x;
    int y = current->y;

    if (visited[y][x])
      continue;
    visited[y][x] = true;

    if (x == goalX && y == goalY) {
      for (AStarCell *cell = current; cell != nullptr; cell = cell->prev) {
        if (maze[cell->y][cell->x] != GOAL && maze[cell->y][cell->x] != START)
          maze[cell->y][cell->x] = PATH;
        renderMaze(maze);
      }
      renderMaze(maze); // Final visualization

      // Destruct the priority_queue
      while (!queue.empty()) {
        delete queue.top();
        queue.pop();
      }

      return true;
    }

    if (maze[y][x] != START)
      maze[y][x] = VISITED;
    renderMaze(maze); // Visualize each step

    for (int i = 0; i < 4; ++i) {
      int nx = x + dx[i];
      int ny = y + dy[i];

      int newCost = current->cost + 1;
      int newHeuristic = heuristic(nx, ny, goalX, goalY);

      if (nx >= 0 && nx < cols && ny >= 0 && ny < rows &&
          maze[ny][nx] != WALL && costs[ny][nx] > newCost) {
        costs[ny][nx] = newCost;
        queue.push(
            new AStarCell(nx, ny, current, newCost, newHeuristic));
      }
    }
  }

  // Destruct the priority_queue
  while (!queue.empty()) {
    delete queue.top();
    queue.pop();
  }
  return false; // No path found
}

bool findPathDijkstra(std::vector<std::vector<char> > &maze, int startX, int startY, int goalX, int goalY) {
  int rows = maze.size();
  int cols = maze[0].size();

  // Set up priority queue with custom comparator
  std::priority_queue<Cell *, std::vector<Cell *>, std::greater<Cell *> > queue;
  // Set up costs matrix
  std::vector<std::vector<int> > costs(
      rows, std::vector<int>(cols, std::numeric_limits<int>::max()));
  // Set up visited matrix
  std::vector<std::vector<bool> > visited(rows, std::vector<bool>(cols, false));

  // Enqueue start position with cost 0
  queue.push(new Cell(startX, startY, nullptr, 0));
  costs[startY][startX] = 0;

  int dx[4] = {1, -1, 0, 0};
  int dy[4] = {0, 0, 1, -1};

  while (!queue.empty()) {
    Cell *current = queue.top();
    queue.pop();
    int x = current->x;
    int y = current->y;

    if (visited[y][x])
      continue;
    visited[y][x] = true;

    if (x == goalX && y == goalY) {
      for (Cell *cell = current; cell != nullptr; cell = cell->prev) {
        if (maze[cell->y][cell->x] != GOAL && maze[cell->y][cell->x] != START)
          maze[cell->y][cell->x] = PATH;
        renderMaze(maze);
      }
      renderMaze(maze); // Final visualization

      // Destruct the priority_queue
      while (!queue.empty()) {
        delete queue.top();
        queue.pop();
      }

      return true;
    }

    if (maze[y][x] != START)
      maze[y][x] = VISITED;
    renderMaze(maze); // Visualize each step

    for (int i = 0; i < 4; ++i) {
      int nx = x + dx[i];
      int ny = y + dy[i];

      int newCost = current->cost + 1;

      if (nx >= 0 && nx < cols && ny >= 0 && ny < rows &&
          maze[ny][nx] != WALL && costs[ny][nx] > newCost) {
        costs[ny][nx] = newCost;
        queue.push(new Cell(nx, ny, current, costs[ny][nx]));
      }
    }
  }
  
  // Destruct the priority_queue
  while (!queue.empty()) {
    delete queue.top();
    queue.pop();
  }
  return false; // No path found
}

bool findPathBFS(std::vector<std::vector<char> > &maze, int startX, int startY, int goalX, int goalY) {
  int rows = maze.size();
  int cols = maze[0].size();

  std::queue<Cell *> queue;
  queue.push(new Cell(startX, startY, nullptr)); // Enqueue start position

  std::vector<std::vector<bool> > visited(rows, std::vector<bool>(cols, false));
  visited[startY][startX] = true;

  int dx[4] = {1, -1, 0, 0};
  int dy[4] = {0, 0, 1, -1};

  while (!queue.empty()) {
    Cell *current = queue.front();
    queue.pop();
    
    int x = current->x;
    int y = current->y;
    // Check for goal
    if (x == goalX && y == goalY) {
      // Backtrack to mark the path
      for (Cell *cell = current; cell != nullptr; cell = cell->prev) {
        if (maze[cell->y][cell->x] != GOAL && maze[cell->y][cell->x] != START)
          maze[cell->y][cell->x] = PATH;
        renderMaze(maze);
      }
      renderMaze(maze); // Final visualization
 
      // Destruct the priority_queue
      while (!queue.empty()) {
        delete queue.front();
        queue.pop();
      }

      return true;
    }

    // Mark as visited
    if (maze[y][x] != START)
      maze[y][x] = VISITED;
    renderMaze(maze); // Visualize each step

    // Explore neighbors
    for (int i = 0; i < 4; ++i) {
      int nx = x + dx[i];
      int ny = y + dy[i];

      if (nx >= 0 && nx < cols && ny >= 0 && ny < rows &&
          maze[ny][nx] != WALL && !visited[ny][nx]) {
        visited[ny][nx] = true;
        queue.push(new Cell(nx, ny, current));
      }
    }
  }
  
  // Destruct the priority_queue
  while (!queue.empty()) {
    delete queue.front();
    queue.pop();
  }

  return false; // No path found
}

bool findPathDFS(std::vector<std::vector<char> > &maze, int startX, int startY, int goalX, int goalY) {
  std::stack<Cell *> stack;
  stack.push(new Cell(startX, startY, nullptr, 0));

  static const int dx[] = {1, -1, 0, 0};
  static const int dy[] = {0, 0, 1, -1};

  while (!stack.empty()) {
    Cell *current = stack.top();
    stack.pop();

    int x = current->x;
    int y = current->y;

    // Check bounds and walls
    if (x < 0 || x >= maze[0].size() || y < 0 || y >= maze.size() ||
        maze[y][x] == WALL || maze[y][x] == VISITED) {
      continue;
    }

    // Check for goal
    if (x == goalX && y == goalY) {
      // Backtrack to mark the path
      for (Cell *cell = current; cell != nullptr; cell = cell->prev) {
        if (maze[cell->y][cell->x] != GOAL && maze[cell->y][cell->x] != START)
          maze[cell->y][cell->x] = PATH;
        renderMaze(maze);
      }
      renderMaze(maze);

    // destruct the stack
    while (!stack.empty()) {
        delete stack.top();
        stack.pop();
      }

      return true;
    }

    // Mark the current cell as visited
    if (maze[y][x] != START)
      maze[y][x] = VISITED;
    renderMaze(maze);

    // Explore neighbors
    for (int direction = 0; direction < 4; ++direction) {
      stack.push(new Cell(x + dx[direction], y + dy[direction], current));
    }
  }

  // destruct the stack
  while (!stack.empty()) {
    delete stack.top();
    stack.pop();
  }

  return false;
}
