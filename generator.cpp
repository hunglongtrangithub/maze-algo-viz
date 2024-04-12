#include "definitions.h"
#include "utils.h"
#include <algorithm>  // For std::shuffle
#include <cstdlib>    // For rand() and srand()
#include <ctime>      // For time()
#include <functional> // For std::function
#include <random>     // For std::mt19937
#include <stack>
#include <vector>

// Directions: N, S, E, W
const int dx[4] = {0, 0, 1, -1};
const int dy[4] = {1, -1, 0, 0};

class UnionFind {
public:
  std::vector<int> parent;
  std::vector<int> rank;

  UnionFind(int size) {
    parent.resize(size);
    rank.resize(size, 0);
    for (int i = 0; i < size; i++) {
      parent[i] = i;
    }
  }

  int find(int x) {
    if (parent[x] != x) {
      parent[x] = find(parent[x]);
    }
    return parent[x];
  }

  void unite(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);
    if (rootX != rootY) {
      if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX;
      } else if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY;
      } else {
        parent[rootY] = rootX;
        rank[rootX]++;
      }
    }
  }
};
void randomizedKruskals(std::vector<std::vector<char> > &maze) {
  std::random_device rd;  // Obtain a random number from hardware
  std::mt19937 gen(rd()); // Seed the generator
  srand(time(nullptr));   // Seed random number generation

  int height = maze.size();
  int width = maze[0].size();
  UnionFind uf(width * height);

  std::vector<std::pair<int, int> > edges;
  // Initialize the maze and collect possible edges
  for (int y = 1; y < height - 1; y += 2) {
    for (int x = 1; x < width - 1; x += 2) {
      maze[y][x] = EMPTY;
      if (x < width - 2)
        edges.emplace_back(y * width + x, y * width + x + 2);
      if (y < height - 2)
        edges.emplace_back(y * width + x, (y + 2) * width + x);
    }
  }

  // Shuffle edges to ensure random order
  std::shuffle(edges.begin(), edges.end(), gen);

  // Process each edge
  for (auto &edge : edges) {
    int cell1 = edge.first;
    int cell2 = edge.second;
    if (uf.find(cell1) != uf.find(cell2)) {
      uf.unite(cell1, cell2);
      int inbetween = (cell1 + cell2) / 2;
      maze[inbetween / width][inbetween % width] = EMPTY;
      renderMaze(maze);
    }
  }
}

char chooseOrientation(int width, int height) {
  if (width < height)
    return 'h';
  else if (height < width)
    return 'v';
  else
    return (rand() % 2) == 0 ? 'h' : 'v';
}
void divide(std::vector<std::vector<char> > &maze, int yStart, int yEnd,
            int xStart, int xEnd, char orientation) {
  if (yEnd <= yStart || xEnd <= xStart) {
    return;
  }

  bool horizontal = orientation == 'h';

  int yMid =
      horizontal ? yStart + (rand() % ((yEnd - yStart) / 2)) * 2 + 1 : yStart;
  int xMid =
      !horizontal ? xStart + (rand() % ((xEnd - xStart) / 2)) * 2 + 1 : xStart;

  // Add a wall
  for (int i = yStart; i <= yEnd; ++i) {
    for (int j = xStart; j <= xEnd; ++j) {
      if (horizontal ? i == yMid : j == xMid) {
        maze[i][j] = WALL;
      }
    }
  }
  // Add a hole in the wall
  int hole = horizontal ? xStart + (rand() % ((xEnd - xStart) / 2)) * 2 + 1
                        : yStart + (rand() % ((yEnd - yStart) / 2)) * 2 + 1;
  maze[horizontal ? yMid : hole][horizontal ? hole : xMid] = EMPTY;
  renderMaze(maze);

  // Recursively divide the two parts
  if (horizontal) {
    divide(maze, yStart, yMid - 1, xStart, xEnd,
           chooseOrientation(xEnd - xStart, yMid - yStart - 1));
    divide(maze, yMid + 1, yEnd, xStart, xEnd,
           chooseOrientation(xEnd - xStart, yEnd - yMid - 1));
  } else {
    divide(maze, yStart, yEnd, xStart, xMid - 1,
           chooseOrientation(xMid - xStart - 1, yEnd - yStart));
    divide(maze, yStart, yEnd, xMid + 1, xEnd,
           chooseOrientation(xEnd - xMid - 1, yEnd - yStart));
  }
}
void recursiveDivision(std::vector<std::vector<char> > &maze) {
  srand(time(nullptr)); // Seed random generator

  int height = maze.size();
  int width = maze[0].size();

  // Initialize the maze with walls
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      maze[y][x] = (y == 0 || x == 0 || y == height - 1 || x == width - 1)
                       ? WALL
                       : EMPTY;
    }
  }

  // Start the division
  divide(maze, 1, height - 2, 1, width - 2,
         chooseOrientation(width - 2, height - 2));
}

