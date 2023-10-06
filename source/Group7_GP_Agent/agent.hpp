/**
 * @file agent.hpp
 * @author Aman Dhruva Thamminana (thammina@msu.edu)
 * @brief this will be our agent class can be created
 * @version 0.0.1 [internal]
 * @date 2023-09-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#pragma once

#include <cassert>
#include <map>
#include <string>
#include <iostream>

#include <random>

#include "../core/AgentBase.hpp"
#include "./GPAgentSensors.hpp"

/**
 * @brief yeeeeeeeehaaaaaaaaa
 */
namespace cowboys
{


    class GPAgent : public cse491::AgentBase
    {
    protected:
        const std::vector<std::string> predefinedMovement = {"down", "down","down","down", "right", "right", "up", "up", "up","up", "right", "right",  "right",  "right",  "right",  "right",  "right", "right", "right",  "right", "right", "left", "left", "left"};

        size_t movementIndex = 0; // current move of the agent

    public:
        GPAgent(size_t id, const std::string &name) : AgentBase(id, name) {
            std::random_device rd;
            std::mt19937 gen(rd());


            std::uniform_int_distribution<int> dist(0, 3); // size of the random set to 3 as we only have 4 actions initally
        }
        ~GPAgent() override = default;

        /// @brief This agent needs a specific set of actions to function.
        /// @return Success.
        bool Initialize() override
        {
            return true;
        }


        /// Choose the action to take a step in the appropriate direction.
        size_t SelectAction(const cse491::WorldGrid &grid,
                            const cse491::type_options_t &type_options,
                            const cse491::item_set_t &item_set,
                            const cse491::agent_set_t &agent_set) override
        {
            if (movementIndex >= predefinedMovement.size()){
                return action_map["right"]; // do nothing if it is out of bound for defined movement
            }

            Sensors::leftWallDistance(grid, *this);
            Sensors::rightWallDistance(grid, *this);
            Sensors::topWallDistance(grid, *this);
            Sensors::bottomWallDistance(grid, *this);

            auto action = action_map[predefinedMovement[movementIndex++]];
            return action;
        }


    };

}