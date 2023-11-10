/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for Data.hpp in source/core
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "Worlds/BiomeGenerator.hpp"

using namespace std;
using namespace group6;

TEST_CASE("Constructor", "[worlds][biome]") {
    static const unsigned int SEED = 5;

    auto biome = BiomeType::Maze;
    BiomeGenerator biomeGenerator(biome, 50, 50, SEED);

    CHECK(biomeGenerator.getBiome() == biome);
    CHECK(biomeGenerator.getSeed() == SEED);
}

string fileToString(const string& fileName) {
    ifstream file(fileName);

    string line;
    string result;
    while (getline(file, line)) {
        result.append(line);
    }

    return result;
}

TEST_CASE("Generation", "[worlds][biome]") {
    static const unsigned int SEED = 5;
    static const string GRID_BASE = "../assets/grids/generated_maze";

    auto biome = BiomeType::Maze;
    BiomeGenerator biomeGenerator(biome, 50, 50, SEED);
    biomeGenerator.generate();
    biomeGenerator.saveToFile(GRID_BASE + "1.grid");

    BiomeGenerator biomeGenerator2(biome, 50, 50, SEED);
    biomeGenerator2.generate();
    biomeGenerator2.saveToFile(GRID_BASE + "2.grid");

    CHECK(fileToString(GRID_BASE + "1.grid") == fileToString(GRID_BASE + "2.grid"));
}
