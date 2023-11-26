
#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include "../../core/AgentBase.hpp"
#include "GPAgentSensors.hpp"

#include "./GPAgentBase.hpp"

namespace cowboys
{
    const int LISTSIZE = 100;

    class LGPAgent : public GPAgentBase
    {
    protected:
        // A dictionary of actions and a dictionary of sensors
        // A sensor is a function that takes in a grid and returns a value (e.g. distance to nearest agent)

        // For example group 1 has a function for the shortest path

        std::vector<std::string> possibleInstructionsList = {};
        std::vector<std::string> actionsList = {};
        std::vector<std::string> operationsList = {"lessthan", "greaterthan", "equals"};
        std::vector<std::string> sensorsNamesList = {"getLeft", "getRight", "getUp", "getDown"};
        std::vector<int> resultsList;

        std::vector<std::tuple<std::string, int, int>> instructionsList = {};
        size_t currentInstructionIndex = 0;

        std::random_device rd;
        std::mt19937 gen;

    public:
        LGPAgent(size_t id, const std::string &name) : GPAgentBase(id, name)
        {
            gen = std::mt19937(rd());

            for (auto i = 0; i < LISTSIZE; i++)
            {
                resultsList.push_back(0);
            }
        }



        /// @brief This agent needs a specific set of actions to function.
        /// @return Success.
        bool Initialize() override
        {
            possibleInstructionsList = EncodeActions(action_map, sensorsNamesList, operationsList, actionsList);
            GenerateRandomActionList();
            return true;
        }

        void GenerateRandomActionList()
        {
            // generate a random list of actions
            std::uniform_int_distribution<size_t> dist(0, possibleInstructionsList.size() - 1);
            std::uniform_int_distribution<size_t> dist2(0, LISTSIZE - 1);
            for (int i = 0; i < LISTSIZE; i++)
            {
                instructionsList.push_back(std::make_tuple(possibleInstructionsList[dist(gen)], dist2(gen), dist2(gen)));
            }

        }

        /// @brief Encodes the actions from an agent's action map into a vector of string, representing action names.
        /// @param action_map The action map from the agent.
        /// @return A vector of strings, representing action names.
        static std::vector<std::string> EncodeActions(const std::unordered_map<std::string, size_t> &action_map, const std::vector<std::string> &sensorsNamesList, const std::vector<std::string> &operationsList, std::vector<std::string> &actionsList)
        {
            std::vector<std::string> instructions;
            for (const auto &[action_name, action_id] : action_map)
            {
                instructions.push_back(action_name);
                actionsList.push_back(action_name);
            }
            for (const auto &sensor : operationsList)
            {
                instructions.push_back(sensor);
            }

            for (const auto &sensor : sensorsNamesList)
            {
                instructions.push_back(sensor);
            }

            return instructions;
        }


        void MutateAgent(double mutation_rate = 0.01) override
        {
            std::uniform_int_distribution<size_t> rnd_mutate(1, 100);
            std::uniform_int_distribution<size_t> dist(0, possibleInstructionsList.size() - 1);
            std::uniform_int_distribution<size_t> dist2(0, LISTSIZE - 1);

            for (auto i = 0; i < LISTSIZE; i++)
            {
                if (rnd_mutate(gen) / 100.0 <= mutation_rate)
                {
                    instructionsList[i] = std::make_tuple(possibleInstructionsList[dist(gen)], dist2(gen), dist2(gen));
                }
            }


//            resultsList = std::vector<int>(LISTSIZE);
            resultsList.clear();
            resultsList.resize(LISTSIZE);
            currentInstructionIndex = 0;
        }

        /// @brief Get the instruction list for this agent.
        /// @return A const reference to the instruction list for this agent.
        const std::vector<std::tuple<std::string, int, int>> &GetInstructionsList(){ return instructionsList; }

        /// @brief Copies the behavior of another LGPAgent into this agent.
        /// @param other The LGPAgent to copy.
        void Configure(const LGPAgent &other) {
            instructionsList = other.instructionsList;
            possibleInstructionsList = other.possibleInstructionsList;
            actionsList = other.actionsList;
            operationsList = other.operationsList;
            sensorsNamesList = other.sensorsNamesList;
            resultsList = other.resultsList;
            currentInstructionIndex = other.currentInstructionIndex;
        }

        /// @brief Copy the behavior of another agent into this agent.
        /// @param other The agent to copy.
        void Copy(const GPAgentBase &other) override
        {
            assert(dynamic_cast<const LGPAgent *>(&other) != nullptr);
            Configure(dynamic_cast<const LGPAgent &>(other));
        }

        std::string Export() {
          return "";
        }

        size_t GetAction([[maybe_unused]] const cse491::WorldGrid &grid,
                            [[maybe_unused]] const cse491::type_options_t &type_options,
                            [[maybe_unused]] const cse491::item_map_t &item_set,
                            [[maybe_unused]] const cse491::agent_map_t &agent_set) override
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

            while (i < LISTSIZE * 2 && action.empty())
            {
                if (std::find(actionsList.begin(), actionsList.end(), std::get<0>(instruction)) != actionsList.end())
                {
                    action = std::get<0>(instruction);
                }
                else if (std::find(sensorsNamesList.begin(), sensorsNamesList.end(), std::get<0>(instruction)) != sensorsNamesList.end())
                {
                    // the instruction is in the sensor list (getLeft, getRight, getUp, getDown)
                    sensor = std::get<0>(instruction);

                    SensorDirection direction = Sensors::getSensorDirectionEnum(sensor);
                    int distance = Sensors::wallDistance(grid, *this, direction);


                    resultsList[currentInstructionIndex] = distance;


                }
                else
                {
                    // the instruction is an operation (lessthan, greaterthan, equals)
                    operation = std::get<0>(instruction);
                    if (operation == "lessthan")
                    {
                        if (std::get<1>(instruction) < std::get<2>(instruction))
                        {
                            resultsList[currentInstructionIndex] = 1;
                        }
                        else
                        {
                            resultsList[currentInstructionIndex] = 0;
                            ++currentInstructionIndex;
                        }
                    }
                    else if (operation == "greaterthan")
                    {
                        if (std::get<1>(instruction) > std::get<2>(instruction))
                        {
                            resultsList[currentInstructionIndex] = 1;
                        }
                        else
                        {
                            resultsList[currentInstructionIndex] = 0;
                            ++currentInstructionIndex;
                        }
                    }
                    else if (operation == "equals")
                    {
                        if (std::get<1>(instruction) == std::get<2>(instruction))
                        {
                            resultsList[currentInstructionIndex] = 1;
                        }
                        else
                        {
                            resultsList[currentInstructionIndex] = 0;
                            ++currentInstructionIndex;
                        }
                    }
                }

                ++currentInstructionIndex;
                if (currentInstructionIndex >= LISTSIZE)
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

        void Serialize(tinyxml2::XMLDocument &, tinyxml2::XMLElement *, double fitness = -1) override {}

        void PrintAgent() override {
          for (auto i = 0; i < LISTSIZE; i++)
          {
            std::cout << std::get<0>(instructionsList[i]) << " ";
          }
          std::cout << std::endl;
        }
    };
}