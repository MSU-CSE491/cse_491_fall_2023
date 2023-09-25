/**
 * This file is part of the Fall 2023, CSE 491 course project - Group 1 fork - monika_k branch.
 * @brief An Agent that will walk.
 * @note Status: PROPOSAL
 **/

#pragma once

#include "../core/AgentBase.hpp"

namespace cse491 {
    class BasicAgent : public AgentBase {
    protected:
        std::unordered_map<std::string, double> a_star_map;     // Map of the tiles on the board with their corresponding values for a* search
        bool isMoving = false;                                  // Is the agent moving?
        int intelligenceLevel = 0;                                  // Depending on the type of agent, mobility varies
                                                                // Measure of intelligence; if agent is an enemy, are they "smart enough" to be able to use a*, or would they resort to
                                                                // another search method? If the agent is the user/player's agent they would have maximum intelligence.
        int healthPoints = 10;                                  
        int mobility = 20;                                      // Number of tiles the agent can move in a given turn?
        int level = 1;                                          // Level of character; scales the rest of their attributes.

    public:
        BasicAgent(size_t id, const std::string & name) : AgentBase(id, name) { }
        ~BasicAgent() = default;

        // a_star function here

        // could also add getters/setters here for different attributes

    };
}