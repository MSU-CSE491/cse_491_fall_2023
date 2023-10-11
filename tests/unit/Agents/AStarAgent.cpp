/**
 * @file AStarAgent.cpp
 * @author Yousif Murrani
 */

//// Catch2
//#define CATCH_CONFIG_MAIN
//#include <catch2/catch_all.hpp>
//
//// class project
//#include "Agents/AStarAgent.h"
//#include "core/WorldBase.hpp"
//
//
//using namespace cse491::walle;
//
//TEST_CASE("AStar Agent Initialization", "[Agents]"){
//
//	// created following agent
//	AStarAgent agent(1, "AStar Agent");
//
//	SECTION("Initialization Failure"){
//
//		// check Initialize fails because agent has no actions
//		REQUIRE(agent.Initialize() == false);
//	}
//
//	// added actions to following agent
//	agent.AddAction("up", 1);
//	agent.AddAction("down", 2);
//	agent.AddAction("left", 3);
//	agent.AddAction("right", 4);
//
//	SECTION("Initialization Success"){
//
//		// check Initialize works because agent has actions
//		REQUIRE(agent.Initialize() == true);
//	}
//
//	SECTION("Agent is an Agent"){
//
//		// agent is always an agent
//		REQUIRE(agent.IsAgent() == true);
//	}
//}
//
//TEST_CASE("AStar Agent World", "[Agents]"){
//
//	AStarAgent agent(1, "AStar Agent");
//	cse491::WorldBase mock_world;
//
//	SECTION("Set World"){
//
//		agent.SetWorld(mock_world);
//		REQUIRE(agent.GetWorld() == mock_world);
//
//	}
//
//}
//
//TEST_CASE("AStarAgent Goal Position", "[Agents]") {
//
//	AStarAgent agent(1, "AStar Agent");
//	cse491::GridPosition goal_position(5, 5);
//
//	SECTION("Set Goal Position") {
//		agent.SetGoalPosition(goal_position);
//		REQUIRE(agent.GetGoalPosition() == goal_position);
//	}
//
//}



