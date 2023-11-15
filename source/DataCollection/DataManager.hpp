#pragma once

#include "AgentReciever.hpp"
#include "GameReceiver.hpp"
#include "DamageCollector.hpp"

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
    public:
        /**
         * Constructor
         */
         DataManager() = default;

         /**
          * Get a handle to the agent receiver
          * @return Const reference to the agent receiver
          */
         const AgentReceiver& GetAgentReceiver() {
             return agentReceiver;
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
