/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for DoubleLinkedList.hpp in source/DataCollection
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "DataCollection/DataReceiver.hpp"


TEST_CASE("DataReceiver Storage", "[DataReceiver]") {
    DataCollection::DataReceiver<int> dataReceiver;
    SECTION("Store data and check if vector is not empty") {
        dataReceiver.store_data(42);
        REQUIRE(dataReceiver.getStorage().size() == 1);
        REQUIRE_FALSE(dataReceiver.getStorage().empty());
        REQUIRE(dataReceiver.getStorage()[0] == 42);
        dataReceiver.getStorage().clear();
        REQUIRE_FALSE(dataReceiver.getStorage().size() == 1);
    }
}







