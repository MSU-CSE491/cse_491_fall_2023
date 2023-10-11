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
    CHECK(list.getHead() == nullptr);
    CHECK(list.getTail() == nullptr);
    CHECK(list.getSize() == 0);
}

TEST_CASE("DoubleLinkedList Adding Values", "[DataCollection][DLL]") {
    DataCollection::DoubleLinkedList<int> list;
    list.add_node(1);
    list.add_node(2);
    list.add_node(3);
    CHECK(list.getHead()->value == 1);
    CHECK(list.getTail()->value == 3);
    CHECK(list.getSize() == 3);
}

TEST_CASE("DoubleLinkedList Removing Values", "[DataCollection][DLL]") {
    DataCollection::DoubleLinkedList<int> list;
    list.add_node(1);
    CHECK(list.getSize() == 1);
    CHECK(list.getHead()->value == 1);
    CHECK(list.getTail()->value == 1);

    auto value = list.remove_node();
    CHECK(list.getSize() == 0);
    CHECK(list.getHead() == nullptr);
    CHECK(list.getTail() == nullptr);
    CHECK(value->value == 1);

    // Check removing after empty is nullptr
    CHECK(list.remove_node() == nullptr);
}

TEST_CASE("DoubleLinkedList Traversing List", "[DataCollection][DLL]") {
    DataCollection::DoubleLinkedList<int> list;
    list.add_node(1);
    list.add_node(2);
    list.add_node(3);

    auto itr = list.getHead();
    while (itr != nullptr) {
        itr = itr->next;
    }
    CHECK(itr == nullptr);
}
