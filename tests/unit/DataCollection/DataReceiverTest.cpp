/**
 * This file is part of the Fall 2023, CSE 491 course project.
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "DataCollection/DataReceiver.hpp"

TEMPLATE_TEST_CASE("DataReceiver class tests", "[DataReceiver][template]", int, double, std::string) {
    DataCollection::DataReceiver<int> dataReceiver;
    SECTION("Test storing and retrieving data") {

        dataReceiver.StoreIntoStorage(42);
        dataReceiver.StoreIntoStorage(321);
        dataReceiver.StoreIntoStorage(123);

        std::vector<int> storedData = dataReceiver.getStorage();
        REQUIRE(storedData.size() == 3);
        REQUIRE(storedData[0] == 42);
        REQUIRE(storedData[1] == 321);
        REQUIRE(storedData[2] == 123);
    }

    SECTION("Test empty storage") {
        std::vector<int> storedData = dataReceiver.getStorage();
        REQUIRE(storedData.empty());
    }
}








