#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "DoubleLinkedList.hpp"
#include "../core/GridPosition.hpp"

namespace DataCollection
{
    class AgentData {
    private:
        std::string name;
        std::vector<int> action_ids;
        DoubleLinkedList<cse491::GridPosition> position; // Assuming GridPosition is a valid type
        std::vector<std::unordered_map<std::string, size_t>> actions;
        int position_size = 0;

    public:
        AgentData() = default;
        ~AgentData() = default;

        void StorePositions(cse491::GridPosition pos) {
            position.add_node(pos);
            position_size += 1;
        }

        void StoreAction(std::unordered_map<std::string, size_t> action) {
            actions.push_back(action);
        }

        void StoreAgentId(int id) {
            action_ids.push_back(id);
        }

        auto& GetActions() {
            return actions;
        }
        // Setter for actions
        void SetActions(auto& newActions) {
            actions = newActions;
        }

        int GetPositionSize() const {
            return position_size;
        }

        auto GetAgentIds() const {
            return action_ids;
        }
    };
} // namespace DataCollection