#include "definitions.h"
#include "generator.h"
#include "solver.h"
#include "utils.h"
#include <iostream>
#include <vector>

#define MAX_DIMENSION 50

void takeDimensions(int &width, int &height) {
  while (true) {
    std::cout << "Width: ";
    std::cin >> width;
    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid input. Please enter a number." << std::endl;
      continue;
    }

    std::cout << "Height: ";
    std::cin >> height;
    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid input. Please enter a number." << std::endl;
      continue;
    }

    // Validate the dimensions
    if (width < 5 || height < 5) {
      std::cout << "Dimensions too small. Please try again." << std::endl;
      continue;
    }
    if (width > MAX_DIMENSION || height > MAX_DIMENSION) {
      std::cout << "Dimensions too large. Please try again." << std::endl;
      continue;
    }
    if (width % 2 == 0 || height % 2 == 0) {
      std::cout << "All dimensions must be odd. Please try again." << std::endl;
      continue;
    }

    // If we reach this point, the input is valid
    break;
  }
}

int main() {
  // Load the maze
  std::vector<std::vector<char>> maze = loadMaze("cpp_logo.txt");

  // Clear screen and print the initial maze
  clearScreen();
  renderMaze(maze, 0, false);

  // Initial welcome message
  std::cout << "Welcome to the Maze Generator and Solver!" << std::endl;

  // Prompt the user to enter maze dimensions
  std::cout << "Enter the width and height of the maze." << std::endl;
  std::cout << "Width and height must be odd numbers between 5 and 50."
            << std::endl;

  int width, height;
  takeDimensions(width, height);

  // Prompt the user to choose a maze generator
  std::cout << "1. Randomized Depth-First Search" << std::endl;
  std::cout << "2. Randomized Prim's Algorithm" << std::endl;
  std::cout << "3. Recursive Division" << std::endl;
  std::cout << "4. Randomized Kruskal's Algorithm" << std::endl;
  std::cout << "Choose an algorithm to generate the maze:" << std::endl;
  int choice;
  std::cin >> choice;

  // Attempt to generate a maze using the selected algorithm
  switch (choice) {
  case 1:
    maze = generateMaze(width, height, randomizedDFS);
    break;
  case 2:
    maze = generateMaze(width, height, randomizedPrims);
    break;
  case 3:
    maze = generateMaze(width, height, recursiveDivision);
    break;
  case 4:
    maze = generateMaze(width, height, randomizedKruskals);
    break;
  default:
    std::cout << "Invalid choice. Using the default algorithm." << std::endl;
    maze = generateMaze(width, height, randomizedDFS);
    break;
  }

  // Prompt the user to choose a maze solver
  std::cout << "1. Depth-First Search" << std::endl;
  std::cout << "2. Breadth-First Search" << std::endl;
  std::cout << "3. Dijkstra Search" << std::endl;
  std::cout << "4. A* Search" << std::endl;
  std::cout << "Choose an algorithm to solver the maze:" << std::endl;
  std::cin >> choice;

  // Set the start and goal positions
  int startX = 1;
  int startY = 1;
  int goalX = maze[0].size() - 2;
  int goalY = maze.size() - 2;

  maze[startY][startX] = START;
  maze[goalY][goalX] = GOAL;

  // Attempt to find a path using the selected algorithm
  bool pathFound = false;
  switch (choice) {
  case 1:
    pathFound = findPathDFS(maze, startX, startY, goalX, goalY);
    break;
  case 2:
    pathFound = findPathBFS(maze, startX, startY, goalX, goalY);
    break;
  case 3:
    pathFound = findPathDijkstra(maze, startX, startY, goalX, goalY);
    break;
  case 4:
    pathFound = findPathAStar(maze, startX, startY, goalX, goalY);
    break;
  default:
    std::cout << "Invalid choice." << std::endl;
    break;
  }
  if (!pathFound) {
    std::cout << "No path found." << std::endl;
  } else {
    std::cout << "Path found!" << std::endl;
  }

  return 0;
}
