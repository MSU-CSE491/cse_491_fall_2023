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

#include "../core/AgentBase.hpp"

namespace cowboys {
  class GPAgent_ : public cse491::AgentBase {
  protected:
    /// A map of extra state information.
    std::unordered_map<std::string, double> extra_state;

  public:
    GPAgent_(size_t id, const std::string &name) : AgentBase(id, name) { extra_state["previous_action"] = 0; }
    ~GPAgent_() = default;

    /// @brief Setup graph.
    /// @return Success.
    bool Initialize() override { return true; }

    /// Choose the action to take a step in the appropriate direction.
    size_t SelectAction(const cse491::WorldGrid &grid, const cse491::type_options_t &type_options,
                        const cse491::item_set_t &item_set, const cse491::agent_set_t &agent_set) override {
      size_t action = GetAction(grid, type_options, item_set, agent_set);

      // Update extra state information.
      extra_state["previous_action"] = action;

      return action;
    }

    virtual size_t GetAction(const cse491::WorldGrid &grid, const cse491::type_options_t &type_options,
                             const cse491::item_set_t &item_set, const cse491::agent_set_t &agent_set) = 0;

    /// @brief Get a map of extra state information
    /// @return Map of extra state information
    const std::unordered_map<std::string, double> GetExtraState() const { return extra_state; }

    /// @brief Mutate this agent.
    /// @param mutation_rate The mutation rate. Between 0 and 1.
    virtual void MutateAgent(double mutation_rate) = 0;

    /// @brief Copy the behavior of another agent into this agent.
    /// @param other The agent to copy. Should be the same type.
    virtual void Copy(const GPAgent_ &other) = 0;
  };

} // End of namespace cowboys
