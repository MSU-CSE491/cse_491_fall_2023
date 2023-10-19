#pragma once

#include <string>
#include <list>
#include <utility>
#include <vector>
#include <unordered_map>
#include "../core/GridPosition.hpp"

namespace DataCollection
{
    /**
     * @brief Represents data collected for an agent, including their name, actions, positions, and associated IDs.
     */
    class AgentData {
    private:
        std::string name;  ///< The name of the agent.
        std::vector<int> actionIds;  ///< IDs associated with the agent's actions.
        std::vector<cse491::GridPosition> position;  ///list of grid positions.
        std::vector<std::unordered_map<std::string,size_t>> actions;  ///< Vector of action maps.

    public:
        /**
         * @brief Default constructor for AgentData class.
         */
//        AgentData() = default;
        AgentData(std::string name) : name(std::move(name)) {}

        /**
         * @brief Destructor for AgentData class.
         */
        ~AgentData() = default;

        /**
         * @brief Stores a grid position for the agent.
         * @param pos The grid position to be stored.
         */
        void StorePositions(cse491::GridPosition pos) {
            position.push_back(pos);
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
            actionIds.push_back(id);
        }

        /**
         * @brief Retrieves the stored actions.
         * @return Reference to the vector of action maps.
         */
        auto& GetActions() {
            return actions;
        }

        int GetActionSize() const {
            return actions.size();
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
            return position.size();
        }

        /**
         * @brief Gets the agent IDs associated with the stored actions.
         * @return The vector of agent IDs.
         */
        auto GetAgentIds() const {
            return actionIds;
        }

        std::string GetName() const {
            return name;
        }
    };
} // namespace DataCollection