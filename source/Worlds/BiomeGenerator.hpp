/**
 * @file BiomeGenerator.hpp
 * @author Paul Schulte, Milan Mihailovic, ChatGPT
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

enum class BiomeType {
    Maze,
    Grasslands
};

/**
 * Holds coordinate position on the grid
 * @param biome The biome of the grid
 */
struct Point {
    int x;    ///< The x coordinate on the grid)
    int y;    ///< The y coordinate on the grid)

    /**
     * Creates an (x,y) coordinate point
     * @param _x  The x-coordinate
     * @param _y  The y-coordinate
     */
    Point(int _x, int _y): x(_x), y(_y) {}

    /**
     * Comparison operator between 2 equal points
     * @param other The other point that this point is being compared to
     * @return True if the 2 points are equal, false otherwise
     */
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }

    /**
     * Comparison operator between 2 different points
     * @param other The other point that this point is being compared to
     * @return True if the 2 points are different, false otherwise
     */
    bool operator!=(const Point& other) const { return !(*this == other); }
};

/**
 * Generates a new grid based on a specified biome
 */
class BiomeGenerator {
private:
    const double frequency = 8.0;         ///< [0.1, 64.0]
    // const int octaves = 8;                ///< [1, 16]

    PerlinNoise perlinNoise;              ///< The Perlin Noise procedural generation algorithm

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

    void setTiles(const char &firstTile, const char &secondTile);
    [[nodiscard]] BiomeType getBiome() const { return biome; }

    void placeDoorTile(const char &doorTile);
    void placeKeyTile(const char &keyTile);

    [[nodiscard]] std::vector<Point> clearPath() const;
    void applyPathToGrid(const std::vector<Point>& path);
};
