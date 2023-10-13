/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for GameData.hpp in source/DataCollection
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "DataCollection/GameData.hpp"
TEST_CASE("GameDataTest", "[DataCollection][GameData]") {
    DataCollection::GameData data;
    // Testing numOfCol getter and setter
    SECTION("Test numOfCol getter and setter") {
        data.setNumOfCol(5);
        CHECK(data.getNumOfCol() == 5);
    }

        // Testing collision data storage and retrieval
    SECTION("Test StoreCollision and GetCollisions") {
        data.StoreCollision(10);
        data.StoreCollision(20);
        CHECK(data.GetCollisions().size() == 2);
        CHECK(data.GetCollisions()[0] == 10);
        CHECK(data.GetCollisions()[1] == 20);
    }

        // Testing shortest path action storage and retrieval
    SECTION("Test StoreShortestPathAction and GetShortestPathActions") {
        data.StoreShortestPathAction(3);
        data.StoreShortestPathAction(4);
        CHECK(data.GetShortestPathActions().size() == 2);
        CHECK(data.GetShortestPathActions()[0] == 3);
        CHECK(data.GetShortestPathActions()[1] == 4);
    }
    
}
