/**
 * @file BiomeGenerator.h
 * @author Paul Schulte
 *
 * Uses perlin noise to create dirt and grass on a grid
 */

#pragma once
#include <queue>
#include <functional>
#include <vector>


#include "PerlinNoise.hpp"
#include "../core/WorldGrid.hpp"

using siv::PerlinNoise;
using cse491::WorldGrid;

struct Point {
    int x, y;
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    bool operator<(const Point& other) const { return std::tie(x, y) < std::tie(other.x, other.y); }
};


enum class BiomeType {
    Maze,
    Grasslands
};

class BiomeGenerator {
private:
    const double frequency = 8.0; ///< [0.1, 64.0]
    const int octaves = 8;        ///< [1, 16]

    PerlinNoise perlinNoise;

    BiomeType biome;                      ///< Biome for the gird
    std::vector<char> tiles;              ///< Vector to store tiles

    unsigned int width;                   ///< Width of the grid
    unsigned int height;                  ///< Height of the grid
    std::vector<std::vector<char>> grid;  ///< Grid of all tiles

public:
    BiomeGenerator(BiomeType biome, unsigned int width, unsigned int height, unsigned int seed);
    ~BiomeGenerator() = default;

    void generate();
    void saveToFile(const std::string &filename) const;
    void placeSpecialTiles(const char& genericTile, const char& specialTile, double percentage);
//    bool isPathExists();

    void setTiles(const char &firstTile, const char &secondTile);
};
