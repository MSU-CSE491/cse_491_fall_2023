/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for Item
 *
 * @author Yousif Murrani
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// class project
#include "core/Item.hpp"

using namespace walle;

TEST_CASE("Item Initialization", "[core]"){

    // create temporary item to work on
    std::shared_ptr<Item> item = std::make_shared<Item>("test", 1, 1, 1);

    // check that it was initialized
    REQUIRE(item != nullptr);
}

TEST_CASE("Item Name", "[core]"){

    // create an item to work on
    std::shared_ptr<Item> item = std::make_shared<Item>("test1", 1, 1, 1);

    SECTION("Get Name"){

        // grab item name
        auto name = item->GetName();

        // check item name
        REQUIRE(name == "test1");
    }

    SECTION("Set Name"){

        // set new name and grab it
        item->SetName("test2");
        auto name = item->GetName();

        // check new name
        REQUIRE(name == "test2");
    }
}

TEST_CASE("Item Level", "[core]"){

    // create an item to work on
    std::shared_ptr<Item> item = std::make_shared<Item>("test1", 1, 1, 1);

    SECTION("Get Level"){

        // grab item level
        auto level = item->GetLevel();

        // check item level
        REQUIRE(level == 1);
    }

    SECTION("Set Level"){

        // set new level and grab it
        item->SetLevel(8);
        auto level = item->GetLevel();

        // check new level
        REQUIRE(level == 8);
    }
}

TEST_CASE("Item Damage", "[core]"){

    // create an item to work on
    std::shared_ptr<Item> item = std::make_shared<Item>("test1", 1, 1, 1);

    SECTION("Get Damage"){

        // grab item damage
        auto damage = item->GetDamage();

        // check item damage
        REQUIRE(damage == 1);
    }

    SECTION("Set Damage"){

        // set new damage and grab it
        item->SetDamage(7);
        auto damage = item->GetDamage();

        // check new name
        REQUIRE(damage == 7);
    }
}

TEST_CASE("Item Durability", "[core]"){

    // create an item to work on
    std::shared_ptr<Item> item = std::make_shared<Item>("test1", 1, 1, 1);

    SECTION("Get Durability"){

        // grab item durability
        auto dura = item->GetDurability();

        // check item durability
        REQUIRE(dura == 1);
    }

    SECTION("Set Durability"){

        // set new durability and grab it
        item->SetDurability(5);
        auto dura = item->GetDurability();

        // check new name
        REQUIRE(dura == 5);
    }

    SECTION("Use Durability"){

        // set a new durability
        item->SetDurability(10);

        // use the item a few times
        item->UseItem();
        item->UseItem();
        item->UseItem();
        item->UseItem();

        // grab the durability
        auto dura = item->GetDurability();

        // make sure it has been affected by uses
        REQUIRE(dura == 6);
    }
}