std::vector<std::pair<int, int> >
getUnvisitedNeighbors(int x, int y, std::vector<std::vector<char> > &maze) {
  std::vector<std::pair<int, int> > neighbors;
  for (int i = 0; i < 4; ++i) {
    int nx = x + dx[i] * 2;
    int ny = y + dy[i] * 2;
    if (nx > 0 && nx < maze[0].size() && ny > 0 && ny < maze.size() &&
        maze[ny][nx] == WALL) {
      neighbors.push_back({nx, ny});
    }
  }
  return neighbors;
}

void randomizedPrims(std::vector<std::vector<char> > &maze) {
  srand(time(nullptr)); // Seed random number generation

  // Initialize the maze with walls
  for (int y = 0; y < maze.size(); ++y) {
    for (int x = 0; x < maze[0].size(); ++x) {
      maze[y][x] = WALL;
    }
  }

  // Initialize the starting point
  int startX = 1 + rand() % (maze[0].size() / 2) * 2;
  int startY = 1 + rand() % (maze.size() / 2) * 2;
  maze[startX][startY] = EMPTY;

  // Initialize the list with the starting point
  std::vector<std::pair<int, int> > list;
  list.push_back(std::make_pair(startX, startY));

  while (!list.empty()) {
    // Randomly select a cell from the list
    int index = rand() % list.size();
    int x = list[index].first;
    int y = list[index].second;
    auto neighbors = getUnvisitedNeighbors(x, y, maze);

    // If the cell has no unvisited neighbors, remove it from the list
    // and continue to the next cell
    while (neighbors.empty()) {
      list.erase(list.begin() + index);
      if (list.empty()) {
        return;
      }
      index = rand() % list.size();
      x = list[index].first;
      y = list[index].second;
      neighbors = getUnvisitedNeighbors(x, y, maze);
    }

    // Randomly select a neighbor
    int nextIndex = rand() % neighbors.size();
    int nx = neighbors[nextIndex].first;
    int ny = neighbors[nextIndex].second;

    // Remove the wall between the current cell and the selected neighbor
    maze[ny][nx] = EMPTY;
    maze[(y + ny) / 2][(x + nx) / 2] = EMPTY;
    renderMaze(maze);

    list.push_back(std::make_pair(nx, ny));
  }
}

void randomizedDFS(std::vector<std::vector<char> > &maze) {
  srand(time(nullptr)); // Seed random number generation

  // Initialize the maze with walls
  for (int y = 0; y < maze.size(); ++y) {
    for (int x = 0; x < maze[0].size(); ++x) {
      maze[y][x] = WALL;
    }
  }
  // Initialize the starting point
  int startX = 1 + rand() % (maze[0].size() / 2) * 2;
  int startY = 1 + rand() % (maze.size() / 2) * 2;

  // Initialize the stack with the starting point
  std::stack<std::pair<int, int> > stack;
  stack.push(std::make_pair(startX, startY));

  while (!stack.empty()) {
    int x = stack.top().first;
    int y = stack.top().second;
    stack.pop();

    std::vector<std::pair<int, int> > neighbors =
        getUnvisitedNeighbors(x, y, maze);
    if (!neighbors.empty()) {
      stack.push(std::make_pair(x, y));

      // Randomly select a neighbor
      int nextIndex = rand() % neighbors.size();
      int nx = neighbors[nextIndex].first;
      int ny = neighbors[nextIndex].second;

      // Remove the wall between the current cell and the selected neighbor
      maze[ny][nx] = EMPTY;
      maze[y + (ny - y) / 2][x + (nx - x) / 2] = EMPTY;
      renderMaze(maze);

      stack.push(std::make_pair(nx, ny));
    }
  }
}

std::vector<std::vector<char> >
generateMaze(int width = 21, int height = 21,
             std::function<void(std::vector<std::vector<char> > &)> carveMaze =
                 randomizedDFS) {
  srand(time(nullptr)); // Seed for random number generation

  // Ensure odd dimensions
  width |= 1;
  height |= 1;

  // Initialize maze with walls
  std::vector<std::vector<char> > maze(height, std::vector<char>(width, WALL));

  carveMaze(maze);

  return maze;
}

// #include <iostream>
// int main() {
//   std::vector<std::vector<char> > maze =
//       generateMaze(48, 41, randomizedKruskals);
//   std::cout << "Maze generated!" << std::endl;
//   std::cout << maze.size() << "x" << maze[0].size() << std::endl;

//   return 0;
// }
