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


TEST_CASE("Inventory Initialization", "[core]"){

	cse491::AgentBase agent(1, "Test Agent");
	walle::Inventory inventory = agent.GetInventory();

	SECTION("Get Agent"){

		// check that the inventorys agent is set correctly
		REQUIRE(inventory.GetAgent() == &agent);
	}

}