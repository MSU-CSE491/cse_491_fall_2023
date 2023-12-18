/**
 * @file ItemUseCollector.cpp
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "DataCollection/ItemUseCollector.hpp"

TEST_CASE("ItemUseCollectorCreate", "[ItemUseCollector]")
{
    DataCollection::ItemUseCollector itemUse;

    CHECK(itemUse.GetUsageData().empty());      // Item usage is empty
    CHECK(itemUse.GetMostFrequent().empty());   // No item is most frequent
    CHECK(itemUse.GetNumberOfItems() == 0);
}

TEST_CASE("ItemUseCollectorInsert", "[ItemUseCollector]")
{
    DataCollection::ItemUseCollector itemUse;
    auto& usageData = itemUse.GetUsageData();

    itemUse.IncrementItemUsage("Sword");
    CHECK(itemUse.GetMostFrequent() == "Sword");
    CHECK(itemUse.GetLeastFrequent() == "Sword");
    CHECK(itemUse.GetNumberOfItems() == 1);
    CHECK(usageData.at("Sword") == 1);
}

TEST_CASE("ItemUseCollectorMultipleInsert", "[ItemUseCollector]")
{
    DataCollection::ItemUseCollector itemUse;
    auto& usageData = itemUse.GetUsageData();

    itemUse.IncrementItemUsage("Sword");
    itemUse.IncrementItemUsage("Boat");
    itemUse.IncrementItemUsage("Stick");
    itemUse.IncrementItemUsage("Sword");
    itemUse.IncrementItemUsage("Sword");
    itemUse.IncrementItemUsage("Stick");
    itemUse.IncrementItemUsage("Sword");
    itemUse.IncrementItemUsage("Boat");
    itemUse.IncrementItemUsage("Stick");
    itemUse.IncrementItemUsage("Shield");

    CHECK(itemUse.GetNumberOfItems() == 4);
    CHECK(itemUse.GetMostFrequent() == "Sword");
    CHECK(itemUse.GetLeastFrequent() == "Shield");
    CHECK(usageData.at("Sword") == 4);
    CHECK(usageData.at("Stick") == 3);
    CHECK(usageData.at("Boat") == 2);
    CHECK(usageData.at("Shield") == 1);
}