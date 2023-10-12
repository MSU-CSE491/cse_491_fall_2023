#pragma once

#include "DataReceiver.hpp"
#include "AgentData.hpp"

namespace DataCollection {

    /**
     * @brief Data receiver class specialized for storing AgentData objects.
     *
     * This class extends DataReceiver class and provides specific functionality
     * for storing AgentData objects along with grid positions and last action IDs.
     */
    class AgentReceiver : public DataReceiver<AgentData> {
    public:
        /**
         * @brief Stores a grid position and last action ID associated with an agent.
         *
         * @param pos The grid position to be stored.
         * @param last_action The last action ID associated with the agent.
         */
        void store_data(cse491::GridPosition pos, int last_action) {
            AgentData agent;
            agent.StorePositions(pos);
            agent.StoreAgentId(last_action);
            storage.push_back(agent);
        }

        /**
         * @brief Gets the position size of the last stored AgentData object.
         *
         * @return The position size of the last stored AgentData object.
         */
        int get_position_size() {
            if (!storage.empty()) {
                return storage.back().GetPositionSize();
            }
            return 0;
        }

        /**
         * @brief Checks if the storage is empty.
         *
         * @return True if the storage is empty, false otherwise.
         */
        bool is_empty() {
            return storage.empty();
        }
    };
} // namespace DataCollection