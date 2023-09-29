/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief An Agent based on genetic programming.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cassert>
#include "../core/AgentBase.hpp"
#include "../Group7GP/GPGraph.hpp"

namespace cowboys
{
    class GPAgent : public cse491::AgentBase
    {
    protected:
        /// The decision graph for this agent.
        std::unique_ptr<Graph> decision_graph;

        /// A map of extra state information.
        std::unordered_map<std::string, double> extra_state;

    public:
        GPAgent(size_t id, const std::string &name)
            : AgentBase(id, name)
        {
        }
        ~GPAgent() = default;

        /// @brief Setup graph.
        /// @return Success.
        bool Initialize() override
        {
            auto graph_builder = std::make_unique<GraphBuilder>(action_map);

            decision_graph = graph_builder->VerticalPacer();

            return true;
        }

        /// Choose the action to take a step in the appropriate direction.
        size_t SelectAction(const cse491::WorldGrid &grid,
                            const cse491::type_options_t &type_options,
                            const cse491::item_set_t &item_set,
                            const cse491::agent_set_t &agent_set) override
        {
            auto inputs = EncodeState(grid, type_options, item_set, agent_set, this);
            size_t action_to_take = decision_graph->MakeDecision(inputs);

            // Update extra state information.
            extra_state["previous_action"] = action_to_take;
            return action_to_take;
        }

        /// @brief Get a map of extra state information
        /// @return Map of extra state information
        const std::unordered_map<std::string, double> GetExtraState() const
        {
            return extra_state;
        }
    };

} // End of namespace cowboys
