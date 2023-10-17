/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for Inventory
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

TEST_CASE("Inventory Initialization", "[core]"){

	// create a temp agent and get its inventory
	cse491::AgentBase agent(1, "Test Agent");
	Inventory inventory = agent.GetInventory();

	SECTION("Get Agent"){

		// check that the inventorys agent is set correctly
		REQUIRE(inventory.GetAgent() == &agent);
	}
}

TEST_CASE("Add Item to Inventory", "[core]"){

	// create a temp agent and get its inventory
	cse491::AgentBase agent(1, "Test Agent");
	Inventory inventory = agent.GetInventory();

	// create two random items to test on
	std::shared_ptr<walle::Item> item1 = std::make_shared<Item>("sword", "weapon", 10);
	std::shared_ptr<walle::Item> item2 = std::make_shared<Item>("apple", "food", 2);

	SECTION("Add Item"){

		// check the default item count
		REQUIRE(inventory.GetItemCount() == 0);

		// checks that adding the items works
		REQUIRE(inventory.AddItem(item1) == true);
		REQUIRE(inventory.AddItem(item2) == true);

		// check the new item count
		REQUIRE(inventory.GetItemCount() == 2);
	}

//	SECTION("Has Item"){
//
//		// checks that the inventory now has those items
//		REQUIRE(inventory.HasItem(item1) == true);
//		REQUIRE(inventory.HasItem(item2) == true);
//	}

}

TEST_CASE("Drop Item from Inventory", "[core]"){

	// create a temp agent and get its inventory
	cse491::AgentBase agent(1, "Test Agent");
	walle::Inventory inventory = agent.GetInventory();

	// create two random items to test on
	std::shared_ptr<walle::Item> item1 = std::make_shared<Item>("axe", "weapon", 12);
	std::shared_ptr<walle::Item> item2 = std::make_shared<Item>("banana", "food", 1);

	inventory.AddItem(item1);
	inventory.AddItem(item2);

	SECTION("Drop Item"){

		// check the item count
		REQUIRE(inventory.GetItemCount() == 2);

		// checks that dropping the items works
		REQUIRE(inventory.DropItem(item1) == true);
		REQUIRE(inventory.DropItem(item2) == true);

		// check the new item count
		REQUIRE(inventory.GetItemCount() == 0);
	}

	inventory.DisplayInventory();

	SECTION("Does Not Have Item"){

		// checks that the inventory now does not have those inventory
		REQUIRE(inventory.HasItem(item1) == false);
		REQUIRE(inventory.HasItem(item2) == false);
	}

}