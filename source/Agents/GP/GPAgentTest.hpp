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
#include <iostream>
#include <map>
#include <random>
#include <string>

#include "../../core/AgentBase.hpp"
#include "./GPAgentSensors.hpp"

/**
 * @brief Namespace for GPagent and its related classes
 *
 * @note yeeeeeeeehaaaaaaaaa 🤠
 */
namespace cowboys {

    class GPAgentTest : public GPAgent_ {
    protected:
        const std::vector<std::string> predefinedMovement = {
                "down",  "down",  "down",  "down",  "right", "right", "up",    "up",
                "up",    "up",    "right", "right", "right", "right", "right", "right",
                "right", "right", "right", "right", "right", "left",  "left",  "left"};

        size_t movementIndex = 0;  ///< current move of the agent

    public:
        GPAgentTest(size_t id, const std::string &name) : GPAgent_(id, name) {
          std::random_device rd;
          std::mt19937 gen(rd());

          std::uniform_int_distribution<int> dist(
                  0,
                  3);  // size of the random set to 3 as we only have 4 actions initally
        }
        ~GPAgentTest() override = default;

        /// @brief This agent needs a specific set of actions to function.
        /// @return Success.
        bool Initialize() override { return true; }


        virtual size_t GetAction(const cse491::WorldGrid &grid, const cse491::type_options_t &type_options,
                                 const cse491::item_map_t &item_set, const cse491::agent_map_t &agent_set) {};

        virtual void MutateAgent(double mutation_rate = 0.8) {};

        /// @brief Copy the behavior of another agent into this agent.
        /// @param other The agent to copy. Should be the same type.
        virtual void Copy(const GPAgent_ &other) {};

        virtual void printAgent() {};

        virtual std::string Export() { return ""; }

        virtual void crossover(const GPAgent_ &other) {};
        virtual void Import(const std::string &genotype) {};


    };

}  // namespace cowboys