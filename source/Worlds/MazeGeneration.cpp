/**
 * @file MazeGeneration.cpp
 * @author Milan Mihailovic, ChatGPT
 *
 *
 */

#include "Mazegeneration.h"
#include <fstream>
#include <random>
#include <stack>
#include <algorithm>

MazeGeneration::MazeGeneration(int width, int height)
        : width(width), height(height), grid(height, std::vector<char>(width, '#')) {
    for (int i = 0; i < width; ++i) {
        grid[0][i] = ' ';
        grid[height-1][i] = ' ';
    }
}

void MazeGeneration::generate() {
    std::stack<std::pair<int, int>> stack;
    int x = 2 * (rand() % (width / 2 - 1)) + 1;
    int y = 2 * (rand() % (height / 2 - 1)) + 1;
    stack.push({x, y});

    while (!stack.empty()) {
        x = stack.top().first;
        y = stack.top().second;
        grid[y][x] = ' ';

        std::vector<std::pair<int, int>> directions = {
                {0, -2}, {-2, 0}, {0, 2}, {2, 0}
        };

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(directions.begin(), directions.end(), g);

        bool moved = false;
        for (const auto &dir : directions) {
            int nx = x + dir.first;
            int ny = y + dir.second;

            if (isValid(nx, ny) && grid[ny][nx] == '#') {
                stack.push({nx, ny});
                grid[ny - dir.second/2][nx - dir.first/2] = ' ';
                moved = true;
                break;
            }
        }

        if (!moved) stack.pop();
    }

    placeSpikeTiles(0.05);
    placeTarTiles(.05);
}

bool MazeGeneration::isValid(int x, int y) {
    return x > 0 && x < width-1 && y > 0 && y < height-1;
}

void MazeGeneration::saveToFile(const std::string &filename) {
    std::ofstream out(filename);
    for (const auto &row : grid) {
        for (const auto &cell : row) {
            out << cell;
        }
        out << "\n";
    }
    out.close();
}

void MazeGeneration::placeSpikeTiles(double percentage) {
    std::vector<std::pair<int, int>> floorPositions;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (grid[i][j] == ' ') {  
                floorPositions.push_back({j, i});
            }
        }
    }

    int numSpikes = floorPositions.size() * percentage;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(floorPositions.begin(), floorPositions.end(), g);

    // Convert some floor tiles to spike tiles
    for (int i = 0; i < numSpikes; ++i) {
        grid[floorPositions[i].second][floorPositions[i].first] = 'X';
    }
}

void MazeGeneration::placeTarTiles(double percentage) {
    std::vector<std::pair<int, int>> floorPositions;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (grid[i][j] == ' ') {
                floorPositions.push_back({j, i});
            }
        }
    }

    int numTars = floorPositions.size() * percentage;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(floorPositions.begin(), floorPositions.end(), g);

    // Convert some floor tiles to tar tiles
    for (int i = 0; i < numTars; ++i) {
        grid[floorPositions[i].second][floorPositions[i].first] = 'O';
    }
}