#pragma once

#include "DataReceiver.hpp"
#include "AgentData.hpp"

namespace DataCollection
{
    class AgentReciever : public DataReceiver<AgentData>{
        public:
        void DebugPrint() override {
            // auto pos = storage.back();
            // std::cout << "Adding: (" << pos.GetX() << ", " << pos.GetY() << ")" << std::endl;
        }
        void store_positions(cse491::GridPosition pos){
            AgentData agent;
            agent.store_positions(pos);
            storage.push_back(agent);
        }
        int get_position_size() { return storage.back().get_position_size(); }
        bool is_empty() { return storage.empty(); }
    };
}
