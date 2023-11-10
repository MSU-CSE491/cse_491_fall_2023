/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for GenerativeWorld.hpp in source/Worlds
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "Worlds/GenerativeWorld.hpp"

TEST_CASE("Constructor", "[worlds][generative]") {
    static const unsigned int SEED = 5;
    cse491::GenerativeWorld world(SEED);
    CHECK(world.GetSeed() == SEED);
}
