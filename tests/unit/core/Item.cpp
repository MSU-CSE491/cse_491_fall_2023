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
#include "core/Inventory.hpp"
#include "core/AgentBase.hpp"
#include "core/Item.hpp"

using namespace walle;

TEST_CASE("Item Initialization", "[core]"){

    // create temporary item to work on
    std::shared_ptr<Item> item = std::make_shared<Item>("test", 1, 1, 1, 1, 1.0);

    // check that it was initialized
    REQUIRE(item != nullptr);
}

TEST_CASE("Item Name", "[core]"){

    // create an item to work on
    std::shared_ptr<Item> item = std::make_shared<Item>("test1", 1, 1, 1, 1, 1.0);

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
    std::shared_ptr<Item> item = std::make_shared<Item>("test1", 1, 1, 1, 1, 1.0);

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
    std::shared_ptr<Item> item = std::make_shared<Item>("test1", 1, 1, 1, 1, 1.0);

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
    std::shared_ptr<Item> item = std::make_shared<Item>("test1", 1, 1, 1, 1, 1.0);

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

TEST_CASE("Item Value", "[core]"){

    // create an item to work on
    std::shared_ptr<Item> item = std::make_shared<Item>("test1", 1, 1, 1, 1, 1.0);

    SECTION("Get Value"){

        // grab item damage
        auto value = item->GetValue();

        // check item damage
        REQUIRE(value == 1);
    }

    SECTION("Set Value"){

        // set new damage and grab it
        item->SetValue(7);
        auto value = item->GetValue();

        // check new name
        REQUIRE(value == 7);
    }
}

TEST_CASE("Item Weight", "[core]"){

    // create an item to work on
    std::shared_ptr<Item> item = std::make_shared<Item>("test1", 1, 1, 1, 1, 1.0);

    SECTION("Get Weight"){

        // grab item damage
        auto weight = item->GetWeight();

        // check item damage
        REQUIRE(weight == 1);
    }

    SECTION("Set Weight"){

        // set new damage and grab it
        item->SetWeight(7.5);
        auto weight = item->GetWeight();

        // check new name
        REQUIRE(weight == 7.5);
    }
}

TEST_CASE("Item Inventory", "[core]"){

    // create a temp agent and get its inventory
    cse491::AgentBase agent(1, "Test Agent");
    walle::Inventory inventory = agent.GetInventory();

    // create an item to work on
    std::shared_ptr<Item> item = std::make_shared<Item>("test1", 1, 1, 1, 1, 1.0);

    SECTION("No Inventory"){

        // check that the item is not part of an inventory to start
        REQUIRE(item->GetInventory() == nullptr);
    }

    inventory.AddItem(item);

    SECTION("Get Inventory"){

        // check that the items inventory has been set
        REQUIRE(item->GetInventory() == &inventory);
    }
}