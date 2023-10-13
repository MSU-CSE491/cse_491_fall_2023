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
