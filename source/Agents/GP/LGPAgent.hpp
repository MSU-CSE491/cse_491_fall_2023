/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief An Agent based on linear genetic programming.
 **/

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
        // A sensor is a function that takes in a grid and returns a value (e.g. distance to left wall)

        std::vector<std::string> possibleInstructionsList = {};
        std::vector<std::string> actionsList = {};
        std::vector<std::string> operationsList = {"lessthan", "greaterthan", "equals"};
        std::vector<std::string> sensorsNamesList = {"getLeft", "getRight", "getUp", "getDown"};

        // A list that stores the results of executed instructions
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

        /// @brief Initialize the agent after being given an action map
        /// @return Success.
        bool Initialize() override
        {
            possibleInstructionsList = EncodeActions(action_map, sensorsNamesList, operationsList, actionsList);
            GenerateRandomActionList();
            return true;
        }

        /// @brief Generate a random list of instructions from a list of possible instructions
        /// @return Success.
        void GenerateRandomActionList()
        {
            std::uniform_int_distribution<size_t> dist(0, possibleInstructionsList.size() - 1);
            std::uniform_int_distribution<size_t> dist2(0, LISTSIZE - 1);
            for (int i = 0; i < LISTSIZE; i++)
            {
                instructionsList.push_back(std::make_tuple(possibleInstructionsList[dist(gen)], dist2(gen), dist2(gen)));
            }

        }

        /// @brief Encodes the actions from an agent's action map into a vector of string, representing action names.
        /// @param action_map The action map from the agent.
        /// @param sensorsNamesList The list of sensors from the agent.
        /// @param operationsList The list of operations from the agent.
        /// @param actionsList The list of actions from the agent.
        /// @return A vector of strings, representing action names.
        static std::vector<std::string> EncodeActions(const std::unordered_map<std::string, size_t> &action_map, const std::vector<std::string> &sensorsNamesList, 
                                                        const std::vector<std::string> &operationsList, std::vector<std::string> &actionsList)
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

        /// @brief Mutate this agent.
        /// @param mutation_rate The probability of any instruction being changed
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


        /// @brief Get the action to take.
        /// @param grid The world grid.
        /// @param type_options The available types of cells in the grid.
        /// @param item_set The set of items in the world.
        /// @param agent_set The set of agents in the world.
        /// @return A size_t corresponding to the action chosen
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
                    // the instruction is in the action list (provided by the world)
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


        /// @brief Export the agent to a string
        /// @return The string representation of the agent
        std::string Export() override {
            std::string encodedLists = "";

            for (auto instruction : instructionsList)
            {
                encodedLists += std::get<0>(instruction);
                encodedLists += ".";
                encodedLists += std::to_string(std::get<1>(instruction));
                encodedLists += ".";
                encodedLists += std::to_string(std::get<2>(instruction));
                encodedLists += ",";
            }

            encodedLists += ";";

            for (auto possInstruction : possibleInstructionsList)
            {
                encodedLists += possInstruction;
                encodedLists += ".";
            }

            encodedLists += ";";

            for (auto action : actionsList)
            {
                encodedLists += action;
                encodedLists += ".";
            }

            return encodedLists;
        }

        /// @brief Serialize this agent to XML.
        /// @param doc The XML document to serialize to.
        /// @param parentElem The parent element to serialize to.
        /// @param fitness The fitness of this agent to write to the XML.
        void SerializeGP(tinyxml2::XMLDocument & doc, tinyxml2::XMLElement* parentElem, double fitness = -1) override
        {
            auto agentElem = doc.NewElement("LGPAgent");
            parentElem->InsertEndChild(agentElem);

            auto listElem = doc.NewElement("instruction list");
            listElem->SetText(Export().c_str());
            if (fitness != -1)
                listElem->SetAttribute("fitness", fitness);
            agentElem->InsertEndChild(listElem);
        }

        /// @brief Load in the string representation of an LGP agent and configure this agent based on it.
        /// @param genotype The string representation of an LGP agent.
        void Import(const std::string & encodedLists) override {
            std::vector<std::tuple<std::string, int, int>> decodedInstructionsList = {};
            std::string decodedInstruction;
            size_t start_pos = 0;
            size_t first_period_pos;
            size_t second_period_pos;
            size_t comma_pos = encodedLists.find(",");

            // Load the instruction list
            while (comma_pos != std::string::npos) {
                decodedInstruction = encodedLists.substr(start_pos, comma_pos - start_pos);
                first_period_pos = decodedInstruction.find(".");
                second_period_pos = decodedInstruction.find(".", first_period_pos + 1);
                decodedInstructionsList.push_back(std::make_tuple(decodedInstruction.substr(0, first_period_pos),
                        std::stoi(decodedInstruction.substr(first_period_pos+1, second_period_pos-first_period_pos+1)), std::stoi(decodedInstruction.substr(second_period_pos+1))));

                start_pos = comma_pos + 1;
                comma_pos = encodedLists.find(",", start_pos);
            }

            std::vector<std::string> decodedPossInstructionsList = {};
            std::vector<std::string> decodedActionsList = {};
            size_t first_semicolon_pos = encodedLists.find(";");
            size_t second_semicolon_pos = encodedLists.find(";", first_semicolon_pos+1);
            std::string unseparated_instruction_list = encodedLists.substr(first_semicolon_pos+1, second_semicolon_pos-first_semicolon_pos+1);
            std::string unseparated_action_list = encodedLists.substr(second_semicolon_pos+1);

            // Load the list of possible instructions
            size_t period_pos = unseparated_instruction_list.find(".");
            start_pos = 0;
            while (period_pos != std::string::npos) {
                decodedPossInstructionsList.push_back(unseparated_instruction_list.substr(start_pos, period_pos));

                start_pos = period_pos + 1;
                period_pos = encodedLists.find(".", start_pos);
            }

            // Load the list of actions
            period_pos = unseparated_action_list.find(".");
            start_pos = 0;
            while (period_pos != std::string::npos) {
                decodedActionsList.push_back(unseparated_action_list.substr(start_pos, period_pos));

                start_pos = period_pos + 1;
                period_pos = encodedLists.find(".", start_pos);
            }

            instructionsList = decodedInstructionsList;
            possibleInstructionsList = decodedPossInstructionsList;
            actionsList = decodedActionsList;
            resultsList.clear();
            resultsList.resize(LISTSIZE);
            currentInstructionIndex = 0;
        }

        /// @brief Print the agent
        void PrintAgent() override {
            for (auto i = 0; i < LISTSIZE; i++)
            {
                std::cout << std::get<0>(instructionsList[i]) << " ";
            }
            std::cout << std::endl;
        }
    };
}
