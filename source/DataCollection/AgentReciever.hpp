#pragma once

#include "DataReceiver.hpp"
#include "AgentData.hpp"

namespace DataCollection
{
    class AgentReciever : public DataReceiver<AgentData>{
        public:
        void store_data(cse491::GridPosition pos, int last_action){
            AgentData agent;
            agent.store_positions(pos);
            agent.store_agent_id(last_action);
            storage.push_back(agent);
        }

        int get_position_size() { return storage.back().get_position_size(); }
        bool is_empty() { return storage.empty(); }
    };
}
