//
// Created by Yones Mussa on 10/11/23.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "DataCollection/AgentData.hpp"

TEST_CASE("AgentData class tests", ) {
    SECTION("Test storing positions and getting position size") {
        DataCollection::AgentData agent("Agent1");
        cse491::GridPosition pos1{1, 2};
        cse491::GridPosition pos2{3, 4};

        agent.StorePositions(pos1);
        agent.StorePositions(pos2);

        REQUIRE(agent.GetPositionSize() == 2);

        SECTION("Test storing actions and getting actions") {
            std::unordered_map<std::string, size_t> action1{{"up", 1}};
            std::unordered_map<std::string, size_t> action2{{"down", 2}};

            agent.StoreAction(action1);
            agent.StoreAction(action2);

            std::vector<std::unordered_map<std::string, size_t>> actions = agent.GetActions();
            REQUIRE(actions.size() == 2);

            REQUIRE(actions[0]["up"] == 1);
            REQUIRE(actions[1]["down"] == 2);
        }

        SECTION("Test storing agent IDs") {
            agent.StoreAgentId(101);
            agent.StoreAgentId(102);

            std::vector<int> ids = agent.GetAgentIds();
            REQUIRE(ids.size() == 2);
            REQUIRE(ids[0] == 101);
            REQUIRE(ids[1] == 102);
        }
    }
}