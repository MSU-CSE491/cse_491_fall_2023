/**
 * @file BiomeGenerator.h
 * @author Paul Schulte
 *
 * Uses perlin noise to create dirt and grass on a grid
 */

#pragma once

#include "PerlinNoise.hpp"
#include "../core/WorldGrid.hpp"

using siv::PerlinNoise;
using cse491::WorldGrid;

class BiomeGenerator {
private:
    const double frequency = 8.0; ///< [0.1, 64.0]
    const int octaves = 8;        ///< [1, 16]

    PerlinNoise perlinNoise;

    unsigned int width;                  ///< Width of the grid
    unsigned int height;                 ///< Height of the grid
    std::vector<std::vector<char>> grid; ///< Grid of all tiles

public:
    BiomeGenerator(unsigned int width, unsigned int height, unsigned int seed);
    ~BiomeGenerator() = default;

    void generate();
    void saveToFile(const std::string &filename) const;
};
