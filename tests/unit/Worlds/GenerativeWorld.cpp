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
#include "Agents/PacingAgent.hpp"

using namespace std;
using namespace group6;
using namespace cse491;

TEST_CASE("Constructor", "[worlds][generative]") {
    static const unsigned int SEED = 5;
    BiomeType biome = BiomeType::Maze;

    GenerativeWorld world(biome, 50, 50, SEED);
    CHECK(world.GetSeed() == SEED);
}

TEST_CASE("ItemHelper", "[worlds][generative]") {
    static const unsigned int SEED = 5;
    BiomeType biome = BiomeType::Maze;

    GenerativeWorld world(biome, 50, 50, SEED);
    AgentBase &agent = world.AddAgent<PacingAgent>("Agent");
    world.AddItem("Boots", "symbol", 'B').SetPosition(1, 1).SetName("Boots").SetProperty("Health", 3.0);
    size_t itemId = world.GetItemID("Boots");
    GridPosition pos(0, 0);

    world.ItemHelper(agent, pos);
    CHECK_FALSE(agent.HasItem(itemId));

    pos = GridPosition(1, 1);
    world.ItemHelper(agent, pos);
    CHECK(agent.HasItem(itemId));
}

TEST_CASE("SpikeTileHelper", "[worlds][generative]") {
    static const unsigned int SEED = 5;
    BiomeType biome = BiomeType::Maze;

    GenerativeWorld world(biome, 50, 50, SEED);
    AgentBase &agent = world.AddAgent<PacingAgent>("Agent");
    world.AddItem("Shield", "symbol", 'B').SetPosition(1, 1).SetName("Shield").SetProperty("Health", 3.0);

    GridPosition pos(1, 1);
    world.ItemHelper(agent, pos);

    world.SpikeTileHelper(agent);
    ItemBase &item = world.GetItem(world.GetItemID("Shield"));
    CHECK(item.GetProperty("Health") == 2.0);
}

TEST_CASE("TarTileHelper", "[worlds][generative]") {
    static const unsigned int SEED = 5;
    BiomeType biome = BiomeType::Maze;

    GenerativeWorld world(biome, 50, 50, SEED);
    AgentBase &agent = world.AddAgent<PacingAgent>("Agent");
    world.AddItem("Boots", "symbol", 'B').SetPosition(1, 1).SetName("Boots").SetProperty("Health", 3.0);

    world.TarTileHelper(agent);
    CHECK(agent.HasProperty("tar_property"));

    agent.RemoveProperty("tar_property");
    GridPosition pos(1, 1);
    world.ItemHelper(agent, pos);

    world.TarTileHelper(agent);
    CHECK_FALSE(agent.HasProperty("tar_property"));
}

TEST_CASE("KeyTileHelper", "[worlds][generative]") {
    static const unsigned int SEED = 5;
    BiomeType biome = BiomeType::Maze;

    GenerativeWorld world(biome, 50, 50, SEED);
    AgentBase &agent = world.AddAgent<PacingAgent>("Agent");
    GridPosition pos(1, 1);

    world.KeyTileHelper(agent, pos);
    CHECK(agent.GetProperty("key_property") == 0.0);

    AgentBase &agent2 = world.AddAgent<InterfaceBase>("Interface");

    world.KeyTileHelper(agent2, pos);
    CHECK(agent2.GetProperty("key_property") == 1.0);
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

TEST_CASE("TeleporterHelper", "[worlds][generative]") {
    static const unsigned int SEED = 5;
    BiomeType biome = BiomeType::Maze;

    GenerativeWorld world(biome, 100, 100, SEED);
    world.AddTeleporters();

    GridPosition pos1(53, 6);
    GridPosition pos2(18, 18);
    GridPosition newPos = pos1;

    world.TeleporterHelper(newPos);
    CHECK(newPos == pos2);

    newPos = pos2;
    world.TeleporterHelper(newPos);
    CHECK(newPos == pos1);
}
