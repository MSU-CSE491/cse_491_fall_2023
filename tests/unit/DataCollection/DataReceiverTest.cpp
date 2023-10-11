/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for DoubleLinkedList.hpp in source/DataCollection
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "DataCollection/DataReceiverTest.hpp"


TEST_CASE("DataReceiverTest Storage", "[DataReceiverTest]") {
    DataCollection::DataReceiverTest<int> dataReceiver;
    SECTION("Store data and check if vector is not empty") {
        dataReceiver.store_data(42);
        REQUIRE(dataReceiver.getStorage().size() == 1);
        REQUIRE_FALSE(dataReceiver.getStorage().empty());
        REQUIRE(dataReceiver.getStorage()[0] == 42);
        dataReceiver.getStorage().clear();
        REQUIRE_FALSE(dataReceiver.getStorage().size() == 1);
    }
}

TEST_CASE("Adding data to actions vector", "[actions]") {
    DataCollection::DataReceiverTest<std::unordered_map<std::string, size_t>> actionsData;

    // Adding data to the actions vector
    std::unordered_map<std::string, size_t> data1 = {{"key1", 1}, {"key2", 2}};
    actionsData.getActions().push_back(data1);

    std::unordered_map<std::string, size_t> data2 = {{"key3", 3}, {"key4", 4}};
    actionsData.getActions().push_back(data2);

    SECTION("Check size of actions vector") {
        REQUIRE(actionsData.getActions().size() == 2);
    }

    SECTION("Verify data in actions vector") {
        REQUIRE(actionsData.getActions()[0]["key1"] == 1);
        REQUIRE(actionsData.getActions()[0]["key2"] == 2);
        REQUIRE(actionsData.getActions()[1]["key3"] == 3);
        REQUIRE(actionsData.getActions()[1]["key4"] == 4);
    }
}

    // Add more test cases as needed to validate the behavior of store_actions function







