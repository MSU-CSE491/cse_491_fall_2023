/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for Data.hpp in source/core
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "Worlds/BiomeGenerator.hpp"
#include "Worlds/GenerativeWorld.hpp"

using namespace std;
using namespace group6;
using namespace cse491;

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

    GenerativeWorld world(biome, 50, 50, SEED);

    BiomeGenerator biomeGenerator(biome, 50, 50, SEED);
    biomeGenerator.setWorld(&world);
    biomeGenerator.generate();
    biomeGenerator.saveToFile(GRID_BASE + "1.grid");

    GenerativeWorld world2(biome, 50, 50, SEED);

    BiomeGenerator biomeGenerator2(biome, 50, 50, SEED);
    biomeGenerator2.setWorld(&world2);
    biomeGenerator2.generate();
    biomeGenerator2.saveToFile(GRID_BASE + "2.grid");

    CHECK(fileToString(GRID_BASE + "1.grid") == fileToString(GRID_BASE + "2.grid"));

    GenerativeWorld world3(biome, 50, 50, SEED + 1);

    BiomeGenerator biomeGenerator3(biome, 50, 50, SEED + 1);
    biomeGenerator3.setWorld(&world3);
    biomeGenerator3.generate();
    biomeGenerator3.saveToFile(GRID_BASE + "3.grid");

    CHECK_FALSE(fileToString(GRID_BASE + "1.grid") == fileToString(GRID_BASE + "3.grid"));
}
