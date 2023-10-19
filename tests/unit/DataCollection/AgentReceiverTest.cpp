/**
 * @file AgentRecieverTest.cpp
 * @author ahmad
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "DataCollection/AgentReciever.hpp"

TEST_CASE("AgentRecieverConstruction", "[AgentRecieverTest]")
{
    DataCollection::AgentReceiver agent_reciever;
    CHECK(agent_reciever.IsEmpty());

}

TEST_CASE("AgentRecieverStoreData", "[AgentRecieverTest]")
{
    DataCollection::AgentReceiver agent_reciever;
    cse491::GridPosition pos{1, 2};
    agent_reciever.StoreData("Agent1",pos, 1);
    CHECK_FALSE(agent_reciever.IsEmpty());
    agent_reciever.StoreData("Agent1",pos, 2);
    agent_reciever.StoreData("Agent2",pos, 3);
    CHECK(agent_reciever.GetAgent("Agent1")->GetPositionSize() == 2);

}
