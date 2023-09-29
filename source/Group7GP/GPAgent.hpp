/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief An Agent based on genetic programming.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <cassert>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include "../Group7GP/GPGraph.hpp"

#include "../core/AgentBase.hpp"

namespace cowboys
{
    class GPAgent : public cse491::AgentBase
    {
    protected:
        /// The decision graph for this agent.
        std::unique_ptr<Graph> decision_graph;

        /// Previous action taken.
        size_t previous_action = 0;

    public:
        GPAgent(size_t id, const std::string &name)
            : AgentBase(id, name)
        {
        }
        ~GPAgent() = default;

        std::vector<size_t> EncodeActions(const std::unordered_map<std::string, size_t> &action_map)
        {
            std::vector<size_t> actions;
            for (const auto &[action_name, action_id] : action_map)
            {
                actions.push_back(action_id);
            }
            // Sort the actions so that they are in a consistent order.
            std::sort(actions.begin(), actions.end());
            return actions;
        }

        /// @brief Setup graph.
        /// @return Success.
        bool Initialize() override
        {
            auto actions = EncodeActions(action_map);

            std::cout << "Actions: ";
            for (auto &action : actions)
            {
                std::cout << action << " ";
            }
            std::cout << std::endl;

            auto graph_builder = std::make_unique<GraphBuilder>(EncodeActions(action_map));

            decision_graph = graph_builder->VerticalPacer();

            return true;
        }

        /// @brief Translates state into nodes for the decision graph.
        /// @return A vector of doubles for the decision graph.
        std::vector<double> EncodeState(const cse491::WorldGrid &grid,
                                        const cse491::type_options_t & /*type_options*/,
                                        const cse491::item_set_t & /*item_set*/,
                                        const cse491::agent_set_t & /*agent_set*/)
        {
            /// TODO: Implement this function properly.
            std::vector<double> inputs;

            auto current_position = GetPosition();

            double current_state = grid.At(current_position);
            double above_state = grid.IsValid(current_position.Above()) ? grid.At(current_position.Above()) : 0.;
            double below_state = grid.IsValid(current_position.Below()) ? grid.At(current_position.Below()) : 0.;
            double left_state = grid.IsValid(current_position.ToLeft()) ? grid.At(current_position.ToLeft()) : 0.;
            double right_state = grid.IsValid(current_position.ToRight()) ? grid.At(current_position.ToRight()) : 0.;
            double prev_action = previous_action;

            inputs.insert(inputs.end(), {prev_action, current_state, above_state, below_state, left_state, right_state});

            std::cout << "Inputs: "
                      << "prev_a=" << prev_action << " current=" << current_state << " above=" << above_state << " below=" << below_state << " left=" << left_state << " right=" << right_state << std::endl;

            return inputs;
        }

        /// Choose the action to take a step in the appropriate direction.
        size_t SelectAction(const cse491::WorldGrid &grid,
                            const cse491::type_options_t &type_options,
                            const cse491::item_set_t &item_set,
                            const cse491::agent_set_t &agent_set) override
        {
            auto inputs = EncodeState(grid, type_options, item_set, agent_set);
            size_t action_to_take = decision_graph->MakeDecision(inputs);
            previous_action = action_to_take;
            return action_to_take;
        }
    };

} // End of namespace cowboys
