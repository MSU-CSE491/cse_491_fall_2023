#pragma once

#include <memory>
#include <utility>
#include "../core/Entity.hpp"
#include "../core/AgentBase.hpp"

namespace DataCollection {

    /**
     * @brief Represents damage related data between an agent and other entities (agents, items, grids, etc)
     */
    class DamageData {
    private:
        std::shared_ptr<cse491::AgentBase> agent;   /// The agent that took damage
        std::shared_ptr<cse491::Entity> source;     /// The source entity that inflicted the damage
        int amount;                                 /// The amount of damage taken from this source

    public:
        /**
         * @brief Default constructor for a DamageData
         * @param src Damage source entity
         * @param amt Amount of damage taken
         */
        DamageData(std::shared_ptr<cse491::AgentBase> agnt,
                   std::shared_ptr<cse491::Entity> src,
                   int amt) : agent(std::move(agnt)), source(std::move(src)), amount(amt) {}

        /**
        * @brief Destructor for DamageData class.
        */
        ~DamageData() = default;
    };
}
