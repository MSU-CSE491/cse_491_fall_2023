#pragma once

#include <unordered_map>
#include <string>
#include "JsonBuilder.hpp"

namespace DataCollection {

    /**
     * @brief A data collector class to quantify agent interactions.
     *
     * Useful for setting up graphs for common interactions.
     */
    class AgentInteractionCollector {
    private:
        std::unordered_map<std::string, int> interactionData;  /// Data storage map of agent name to interactions.
    public:
        /**
         * Default constructor for AgentInteractionCollector
         */
        AgentInteractionCollector() = default;

        /**
         * Getter for interaction data
         * @return Const reference to the interaction data storage.
         */
        const std::unordered_map<std::string, int>& GetInteractionData() { return interactionData; }

        /**
         * Get the amount of unique agents that occured
         * @return int amount of agent occurances
         */
        size_t GetUniqueInteractions() { return interactionData.size(); }

        /**
         * Increment occurance amount for a certain agent.
         * @param agentName Agent name to record new interaction with
         */
        void RecordInteraction(const std::string& agentName) { interactionData[agentName]++; }

        void WriteToInteractionFile(const std::string filename){
            JsonBuilder json_builder;
            std::ofstream jsonfilestream(filename);
            json_builder.StartArray("agentInteractions");
            for (auto& [agentName, interactionCount] : interactionData) {
                json_builder.AddName(agentName);
                json_builder.AddInt("interactionCount", interactionCount);
                json_builder.InputToArray("agentInteractions", json_builder.GetJSON());
                json_builder.ClearJSON();
            }
            json_builder.WriteToFile(jsonfilestream, json_builder.GetJSONArray());
            jsonfilestream.close();
        }

    };
}