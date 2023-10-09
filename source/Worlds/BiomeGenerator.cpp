/**
 * @file BiomeGenerator.cpp
 * @author Paul Schulte
 */

#include <fstream>
#include "BiomeGenerator.h"

using std::vector;

/**
 * Creates a generator with a grid of the given size and using the given seed
 * @param biome  The biome of the grid
 * @param width  The width of the grid
 * @param height The height of the grid
 * @param seed   The seed used for random number generation
 */
BiomeGenerator::BiomeGenerator(const std::string &biome, unsigned int width, unsigned int height, unsigned int seed) : biome(biome), width(width), height(height) {
    perlinNoise = PerlinNoise(seed);
    grid = vector<vector<char>>(height, vector<char>(width));
}

/**
 * Generates the grid with two types of tiles
 * @param tile1  The first tile generated
 * @param tile2 The second tile generated
 */
void BiomeGenerator::generate(const char &tile1, const char &tile2) {
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            const double val = perlinNoise.noise2D(x * frequency / width, y * frequency / height);
            grid.at(y).at(x) = val < 0 ? tile1 : tile2;
        }
    }

    if (getBiome() == "maze")
    {
        placeSpecialTiles(tile1, 'X', 0.02); // Placing spike tiles
        placeSpecialTiles(tile1, 'O', 0.05); // Placing tar tiles
    }
}

/**
 * Generates special tiles on the grid
 * @param genericTile  The tile that the special tile can spawn on
 * @param specialTile The special tile to generate
 */
void BiomeGenerator::placeSpecialTiles(const char &genericTile, const char &specialTile, double percentage) {
    std::vector<std::pair<int, int>> floorPositions;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (grid[i][j] == genericTile) {
                floorPositions.push_back({j, i});
            }
        }
    }

    int numSpikes = floorPositions.size() * percentage;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(floorPositions.begin(), floorPositions.end(), g);

    // Convert some generic floor tiles to special tiles
    for (int i = 0; i < numSpikes; ++i) {
        grid[floorPositions[i].second][floorPositions[i].first] = specialTile;
    }
}

/**
 * Saves the grid to the given filepath
 * @param filename The filename the grid will be saved to
 */
void BiomeGenerator::saveToFile(const std::string &filename) const {
    std::ofstream out(filename);
    for (const auto &row : grid) {
        for (const auto &cell : row) {
            out << cell;
        }
        out << "\n";
    }
    out.close();
}