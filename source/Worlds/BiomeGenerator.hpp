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
#include "../core/WorldBase.hpp"
#include "../core/WorldGrid.hpp"
#include "../core/Data.hpp"

namespace group6 {
    using siv::PerlinNoise;
    using cse491::WorldGrid, cse491::WorldBase, cse491::GridPosition;

    enum class BiomeType {
        Maze,
        Grasslands,
        Ocean
    };

    /**
     * Generates a new grid based on a specified biome
     */
    class BiomeGenerator {
    private:
        const double frequency = 8.0;         ///< [0.1, 64.0]
        const int octaves = 8;                ///< [1, 16]

        PerlinNoise perlinNoise;              ///< The Perlin Noise procedural generation algorithm

        BiomeType biome;                      ///< Biome for the gird

        unsigned int width;                   ///< Width of the grid
        unsigned int height;                  ///< Height of the grid
        WorldGrid grid;                       ///< Grid of all tiles

        WorldBase *worldPtr = nullptr;        ///< Pointer to world

        unsigned int seed;                    ///< Seed used for RNG

        GridPosition keyLocation;

        std::vector<size_t> tiles;            ///< Vector to store tiles

        size_t floor_id = 0;
        size_t wall_id = 1;

        size_t spike_id = 2;
        size_t tar_id = 3;
        size_t key_id = 4;
        size_t door_id = 5;

        size_t grass_id = 6;
        size_t dirt_id = 7;

        size_t tree_id = 8;

        size_t sand_id = 11;
        size_t water_id = 10;

        size_t hole_id = 9;

    public:
        BiomeGenerator(BiomeType biome, unsigned int width, unsigned int height, unsigned int seed);
        ~BiomeGenerator() = default;

        void setWorld(WorldBase *world);

        void generate();
        void saveToFile(const std::string &filename) const;
        void placeSpecialTiles(const size_t &genericTile, const size_t &specialTile, double percentage);

        [[nodiscard]] unsigned int getSeed() const { return  seed; }

        void setTiles(const size_t &firstTile, const size_t &secondTile);
        [[nodiscard]] BiomeType getBiome() const { return biome; }

        void placeDoorTile(const size_t &doorTile);
        void placeTileRandom(const size_t& tile, const size_t& spawnTile);

        [[nodiscard]] std::vector<GridPosition> clearPath() const;
        void applyPathToGrid(const std::vector<GridPosition>& path);

        void placeTrees();

        void oceanHandler();
    };
}
