/**
 * @file AgentRecieverTest.cpp
 * @author ahmad
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "DataCollection/AgentReciever.hpp"

TEST_CASE("AgentRecieverConstruction", "[AgentRecieverTest]")
{
    DataCollection::AgentReciever agent_reciever;
    CHECK(agent_reciever.is_empty());

}

TEST_CASE("AgentRecieverTest", "[AgentRecieverTest]")
{
    DataCollection::AgentReciever agent_reciever;
    cse491::GridPosition pos1(1, 1);
    cse491::GridPosition pos2(2, 2);
    cse491::GridPosition pos3(3, 3);
    cse491::GridPosition pos4(4, 4);

    agent_reciever.store_data(pos1, 0);
    CHECK(agent_reciever.get_position_size() == 1);
    agent_reciever.store_data(pos2, 0);
    CHECK(agent_reciever.get_position_size() == 2);
    agent_reciever.store_data(pos3, 0);
    CHECK(agent_reciever.get_position_size() == 3);
    agent_reciever.store_data(pos4, 0);
    CHECK(agent_reciever.get_position_size() == 4);
}