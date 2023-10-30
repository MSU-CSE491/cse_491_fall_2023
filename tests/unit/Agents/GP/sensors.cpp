/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for agent.hpp in source/Agents/GP
 **/

// Catch2
#define CATCH_CONFIG_MAIN



#include <catch2/catch_all.hpp>

// Class project
#include "Agents/GP/GPAgent.hpp"
#include "Agents/GP/GPAgentSensors.hpp"


#include "Agents/PacingAgent.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Worlds/MazeWorld.hpp"


#include "core/GridPosition.hpp"

TEST_CASE("Agent Position", "[gp_agents]")
{

    cse491::MazeWorld world;
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);

    //GP agent
    auto & GPagent = static_cast<cowboys::GPAgent&>(world.AddAgent<cowboys::GPAgent>("GP 1").SetPosition(1, 0).SetProperty("char", 'G'));


    SECTION("Setting the first position")
    {
        GPagent.SetPosition(1, 1);
        CHECK(GPagent.GetPosition() == cse491::GridPosition(1, 1));
    }

    SECTION("Checking Left Right above below for (0,0)"){
        GPagent.SetPosition(0, 0);
        CHECK(GPagent.GetPosition() == cse491::GridPosition(0, 0));
        CHECK(cowboys::Sensors::wallDistance(world.GetGrid(), GPagent, cowboys::SensorDirection::ABOVE) == 0);
        CHECK(cowboys::Sensors::wallDistance(world.GetGrid(), GPagent, cowboys::SensorDirection::LEFT) == 0);
        CHECK(cowboys::Sensors::wallDistance(world.GetGrid(), GPagent, cowboys::SensorDirection::RIGHT) == 1);
        CHECK(cowboys::Sensors::wallDistance(world.GetGrid(), GPagent, cowboys::SensorDirection::BELOW) == 4);
    }


    SECTION("Checking Left Right above below for (0,0)"){
        GPagent.SetPosition(0, 0);
        CHECK(GPagent.GetPosition() == cse491::GridPosition(0, 0));
        CHECK(cowboys::Sensors::wallDistance(world.GetGrid(), GPagent, cowboys::SensorDirection::ABOVE) == 0);
        CHECK(cowboys::Sensors::wallDistance(world.GetGrid(), GPagent, cowboys::SensorDirection::LEFT) == 0);
        CHECK(cowboys::Sensors::wallDistance(world.GetGrid(), GPagent, cowboys::SensorDirection::RIGHT) == 1);
        CHECK(cowboys::Sensors::wallDistance(world.GetGrid(), GPagent, cowboys::SensorDirection::BELOW) == 4);
    }

    SECTION("Checking Left Right above below for (0,4)"){
        GPagent.SetPosition(0, 4);
        CHECK(GPagent.GetPosition() == cse491::GridPosition(0, 4));
        CHECK(cowboys::Sensors::wallDistance(world.GetGrid(), GPagent, cowboys::SensorDirection::ABOVE) == 4);
        CHECK(cowboys::Sensors::wallDistance(world.GetGrid(), GPagent, cowboys::SensorDirection::LEFT) == 0);
        CHECK(cowboys::Sensors::wallDistance(world.GetGrid(), GPagent, cowboys::SensorDirection::RIGHT) == 4);
        CHECK(cowboys::Sensors::wallDistance(world.GetGrid(), GPagent, cowboys::SensorDirection::BELOW) == 0);
    }

    SECTION("Checking Left Right above below for (0,0)"){
        GPagent.SetPosition(6, 0);
        CHECK(GPagent.GetPosition() == cse491::GridPosition(6, 0));
        CHECK(cowboys::Sensors::wallDistance(world.GetGrid(), GPagent, cowboys::SensorDirection::ABOVE) == 0);
        CHECK(cowboys::Sensors::wallDistance(world.GetGrid(), GPagent, cowboys::SensorDirection::LEFT) == 3);
        CHECK(cowboys::Sensors::wallDistance(world.GetGrid(), GPagent, cowboys::SensorDirection::RIGHT) == 8);
        CHECK(cowboys::Sensors::wallDistance(world.GetGrid(), GPagent, cowboys::SensorDirection::BELOW) == 4);
    }



}
