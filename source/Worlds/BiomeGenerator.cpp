/**
 * @file BiomeGenerator.cpp
 * @author Paul Schulte
 */

#include <fstream>
#include "BiomeGenerator.h"

using std::vector;

/**
 * Creates a generator with a grid of the given size and using the given seed
 * @param width  The width of the grid
 * @param height The height of the grid
 * @param seed   The seed used for random number generation
 */
BiomeGenerator::BiomeGenerator(unsigned int width, unsigned int height, unsigned int seed) : width(width), height(height) {
    perlinNoise = PerlinNoise(seed);
    grid = vector<vector<char>>(height, vector<char>(width));
}

/**
 * Generates the grid with grass and dirt
 */
void BiomeGenerator::generate() {
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            const double val = perlinNoise.noise2D(x * frequency / width, y * frequency / height);
            grid.at(y).at(x) = val < 0 ? 'M' : '~';
        }
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