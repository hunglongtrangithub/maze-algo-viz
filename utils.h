#ifndef MAZE_UTILS_H
#define MAZE_UTILS_H

#include <string>
#include <vector>

// Function to load a maze from a file
std::vector<std::vector<char>> loadMaze(const std::string &filename);
// Function to render a maze
void renderMaze(const std::vector<std::vector<char>> &maze, int delay = 5,
                bool pad = true);
// Function to clear the screen
void clearScreen();
#endif // MAZE_UTILS_H
