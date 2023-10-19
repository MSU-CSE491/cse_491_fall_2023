/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A base class interface for all agent types.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <cassert>
#include <string>
#include <unordered_map>

#include "Data.hpp"
#include "Entity.hpp"
#include "GridPosition.hpp"
#include "WorldGrid.hpp"

namespace cse491 {

  class AgentBase : public Entity {
  protected:
    /// A map of names to IDs for each available action
    std::unordered_map<std::string, size_t> action_map;

    int action_result=0;  ///< Usually a one (success) or zero (failure).

  public:
    AgentBase(size_t id, const std::string & name) : Entity(id, name) { }
    ~AgentBase() = default; // Already virtual from Entity

    // -- World Interactions --

    /// @brief Run AFTER the world configures the agent, for additional tests or setup.
    /// @return Was the initialization successful?
    virtual bool Initialize() { return true; }

    // -- Entity Overrides --

    bool IsAgent() const override { return true; }


    // -- Action management --

    /// Test if agent already has a specified action.
    [[nodiscard]] bool HasAction(const std::string & action_name) const {
      return action_map.count(action_name);
    }

    /// Return an action ID *if* that action exists, otherwise return zero.
    [[nodiscard]] size_t GetActionID(const std::string & action_name) const {
      auto it = action_map.find(action_name);
      if (it == action_map.end()) return 0;
      return it->second;
    }

    /// Provide a new action that this agent can take.
    virtual AgentBase & AddAction(const std::string & action_name, size_t action_id) {
      assert(!HasAction(action_name)); // Cannot add existing action name.
      action_map[action_name] = action_id;
      return *this;
    }

    /// @brief Decide the next action for this agent to perform; should be overridden!
    /// @param grid The agent is provided with the current WorldGrid
    /// @return ID associated with the action to perform; (zero is always "no action")
    [[nodiscard]] virtual size_t SelectAction(
        [[maybe_unused]] const WorldGrid & grid,
        [[maybe_unused]] const type_options_t & type_options,
        [[maybe_unused]] const item_set_t & item_set,
        [[maybe_unused]] const agent_set_t & agent_set
      )
    { return 0; }

    /// Retrieve the result of the most recent action.
    [[nodiscard]] int GetActionResult() const { return action_result; }

    /// Update the result from the most recent action.
    void SetActionResult(int result) { action_result = result; }

  };

} // End of namespace cse491
