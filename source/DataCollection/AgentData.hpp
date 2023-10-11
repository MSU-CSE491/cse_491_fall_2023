#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "DoubleLinkedList.hpp"
#include "../core/GridPosition.hpp"

namespace DataCollection
{
    class AgentData
    {
    private:
        std::string name;
        std::vector<int> action_ids;
        DoubleLinkedList<cse491::GridPosition> position;
        std::vector<std::unordered_map<std::string, size_t>> actions;
        int position_size = 0;
    public:
        AgentData() = default;
//        AgentData(std::string name) : name(name) {}
        virtual ~AgentData() = default;
        void store_positions(cse491::GridPosition pos){
            position.add_node(pos);
            position_size += 1;
        }
        inline void store_action(std::unordered_map<std::string, size_t> obj2){
            actions.push_back(obj2);
        }
        void store_agent_id(int id) {
            action_ids.push_back(id);
        }
        std::vector<std::unordered_map<std::string, size_t>> get_action(){
            return actions;
        }
        int get_position_size() { return position_size; }
    };
} // namespace DataCollection