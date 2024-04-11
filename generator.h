#ifndef MAZE_GENERATORS_H
#define MAZE_GENERATORS_H

#include <vector>
#include <functional>

// Union-Find class for Kruskal's algorithm
class UnionFind {
public:
    UnionFind(int size);
    int find(int x);
    void unite(int x, int y);

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

// Function prototypes for maze generation algorithms
void randomizedKruskals(std::vector<std::vector<char> >& maze);
void recursiveDivision(std::vector<std::vector<char> >& maze);
void randomizedPrims(std::vector<std::vector<char> >& maze);
void randomizedDFS(std::vector<std::vector<char> >& maze);


// Generic maze generation function
std::vector<std::vector<char> > generateMaze(int width = 29, int height = 21,
             std::function<void(std::vector<std::vector<char> > &)> carveMaze = randomizedDFS);

#endif // MAZE_GENERATORS_H
