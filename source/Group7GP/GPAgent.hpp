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

#include "../core/AgentBase.hpp"

namespace cowboys
{
    using std::vector, std::string, std::map, std::shared_ptr, std::unique_ptr, std::make_shared, std::make_unique;

    class GraphNode
    {
    protected:
        vector<shared_ptr<GraphNode>> inputs;
        vector<size_t> inputs_indices;

        /// The output of this node.
        double output = 0.;

    public:
        GraphNode() = default;
        virtual ~GraphNode() = default;

        /// TODO: Check guidelines for this
        GraphNode(double default_value) : output{default_value} {}

        /// @brief Constructor for input indices
        /// @param input_indices
        GraphNode(vector<size_t> input_indices) : inputs_indices{input_indices} {}

        string name;

        /// TODO: Cache outputs
        virtual double GetOutput() const { return output; }

        shared_ptr<GraphNode> GetInput(size_t input_idx) const
        {
            if (input_idx >= inputs.size())
                // Return last input if out of bounds
                return inputs.back();
            else
                return inputs.at(input_idx);
        }

        void AddInput(shared_ptr<GraphNode> node) { inputs.push_back(node); }
        void AddInputs(const vector<shared_ptr<GraphNode>> &nodes) { inputs.insert(inputs.end(), nodes.begin(), nodes.end()); }
        void SetInputs(const vector<shared_ptr<GraphNode>> &nodes) { inputs = nodes; }
        void SetOutput(double value) { output = value; }
    };

    class GateNode : public GraphNode
    {
    public:
        GateNode() = default;
        virtual ~GateNode() = default;

        double GetOutput() const override
        {
            // input 0 is the input
            // input 1 is the condition
            if (inputs.size() < 2)
                return 0.;

            // Let the signal through if the condition is not 0
            return GetInput(1)->GetOutput() != 0. ? GetInput(0)->GetOutput() : 0.;
        }
    };

    class AndNode : public GraphNode
    {
    public:
        AndNode() = default;
        virtual ~AndNode() = default;

        double GetOutput() const override
        {
            // input 0 and 1 are the inputs
            if (inputs.size() < 2)
                return 0.;
            auto condition = GetInput(0)->GetOutput() != 0. && GetInput(1)->GetOutput() != 0.;
            return condition ? 1. : 0.;
        }
    };

    class AnyEqNode : public GraphNode
    {
    public:
        AnyEqNode() = default;
        virtual ~AnyEqNode() = default;

        double GetOutput() const override
        {
            // input 0 is the input, the rest is the values that we are checking against
            if (inputs.size() < 2)
                return 0.;
            for (size_t i = 1; i < inputs.size(); ++i)
            {
                auto condition = GetInput(0)->GetOutput() == GetInput(i)->GetOutput();
                if (condition)
                    return 1.;
            }
            return 0.;
        }
    };

    class NotNode : public GraphNode
    {
    public:
        NotNode() = default;
        virtual ~NotNode() = default;

        double GetOutput() const override
        {
            // input 0 is the value to not
            if (inputs.size() < 1)
                return 0.;
            auto condition = GetInput(0)->GetOutput() == 0.;
            return condition ? 1. : 0.;
        }
    };

    class SumNode : public GraphNode
    {
    public:
        SumNode() = default;
        virtual ~SumNode() = default;

        double GetOutput() const override
        {
            // Sum all inputs
            double sum = 0.;
            for (auto &input : inputs)
            {
                sum += input->GetOutput();
            }
            return sum;
        }
    };

    class Graph
    {
    protected:
        /// Actions that can be taken.
        vector<size_t> actions;

        /// Layers of nodes in the graph.
        vector<vector<shared_ptr<GraphNode>>> layers;

    public:
        Graph(const vector<size_t> &action_vec) : actions{action_vec} { assert(actions.size() > 0); }
        ~Graph() = default;

        size_t MakeDecision(const vector<double> &inputs)
        {
            if (layers.size() == 0)
                return actions.at(0);

            // Set inputs of input layer
            size_t i = 0;
            for (auto &node : layers[0])
            {
                double input = 0;
                if (i < inputs.size())
                    input = inputs.at(i);
                node->SetOutput(input);
                ++i;
            }

            // Get output of last layer
            vector<double> outputs;
            for (auto &node : layers.back())
            {
                outputs.push_back(node->GetOutput());
            }

            std::cout << "Outputs: ";
            for (auto &output : outputs)
            {
                std::cout << output << " ";
            }
            std::cout << std::endl;

            // Choose the action with the highest output
            auto max_output = std::max_element(outputs.begin(), outputs.end());
            size_t index = std::distance(outputs.begin(), max_output);

            // If index is out of bounds, return the last action
            size_t action = 0;
            if (index >= actions.size())
                action = actions.back();
            else
                action = actions.at(index);
            std::cout << "Action: " << action << std::endl;
            return action;
        }

        void AddLayer(const vector<shared_ptr<GraphNode>> &layer) { layers.push_back(layer); }
    };

    class GPAgent : public cse491::AgentBase
    {
    protected:
        /// The decision graph for this agent.
        unique_ptr<Graph> decision_graph;

        /// Previous action taken.
        size_t previous_action = 0;

