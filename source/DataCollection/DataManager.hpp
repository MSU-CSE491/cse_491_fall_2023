#pragma once

#include "AgentReciever.hpp"
#include "GameReceiver.hpp"
#include "DamageCollector.hpp"
#include "ItemUseCollector.hpp"

namespace DataCollection {

    /**
     * @brief Represents a data control system to hold all related receivers and collectors.
     *
     * World should report back relevant data here and it will be stored appropriately.
     * Also manages graphing utilities.
     */
    class DataManager {
    private:
        AgentReceiver agentReceiver;        // Receiver for agent based data
        GameReceiver gameReceiver;          // Receiver for game based data
        DamageCollector damageCollector;    // Collector for item damage data
        ItemUseCollector itemUseCollector;  // Collector for item usage data
    public:
        /**
         * Constructor
         */
         DataManager() = default;

         /**
          * Destructor
          */
          ~DataManager() {
              std::cout << "test" << std::endl;
          }

         /**
          * Get a handle to the agent receiver
          * @return Reference to the agent receiver
          */
         AgentReceiver& GetAgentReceiver() {
             return agentReceiver;
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
    };
}
