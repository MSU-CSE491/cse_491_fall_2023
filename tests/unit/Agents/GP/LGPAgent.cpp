/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for source/[Deprecated]Group7_GP_Agent/CGPAgent.hpp
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Agents/GP/LGPAgent.hpp"

using namespace cowboys;

TEST_CASE("LGPAgent construction", "[group7][agent]") {
  SECTION("LGPAgent construction") {
    LGPAgent agent(0, "agent");
    agent.Initialize();
    CHECK(agent.GetInstructionsList().size() == LISTSIZE);
  }
}
TEST_CASE("Copying", "[group7][agent]") {
  SECTION("Different lists (presumably)") {
    LGPAgent agent(0, "agent");
    agent.Initialize();
    LGPAgent agent2(1, "agent2");
    agent2.Initialize();
    CHECK(agent.GetInstructionsList() != agent2.GetInstructionsList());
    
    // Generic references to LGPAgent
    GPAgentBase &agent_ref = agent;
    GPAgentBase &agent2_ref = agent2;
    agent2_ref.Copy(agent_ref); // Copy agent into agent2
    CHECK(agent.GetInstructionsList() == agent2.GetInstructionsList());
  }
  SECTION("Mutation") {
    LGPAgent agent(0, "agent");
    agent.Initialize();
    LGPAgent agent2(1, "agent2");
    agent2.Initialize();
    agent2.Copy(agent);
    CHECK(agent.GetInstructionsList() == agent2.GetInstructionsList());
    agent.MutateAgent(1);
    CHECK(agent.GetInstructionsList() != agent2.GetInstructionsList());
  }
}
