/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for DoubleLinkedList.hpp in source/DataCollection
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "DataCollection/DoubleLinkedList.hpp"

TEST_CASE("DoubleLinkedList Construction", "[DataCollection][DLL]") {
    DataCollection::DoubleLinkedList<int> list;
    CHECK(list.getSize() == 0);
}


