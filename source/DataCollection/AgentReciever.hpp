#pragma once

#include "DataReceiver.hpp"
#include "AgentData.hpp"

namespace DataCollection
{
    class AgentReciever : public DataReceiver<AgentData>{
        public:
        void store_data(cse491::GridPosition pos, int last_action){
            AgentData agent;
            agent.StorePositions(pos);
            agent.StoreAgentId(last_action);
            storage.push_back(agent);
        }

        int get_position_size() { return storage.back().GetPositionSize(); }
        bool is_empty() { return storage.empty(); }
    };
}
