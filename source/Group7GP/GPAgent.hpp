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

            decision_graph = std::make_unique<Graph>(EncodeActions(action_map));

            //
            // Hardcoded graph for maze world, pacing up and down
            //
            std::vector<std::shared_ptr<GraphNode>> input_layer;
            // prev_action, current_state, above_state, below_state, left_state, right_state
            std::shared_ptr<GraphNode> prev_action = std::make_shared<GraphNode>(0.);
            std::shared_ptr<GraphNode> current_state = std::make_shared<GraphNode>(0.);
            std::shared_ptr<GraphNode> above_state = std::make_shared<GraphNode>(0.);
            std::shared_ptr<GraphNode> below_state = std::make_shared<GraphNode>(0.);
            std::shared_ptr<GraphNode> left_state = std::make_shared<GraphNode>(0.);
            std::shared_ptr<GraphNode> right_state = std::make_shared<GraphNode>(0.);
            input_layer.insert(input_layer.end(), {prev_action, current_state, above_state, below_state, left_state, right_state});
            decision_graph->AddLayer(input_layer);

            // state == 1 => floor which is walkable
            std::vector<std::shared_ptr<GraphNode>> obstruction_layer;
            std::shared_ptr<GraphNode> up_not_blocked = std::make_shared<AnyEqNode>();
            up_not_blocked->AddInputs(std::vector<std::shared_ptr<GraphNode>>{above_state, std::make_shared<GraphNode>(1)});
            std::shared_ptr<GraphNode> down_not_blocked = std::make_shared<AnyEqNode>();
            down_not_blocked->AddInputs(std::vector<std::shared_ptr<GraphNode>>{below_state, std::make_shared<GraphNode>(1)});
            obstruction_layer.insert(obstruction_layer.end(), {up_not_blocked, down_not_blocked});
            decision_graph->AddLayer(obstruction_layer);

            // Separate previous action into up and down nodes
            std::vector<std::shared_ptr<GraphNode>> prev_action_layer;
            std::shared_ptr<GraphNode> up_prev_action = std::make_shared<AnyEqNode>();
            up_prev_action->AddInputs(std::vector<std::shared_ptr<GraphNode>>{prev_action, std::make_shared<GraphNode>(1.)});
            std::shared_ptr<GraphNode> down_prev_action = std::make_shared<AnyEqNode>();
            down_prev_action->AddInputs(std::vector<std::shared_ptr<GraphNode>>{prev_action, std::make_shared<GraphNode>(2.)});
            prev_action_layer.insert(prev_action_layer.end(), {up_prev_action, down_prev_action});
            decision_graph->AddLayer(prev_action_layer);

            std::vector<std::shared_ptr<GraphNode>> moving_layer;
            // If up_not_blocked and up_prev_action ? return 1 : return 0
            // If down_not_blocked and down_prev_action ? return 1 : return 0
            std::shared_ptr<GraphNode> keep_up = std::make_shared<AndNode>();
            keep_up->AddInputs(std::vector<std::shared_ptr<GraphNode>>{up_not_blocked, up_prev_action});
            std::shared_ptr<GraphNode> keep_down = std::make_shared<AndNode>();
            keep_down->AddInputs(std::vector<std::shared_ptr<GraphNode>>{down_not_blocked, down_prev_action});
            moving_layer.insert(moving_layer.end(), {keep_up, keep_down});
            decision_graph->AddLayer(moving_layer);

            // If down_blocked, turn_up
            // If up_blocked, turn_down
            std::vector<std::shared_ptr<GraphNode>> turn_layer;
            std::shared_ptr<GraphNode> turn_up = std::make_shared<NotNode>();
            turn_up->AddInputs(std::vector<std::shared_ptr<GraphNode>>{down_not_blocked});
            std::shared_ptr<GraphNode> turn_down = std::make_shared<NotNode>();
            turn_down->AddInputs(std::vector<std::shared_ptr<GraphNode>>{up_not_blocked});
            turn_layer.insert(turn_layer.end(), {turn_up, turn_down});
            decision_graph->AddLayer(turn_layer);

            // Output layer, up, down, left, right
            std::vector<std::shared_ptr<GraphNode>> action_layer;
            // move up = keep_up + turn_up,
            // move down = keep_down + turn_down,
            std::shared_ptr<GraphNode> up = std::make_shared<SumNode>();
            up->AddInputs(std::vector<std::shared_ptr<GraphNode>>{keep_up, turn_up});
            std::shared_ptr<GraphNode> down = std::make_shared<SumNode>();
            down->AddInputs(std::vector<std::shared_ptr<GraphNode>>{keep_down, turn_down});
            std::shared_ptr<GraphNode> left = std::make_shared<GraphNode>(0.);
            std::shared_ptr<GraphNode> right = std::make_shared<GraphNode>(0.);
            action_layer.insert(action_layer.end(), {up, down, left, right});
            decision_graph->AddLayer(action_layer);

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
