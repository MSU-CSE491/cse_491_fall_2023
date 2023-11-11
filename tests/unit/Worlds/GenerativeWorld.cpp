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
