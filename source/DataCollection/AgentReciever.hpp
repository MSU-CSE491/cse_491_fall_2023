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
    private:
        std::unordered_map<std::string, std::shared_ptr<AgentData>> agent_map;

    public:
        /**
         * @brief Stores a grid position and last action ID associated with an agent.
         *
         * @param pos The grid position to be stored.
         * @param last_action The last action ID associated with the agent.
         */
        void StoreData(std::string name, cse491::GridPosition pos, [[maybe_unused]] int last_action) {
            std::shared_ptr<AgentData> agent = GetAgent(name);
            agent->StorePositions(pos);
//            agent->StoreAction(last_action);
            StoreIntoStorage(*agent);
        }

        void AddAgent(const std::string& name) {
            AgentData agent(name);
            agent_map[name] = std::make_shared<AgentData>(agent);
        }

        std::shared_ptr<AgentData> GetAgent(const std::string& name)
        {
            if (agent_map.find(name)==agent_map.end()) {
                AddAgent(name);
                return agent_map[name];
            }
            else {
                return agent_map[name];
            }
        }

        AgentData GetAgentData(const std::string& name) {
            return *agent_map[name];
        }
    };
} // namespace DataCollection