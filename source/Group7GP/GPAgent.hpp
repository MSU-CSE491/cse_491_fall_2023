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

        /// The default output of this node.
        double output = 0.;

    public:
        GraphNode() = default;

        /// TODO: Check guidelines for this
        GraphNode(double default_value) : output{default_value} {}
        virtual ~GraphNode() = default;

        string name;

        /// TODO: Cache outputs
        virtual double GetOutput() const
        {
            return output;
        }

        virtual void AddInput(shared_ptr<GraphNode> node)
        {
            inputs.push_back(node);
        }

        virtual void SetInputs(const vector<shared_ptr<GraphNode>> &nodes)
        {
            inputs = nodes;
        }
    };

    class IfNode : public GraphNode
    {
    public:
        IfNode() = default;
        virtual ~IfNode() = default;

        double GetOutput() const override
        {
            // input 0 and 1 are the inputs
            // input 2 is the condition
            if (inputs.size() < 3)
                return 0.;
            else
                return inputs[2]->GetOutput() != 0. ? inputs[0]->GetOutput() : inputs[1]->GetOutput();
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

        size_t MakeDecision(const vector<shared_ptr<GraphNode>> &inputs)
        {
            if (layers.size() == 0)
                return actions.at(0);

            // Set inputs of first layer
            for (auto &node : layers[0])
            {
                node->SetInputs(inputs);
            }

            // Get output of last layer
            vector<double> outputs;
            for (auto &node : layers.back())
            {
                outputs.push_back(node->GetOutput());
            }

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

        void AddLayer(const vector<shared_ptr<GraphNode>> &layer)
        {
            layers.push_back(layer);
        }
    };

    class GPAgent : public cse491::AgentBase
    {
    protected:
        /// The decision graph for this agent.
        unique_ptr<Graph> decision_graph;

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
            // forward -> blocked? -> turnleft
            // forward -> not blocked? -> forward
            // turnleft -> not blocked? -> forward
            // turnleft -> blocked? -> turnleft
            auto actions = EncodeActions(action_map);

            std::cout << "Actions: ";
            for (auto &action : actions)
            {
                std::cout << action << " ";
            }
            std::cout << std::endl;

            decision_graph = make_unique<Graph>(EncodeActions(action_map));
            
            // Hardcoded graph
            vector<shared_ptr<GraphNode>> action_layer;
            shared_ptr<GraphNode> up = make_shared<GraphNode>(0.);
            shared_ptr<GraphNode> down = make_shared<GraphNode>(12.);
            shared_ptr<GraphNode> left = make_shared<GraphNode>(2.);
            shared_ptr<GraphNode> right = make_shared<GraphNode>(1.);
            action_layer.insert(action_layer.end(), {up, down, left, right});
            decision_graph->AddLayer(action_layer);

            return true;
        }

        /// @brief Translates state into nodes for the decision graph.
        /// @return A vector of doubles for the decision graph.
        vector<shared_ptr<GraphNode>> EncodeState(const cse491::WorldGrid &grid,
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

            inputs.insert(inputs.end(), {current_state, above_state, below_state, left_state, right_state});

            vector<shared_ptr<GraphNode>> nodes;
            for (const auto &input : inputs)
            {
                nodes.push_back(make_shared<GraphNode>(input));
            }
            return nodes;
        }

        /// Choose the action to take a step in the appropriate direction.
        size_t SelectAction(const cse491::WorldGrid &grid,
                            const cse491::type_options_t &type_options,
                            const cse491::item_set_t &item_set,
                            const cse491::agent_set_t &agent_set) override
        {
            auto inputs = EncodeState(grid, type_options, item_set, agent_set);
            return decision_graph->MakeDecision(inputs);
        }
    };

} // End of namespace cowboys
