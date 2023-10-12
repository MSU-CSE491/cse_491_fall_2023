#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "DoubleLinkedList.hpp"
#include "../core/GridPosition.hpp"

namespace DataCollection
{
    /**
     * @brief Represents data collected for an agent, including their name, actions, positions, and associated IDs.
     */
    class AgentData {
    private:
        std::string name;  ///< The name of the agent.
        std::vector<int> action_ids;  ///< IDs associated with the agent's actions.
        DoubleLinkedList<cse491::GridPosition> position;  ///< Linked list of grid positions.
        std::vector<std::unordered_map<std::string, size_t>> actions;  ///< Vector of action maps.
        int position_size = 0;  ///< Current size of the position data.

    public:
        /**
         * @brief Default constructor for AgentData class.
         */
        AgentData() = default;

        /**
         * @brief Destructor for AgentData class.
         */
        ~AgentData() = default;

        /**
         * @brief Stores a grid position for the agent.
         * @param pos The grid position to be stored.
         */
        void StorePositions(cse491::GridPosition pos) {
            position.add_node(pos);
            position_size += 1;
        }

        /**
         * @brief Stores an action for the agent.
         * @param action The action to be stored.
         */
        void StoreAction(std::unordered_map<std::string, size_t> action) {
            actions.push_back(action);
        }

        /**
         * @brief Stores an agent ID associated with an action.
         * @param id The agent ID to be stored.
         */
        void StoreAgentId(int id) {
            action_ids.push_back(id);
        }

        /**
         * @brief Retrieves the stored actions.
         * @return Reference to the vector of action maps.
         */
        auto& GetActions() {
            return actions;
        }

        /**
         * @brief Sets the stored actions to a new set of actions.
         * @param newActions The new set of actions to be stored.
         */
        void SetActions(const std::vector<std::unordered_map<std::string, size_t>>& newActions) {
            actions = newActions;
        }

        /**
         * @brief Gets the size of the stored grid positions.
         * @return The number of stored grid positions.
         */
        int GetPositionSize() const {
            return position_size;
        }

        /**
         * @brief Gets the agent IDs associated with the stored actions.
         * @return The vector of agent IDs.
         */
        auto GetAgentIds() const {
            return action_ids;
        }
    };
} // namespace DataCollection