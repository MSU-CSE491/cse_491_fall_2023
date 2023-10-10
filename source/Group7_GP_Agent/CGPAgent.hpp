/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief An Agent based on genetic programming.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>

#include "GPAgent.hpp"
#include "GraphBuilder.hpp"

namespace cowboys {
    /// Don't know the maximum size a state can be, arbitrary large number
    constexpr size_t INPUT_SIZE = 10000;

    /// Number of computational layers for each agent
    constexpr size_t NUM_LAYERS = 10;

    /// The number of nodes in each layer
    constexpr size_t NUM_NODES_PER_LAYER = 10;

    /// The number of inputs for each node
    constexpr size_t NUM_NODE_INPUTS = 10;

    /// The number of layers preceding a node's layer that the node can reference
    constexpr size_t LAYERS_BACK = 2;

    class CGPAgent : public GPAgent {
    protected:
        /// The decision graph for this agent.
        std::unique_ptr<Graph> decision_graph;

    public:
        CGPAgent(size_t id, const std::string &name) : GPAgent(id, name) {}

        /// @brief Setup graph.
        /// @return Success.
        bool Initialize() override {
            auto graph_builder = GraphBuilder();

            // decision_graph = graph_builder.CartesianGraph(INPUT_SIZE, action_map.size(), NUM_LAYERS,
            // NUM_NODES_PER_LAYER);
            decision_graph = graph_builder.VerticalPacer();

            return true;
        }

        size_t GetAction(const cse491::WorldGrid &grid, const cse491::type_options_t &type_options,
                         const cse491::item_set_t &item_set, const cse491::agent_set_t &agent_set) override {
            auto inputs = EncodeState(grid, type_options, item_set, agent_set, this, extra_state);
            size_t action_to_take = decision_graph->MakeDecision(inputs, EncodeActions(action_map));
            return action_to_take;
        }
    };

} // End of namespace cowboys
