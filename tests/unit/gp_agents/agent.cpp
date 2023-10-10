/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for agent.hpp in source/Group7_GP_Agent
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "Group7_GP_Agent/agent.hpp"

#include "Group7_GP_Agent/GPAgentsRegisters.hpp"



TEST_CASE("Agent construction", "[gp_agents]")
{
    SECTION("Inheritance from AgentBase")
    {
        cowboys::GPAgent agent{1, "name"};
        CHECK(agent.GetID() == 1);
        CHECK(agent.GetName() == "name");
    }
}
