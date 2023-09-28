/**
 * @file MazeGeneration.h
 * @author Milan Mihailovic, ChatGPT
 *
 *
 */

#pragma once

#include <vector>
#include <string>

class MazeGeneration {
public:
    MazeGeneration(int width, int height);
    void generate();
    void saveToFile(const std::string &filename);

    void placeSpikeTiles(double percentage);
    void placeTarTiles(double percentage);

private:
    int width;
    int height;
    std::vector<std::vector<char>> grid;
//    void dfs(int x, int y);
    bool isValid(int x, int y);
};