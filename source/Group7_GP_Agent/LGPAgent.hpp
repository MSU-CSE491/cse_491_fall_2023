
#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <random>
#include "../core/AgentBase.hpp"

const int MAX_MOVEMENTS = 100;

namespace cowboys {

    class LGPAgent : public cse491::AgentBase {
    protected:
        // A dictionary of actions and a dictionary of sensors
        // A sensor is a function that takes in a grid and returns a value (e.g. distance to nearest agent)

        // For example group 1 has a function for the shortest path
        // TODO: ADD A STAY STATEMENT WORLD
        std::vector<std::string> actionsList = {};

        std::vector<std::string> instructionsList = {};
        size_t currentInstructionIndex = 0;

        size_t movementIndex = 0; // current move of the agent
        std::random_device rd;
        std::mt19937 gen;

    public:
        LGPAgent(size_t id, const std::string &name) : AgentBase(id, name) {
            // TODO: dont care didnt ask ignore this
            gen = std::mt19937(rd());
        }

        ~LGPAgent() override = default;

        /// @brief This agent needs a specific set of actions to function.
        /// @return Success.
        bool Initialize() override {
            actionsList = EncodeActions(action_map);
            GenerateRandomActionList();
            return true;
        }

        void GenerateRandomActionList() {
            // generate a random list of actions
            std::uniform_int_distribution<size_t> dist(0, actionsList.size() - 1);
            for (int i = 0; i < MAX_MOVEMENTS; i++) {
                instructionsList.push_back(actionsList[dist(gen)]);
            }

            for (auto &action : instructionsList) {
                std::cout << action << " ";
            }
            std::cout << std::endl;

        }

        /// @brief Encodes the actions from an agent's action map into a vector of string, representing action names.
        /// @param action_map The action map from the agent.
        /// @return A vector of strings, representing action names.
        static std::vector<std::string> EncodeActions(const std::unordered_map<std::string, size_t> &action_map)
        {
            std::vector<std::string> actions;
            for (const auto &[action_name, action_id] : action_map)
            {
                actions.push_back(action_name);
            }
            return actions;
        }

        size_t SelectAction(const cse491::WorldGrid &grid,
                            const cse491::type_options_t &type_options,
                            const cse491::item_set_t &item_set,
                            const cse491::agent_set_t &agent_set) override {

            auto action = action_map[instructionsList[currentInstructionIndex++]];

            if (currentInstructionIndex >= instructionsList.size()) {
                currentInstructionIndex = 0;
            }

            return action;

        }
    };
}

