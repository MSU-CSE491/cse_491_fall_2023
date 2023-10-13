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
        void StoreData(cse491::GridPosition pos, int last_action) {
            AgentData agent;
            agent.StorePositions(pos);
            agent.StoreAgentId(last_action);
            StoreIntoStorage(agent);
        }
    };
} // namespace DataCollection