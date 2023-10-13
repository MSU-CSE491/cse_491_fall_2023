/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for GameData.hpp in source/DataCollection
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "DataCollection/DoubleLinkedList.hpp"
#include "DataCollection/GameData.hpp"
#include "DataCollection/DoubleLinkedList.hpp"
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

        // Testing shortest path position storage and retrieval
    SECTION("Test StoreShortestPathPos and GetShortestPathPos") {
        DataCollection::DoubleLinkedList<cse491::GridPosition> pos1;
        pos1.add_node(cse491::GridPosition(1, 1));
        data.StoreShortestPathPos(pos1);

        DataCollection::DoubleLinkedList<cse491::GridPosition> pos2;
        pos2.add_node(cse491::GridPosition(2, 2));
        data.StoreShortestPathPos(pos2);

        CHECK(data.GetShortestPathPos().size() == 2);
        // Here you might also want to check the actual positions inside the stored LinkedLists
    }
}