    public:
        GPAgent(size_t id, const string &name)
            : AgentBase(id, name)
        {
        }
        ~GPAgent() = default;

        vector<size_t> EncodeActions(const std::unordered_map<std::string, size_t> &action_map)
        {
            vector<size_t> actions;
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

            decision_graph = make_unique<Graph>(EncodeActions(action_map));

            //
            // Hardcoded graph for maze world, pacing up and down
            //
            vector<shared_ptr<GraphNode>> input_layer;
            // prev_action, current_state, above_state, below_state, left_state, right_state
            shared_ptr<GraphNode> prev_action = make_shared<GraphNode>(0.);
            shared_ptr<GraphNode> current_state = make_shared<GraphNode>(0.);
            shared_ptr<GraphNode> above_state = make_shared<GraphNode>(0.);
            shared_ptr<GraphNode> below_state = make_shared<GraphNode>(0.);
            shared_ptr<GraphNode> left_state = make_shared<GraphNode>(0.);
            shared_ptr<GraphNode> right_state = make_shared<GraphNode>(0.);
            input_layer.insert(input_layer.end(), {prev_action, current_state, above_state, below_state, left_state, right_state});
            decision_graph->AddLayer(input_layer);

            // state == 1 => floor which is walkable
            vector<shared_ptr<GraphNode>> obstruction_layer;
            shared_ptr<GraphNode> up_not_blocked = make_shared<AnyEqNode>();
            up_not_blocked->AddInputs(vector<shared_ptr<GraphNode>>{above_state, make_shared<GraphNode>(1)});
            shared_ptr<GraphNode> down_not_blocked = make_shared<AnyEqNode>();
            down_not_blocked->AddInputs(vector<shared_ptr<GraphNode>>{below_state, make_shared<GraphNode>(1)});
            obstruction_layer.insert(obstruction_layer.end(), {up_not_blocked, down_not_blocked});
            decision_graph->AddLayer(obstruction_layer);

            // Separate previous action into up and down nodes
            vector<shared_ptr<GraphNode>> prev_action_layer;
            shared_ptr<GraphNode> up_prev_action = make_shared<AnyEqNode>();
            up_prev_action->AddInputs(vector<shared_ptr<GraphNode>>{prev_action, make_shared<GraphNode>(1.)});
            shared_ptr<GraphNode> down_prev_action = make_shared<AnyEqNode>();
            down_prev_action->AddInputs(vector<shared_ptr<GraphNode>>{prev_action, make_shared<GraphNode>(2.)});
            prev_action_layer.insert(prev_action_layer.end(), {up_prev_action, down_prev_action});
            decision_graph->AddLayer(prev_action_layer);

            vector<shared_ptr<GraphNode>> moving_layer;
            // If up_not_blocked and up_prev_action ? return 1 : return 0
            // If down_not_blocked and down_prev_action ? return 1 : return 0
            shared_ptr<GraphNode> keep_up = make_shared<AndNode>();
            keep_up->AddInputs(vector<shared_ptr<GraphNode>>{up_not_blocked, up_prev_action});
            shared_ptr<GraphNode> keep_down = make_shared<AndNode>();
            keep_down->AddInputs(vector<shared_ptr<GraphNode>>{down_not_blocked, down_prev_action});
            moving_layer.insert(moving_layer.end(), {keep_up, keep_down});
            decision_graph->AddLayer(moving_layer);

            // If down_blocked, turn_up
            // If up_blocked, turn_down
            vector<shared_ptr<GraphNode>> turn_layer;
            shared_ptr<GraphNode> turn_up = make_shared<NotNode>();
            turn_up->AddInputs(vector<shared_ptr<GraphNode>>{down_not_blocked});
            shared_ptr<GraphNode> turn_down = make_shared<NotNode>();
            turn_down->AddInputs(vector<shared_ptr<GraphNode>>{up_not_blocked});
            turn_layer.insert(turn_layer.end(), {turn_up, turn_down});
            decision_graph->AddLayer(turn_layer);

            // Output layer, up, down, left, right
            vector<shared_ptr<GraphNode>> action_layer;
            // move up = keep_up + turn_up,
            // move down = keep_down + turn_down,
            shared_ptr<GraphNode> up = make_shared<SumNode>();
            up->AddInputs(vector<shared_ptr<GraphNode>>{keep_up, turn_up});
            shared_ptr<GraphNode> down = make_shared<SumNode>();
            down->AddInputs(vector<shared_ptr<GraphNode>>{keep_down, turn_down});
            shared_ptr<GraphNode> left = make_shared<GraphNode>(0.);
            shared_ptr<GraphNode> right = make_shared<GraphNode>(0.);
            action_layer.insert(action_layer.end(), {up, down, left, right});
            decision_graph->AddLayer(action_layer);

            return true;
        }

        /// @brief Translates state into nodes for the decision graph.
        /// @return A vector of doubles for the decision graph.
        vector<double> EncodeState(const cse491::WorldGrid &grid,
                                   const cse491::type_options_t & /*type_options*/,
                                   const cse491::item_set_t & /*item_set*/,
                                   const cse491::agent_set_t & /*agent_set*/)
        {
            /// TODO: Implement this function properly.
            vector<double> inputs;

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
