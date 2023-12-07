#pragma once

#include "AgentReciever.hpp"
#include "GameReceiver.hpp"
#include "DamageCollector.hpp"
#include "ItemUseCollector.hpp"
#include "AgentInteractionCollector.hpp"

namespace DataCollection {

    /**
     * @brief Represents a data control system to hold all related receivers and collectors.
     *
     * World should report back relevant data here and it will be stored appropriately.
     * Also manages graphing utilities.
     */
    class DataManager {
    private:
        AgentReceiver agentReceiver;                            /// Receiver for agent based data
        GameReceiver gameReceiver;                              /// Receiver for game based data
        DamageCollector damageCollector;                        /// Collector for item damage data
        ItemUseCollector itemUseCollector;                      /// Collector for item usage data
        AgentInteractionCollector agentInteractionCollector;    /// Collector for agent interaction data
        DataManager() {}

        DataManager(const DataManager&) = delete;
        DataManager& operator=(const DataManager&) = delete;
    public:
         /**
           * Destructor
           */
         ~DataManager() = default;

         static DataManager& GetInstance() {
             static DataManager instance;
             return instance;
         }

         /**
          * Get a handle to the agent receiver
          * @return Reference to the agent receiver
          */
         AgentReceiver& GetAgentReceiver() {
             return agentReceiver;
         }

        /**
        * Get a handle to the agent interaction collector
        * @return Reference to the agent interaction collector
        */
        AgentInteractionCollector& GetAgentInteractionCollector() {
            return agentInteractionCollector;
        }

         /**
          * Get a handle to the item use collector
          * @return Reference to the item use collector
          */
         ItemUseCollector& GetItemUseCollector() {
             return itemUseCollector;
         }

        /**
         * Get a handle to the game receiver
         * @return Const reference to the game receiver
         */
        const GameReceiver& GetGameReceiver() {
            return gameReceiver;
        }

        /**
          * Get a handle to the damage collector
          * @return Const reference to the damage collector
          */
        const DamageCollector& GetDamageCollector() {
            return damageCollector;
        }

        void WriteToJson() {
            std::filesystem::path currentPath = std::filesystem::current_path().parent_path().parent_path();
            currentPath = currentPath / "source" / "DataCollection" / "GRAPH";
            // Construct the full path to the data directory and the damage_data.json file
            std::filesystem::path DamagefilePath = currentPath  / "damage_data.json";
            std::filesystem::path ItemUsefilePath = currentPath  / "itemUsage.json";
            std::filesystem::path ItemdamagefilePath = currentPath  / "itemDamage.json";
            std::filesystem::path PositionfilePath = currentPath  / "gridPositions.json";
            std::filesystem::path InteractionfilePath = currentPath  / "agentInteractions.json";
            agentReceiver.WriteToPositionFile(PositionfilePath.string());
            itemUseCollector.WriteToItemUseFile(ItemUsefilePath.string());
            agentInteractionCollector.WriteToInteractionFile(InteractionfilePath.string());
        }
    };
}
