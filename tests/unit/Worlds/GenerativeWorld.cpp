/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for GenerativeWorld.hpp in source/Worlds
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "Worlds/GenerativeWorld.hpp"
#include "Worlds/BiomeGenerator.hpp"

using namespace std;
using namespace group6;
using namespace cse491;

TEST_CASE("Constructor", "[worlds][generative]") {
    static const unsigned int SEED = 5;
    BiomeType biome = BiomeType::Maze;

    GenerativeWorld world(biome, 50, 50, SEED);
    CHECK(world.GetSeed() == SEED);
}

TEST_CASE("FindTiles", "[worlds][generative]") {
    size_t wall_id = 1;
    size_t spike_id = 2;
    size_t door_id = 3;

    WorldGrid grid(10, 10);

    vector<GridPosition> walls {GridPosition(1, 1), GridPosition(2, 1), GridPosition(3, 1)};
    vector<GridPosition> spikes {GridPosition(5, 5), GridPosition(8, 5)};
    vector<GridPosition> doors {GridPosition(8, 8)};

    grid.At(walls[0]) = wall_id;
    grid.At(walls[1]) = wall_id;
    grid.At(walls[2]) = wall_id;

    grid.At(spikes[0]) = spike_id;
    grid.At(spikes[1]) = spike_id;

    grid.At(doors[0]) = door_id;

    CHECK(GenerativeWorld::FindTiles(grid, wall_id) == walls);
    CHECK(GenerativeWorld::FindTiles(grid, spike_id) == spikes);
    CHECK(GenerativeWorld::FindTiles(grid, door_id) == doors);
}
