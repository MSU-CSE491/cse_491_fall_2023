/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for source/Group7_GP_Agent/CGPAgent.hpp
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Group7_GP_Agent/CGPAgent.hpp"

using namespace cowboys;

TEST_CASE("CGPAgent construction", "[group7][agent]") {
  SECTION("CGPAgent construction") {
    CGPAgent agent(0, "agent");
    CHECK(agent.GetGenotype().GetNumConnections() == 0);
    CHECK(agent.GetGenotype().GetNumFunctionalNodes() == 0);
  }
  SECTION("CGPAgent construction with genotype") {
    CGPGenotype genotype({8, 4, 2, 10, 2});

    CGPAgent agent(0, "agent", genotype);
    CHECK(agent.GetGenotype().GetNumConnections() == 8 * 10 + (8 + 10) * 10 + (10 + 10) * 4);
    CHECK(agent.GetGenotype().GetNumFunctionalNodes() == 2 * 10 + 4);
  }
}
TEST_CASE("Copying", "[group7][agent][genotype]") {
  CGPGenotype genotype({8, 4, 2, 10, 2});
  CGPAgent agent(0, "agent", genotype);
  CGPAgent agent2(1, "agent2");
  CHECK(agent.GetGenotype() != agent2.GetGenotype());

  GPAgent_ &agent_ref = agent;
  GPAgent_ &agent2_ref = agent2;
  agent2_ref.Copy(agent_ref);
  CHECK(agent.GetGenotype() == agent2.GetGenotype());
}
