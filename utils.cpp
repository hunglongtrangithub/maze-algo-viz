#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <string>
#include <vector>

std::vector<std::vector<char> > loadMaze(const std::string &filename) {
  std::vector<std::vector<char> > maze;
  std::ifstream file(filename);
  std::string line;

  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return maze; // Returns an empty maze if file opening fails
  }

  while (std::getline(file, line)) {
    std::vector<char> row;
    for (char cell : line) {
      row.push_back(cell);
    }
    maze.push_back(row);
  }

  file.close();
  return maze;
}

void clearScreen() {
#ifdef _WIN32
  system("CLS");
#else
  system("clear");
#endif
}

void renderMaze(const std::vector<std::vector<char> > &maze, int delay,
                bool pad) {
  clearScreen();
  for (const auto &row : maze) {
    for (char cell : row) {
      if (pad) {
        std::cout << cell << ' ';
      } else {
        std::cout << cell;
      }
    }
    std::cout << '\n';
  }
  std::this_thread::sleep_for(
      std::chrono::milliseconds(delay)); // Slow down for visualization
}

// int main() {
//   std::vector<std::vector<char> > maze = loadMaze("maze.txt");
//   std::cout << maze.size() << " " << maze[0].size() << std::endl;
//   for (const auto &row : maze) {
//     std::cout << row.size() << " ";
//     // for (char cell : row) {
//     //   std::cout << cell << ' ';
//     // }
//     std::cout << '\n';
//   }

//   return 0;
// }
