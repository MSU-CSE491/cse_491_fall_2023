
#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include "../core/AgentBase.hpp"
#include "GPAgentSensors.hpp"

const int LISTSIZE = 100;

#define EXPERIMENTAL_CLASS __attribute__((annotate("experimental_class"), warning("This is an experimental class and should be used with caution.")))
#define EXPERIMENTAL_FUNCTION __attribute__((annotate("experimental_function"), warning("This is an experimental function and should be used with caution.")))


namespace cowboys
{

    EXPERIMENTAL_CLASS class LGPAgent : public cse491::AgentBase
    {
    protected:
        // A dictionary of actions and a dictionary of sensors
        // A sensor is a function that takes in a grid and returns a value (e.g. distance to nearest agent)

        // For example group 1 has a function for the shortest path
        // TODO: ADD A STAY STATEMENT WORLD

        std::vector<std::string> possibleInstructionsList = {};
        std::vector<std::string> actionsList = {"up", "down", "left", "right"};
        std::vector<std::string> operationsList = {"lessthan", "greaterthan", "equals"};
        std::vector<int> resultsList = std::vector<int>(LISTSIZE);

        std::vector<std::tuple<std::string, int, int>> instructionsList = {};
        size_t currentInstructionIndex = 0;

        std::vector<cowboys::Sensors> sensorsList;
        std::vector<std::string> sensorsNamesList = {"getLeft", "getRight", "getUp", "getDown"};

        bool hasActionOccured = false;

        std::random_device rd;
        std::mt19937 gen;

    public:
        EXPERIMENTAL_FUNCTION LGPAgent(size_t id, const std::string &name) : AgentBase(id, name)
        {
            // TODO: dont care didnt ask ignore this
            gen = std::mt19937(rd());
        }

        ~LGPAgent() override = default;

        /// @brief This agent needs a specific set of actions to function.
        /// @return Success.
        EXPERIMENTAL_FUNCTION bool Initialize() override
        {
            possibleInstructionsList = EncodeActions(action_map, sensorsNamesList);
            GenerateRandomActionList();
            return true;
        }

        EXPERIMENTAL_FUNCTION void GenerateRandomActionList()
        {
            // generate a random list of actions
            std::uniform_int_distribution<size_t> dist(0, possibleInstructionsList.size() - 1);
            std::uniform_int_distribution<size_t> dist2(0, resultsList.size() - 1);
            for (int i = 0; i < LISTSIZE; i++)
            {
                instructionsList.push_back(std::make_tuple(possibleInstructionsList[dist(gen)], dist2(gen), dist2(gen)));
            }
        }

        /// @brief Encodes the actions from an agent's action map into a vector of string, representing action names.
        /// @param action_map The action map from the agent.
        /// @return A vector of strings, representing action names.
        EXPERIMENTAL_FUNCTION static std::vector<std::string> EncodeActions(const std::unordered_map<std::string, size_t> &action_map, const std::vector<std::string> &sensorsNamesList)
        {
            std::vector<std::string> instructions;
            for (const auto &[action_name, action_id] : action_map)
            {
                instructions.push_back(action_name);
            }
            instructions.push_back("lessthan");
            instructions.push_back("greaterthan");
            instructions.push_back("equals");

            for (const auto &sensor : sensorsNamesList)
            {
                instructions.push_back(sensor);
            }

            return instructions;
        }

        EXPERIMENTAL_FUNCTION size_t SelectAction([[maybe_unused]] const cse491::WorldGrid &grid,
                            [[maybe_unused]] const cse491::type_options_t &type_options,
                            [[maybe_unused]] const cse491::item_set_t &item_set,
                            [[maybe_unused]] const cse491::agent_set_t &agent_set) override
        {

            std::string action;
            std::string sensor;
            std::string operation;
            auto instruction = instructionsList[currentInstructionIndex];
            int i = 0;

            if (currentInstructionIndex != 0)
            {
                resultsList[currentInstructionIndex - 1] = action_result;
            }
            else
            {
                resultsList[LISTSIZE - 1] = action_result;
            }

            while (i < LISTSIZE && action.empty())
            {
                ++currentInstructionIndex;
                if (std::find(actionsList.begin(), actionsList.end(), get<0>(instruction)) != actionsList.end())
                {
                    action = get<0>(instruction);
                }
                else if (std::find(sensorsNamesList.begin(), sensorsNamesList.end(), get<0>(instruction)) != sensorsNamesList.end())
                {
                    // the instruction is in the sensor list (getLeft, getRight, getUp, getDown)
                    sensor = get<0>(instruction);

                    SensorDirection direction = Sensors::getSensorDirectionEnum(sensor);
                    int distance = Sensors::wallDistance(grid, *this, direction);
                    resultsList[currentInstructionIndex - 1] = distance;
                }

                else
                {
                    // the instruction is an operation (lessthan, greaterthan, equals)
                    operation = get<0>(instruction);
                }

                if (currentInstructionIndex >= instructionsList.size())
                {
                    currentInstructionIndex = 0;
                }
                ++i;
                instruction = instructionsList[currentInstructionIndex];
            }
            if (!action.empty())
            {
                return action_map[action];
            }

            return 0;
        }
    };
}