/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A base class interface for all agent types.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <cassert>
#include <string>
#include <unordered_map>
#include <utility>

#include "Data.hpp"
#include "Entity.hpp"
#include "GridPosition.hpp"
#include "WorldGrid.hpp"
#include "../DataCollection/AgentData.hpp"

namespace cse491 {

  enum State /// States that an agent can be in.
  {
    Healthy,
    Taking_Damage,
    Dying,
    Deceased
  };

  class AgentBase : public Entity {
  protected:
    size_t grid_id=0;       ///< Which grid is this agent on?

    /// A map of names to IDs for each available action
    std::unordered_map<std::string, size_t> action_map;
    int action_result=0;  ///< Usually a one (success) or zero (failure).

    int action; // The action that the agent is currently performing

    State agent_state = Healthy;  /// Default value upon initialization

  public:
    AgentBase(size_t id, const std::string & name) : Entity(id, name) {}
    ~AgentBase() = default; // Already virtual from Entity

    [[nodiscard]] size_t GetGridID() const { return grid_id; }
    [[nodiscard]] bool IsOnGrid(size_t in_grid_id) const { return grid_id == in_grid_id; }

    // -- World Interactions --

    /// @brief Run AFTER the world configures the agent, for additional tests or setup.
    /// @return Was the initialization successful?
    virtual bool Initialize() { return true; }

    // -- Entity Overrides --

    bool IsAgent() const override { return true; }

    // -- Agent Interaction --

    /// @brief Get the current state of the Agent
    /// @return State enum of agent_state
    State GetAgentState() { return agent_state; }

    /// @brief Updates an Agent's state depending on health
    /// @param agent The agent whose state to update
    /// If the agent's health is between Max_health and 3 -> Healthy
    /// If the agent's health is between 0 and 3 -> Dying
    /// If the agent is Taking_Damage -> Taking_Damage
    /// If the agent is at 0 or below -> Dead
    /// @see TakeDamage
    /// @return None
    void UpdateAgentState(cse491::AgentBase & agent) {
      if(agent.HasProperty("Health")){
        if(agent.GetProperty<int>("Health") <= agent.GetProperty<int>("Max_Health")
        && agent.GetProperty<int>("Health") > 3){
          agent.agent_state = Healthy;
        }
        else if(agent.GetProperty<int>("Health") <= 3 && agent.GetProperty<int>("Health") > 0){
          agent.agent_state = Dying;
        }
        else if(agent.GetProperty<int>("Health") <= 0){
          agent.agent_state = Deceased;
        }
      }
      if(agent.HasProperty("Taking_Damage")){
        if(agent.GetProperty<bool>("Taking_Damage") == true){
          agent.agent_state = Taking_Damage;
        }
      }
    }

    /// @brief If the agent is in State::Taking_Damage, decrease the health
    /// by the damage factor once per timestep.
    void TakeDamage(cse491::AgentBase & agent){
        agent.SetProperty<int>("Health", agent.GetProperty<int>("Health") -
        agent.GetProperty<int>("Taking_Damage"));
        UpdateAgentState(agent);
    }

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

    void storeActionMap(std::string name) {
        DataCollection::AgentData data(name);
        data.StoreAction(action_map);
    }

    /// Allow derived agents to provide an arbitrary next position for the world to move the agent to
    [[nodiscard]] virtual GridPosition GetNextPosition() { return Entity::GetPosition(); }

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
        [[maybe_unused]] const item_map_t & item_map,
        [[maybe_unused]] const agent_map_t & agent_map
      )
    { return 0; }

    /// Retrieve the result of the most recent action.
    [[nodiscard]] int GetActionResult() const { return action_result; }

    /// Update the result from the most recent action.
    void SetActionResult(int result) { action_result = result; }

    /// @brief Send a notification to this agent, typically from the world.
    /// @param message Contents of the notification
    /// @param msg_type Category of message, such as "item_alert", "damage", or "enemy"
    /// This function is useful to notify users of events in the world, such as them taking
    /// damage, finding an item, etc.  The message type can potentially also provide
    /// information to an autonomous agent assuming we come up with a standard list of types.
    virtual void Notify(const std::string & /*message*/,const std::string & /*msg_type*/="none") { }

    /**
     * Serialize agent (assume no properties)
     * @param os ostream
     */
    void Serialize(std::ostream &os) override {
      os << name << '\n';
      os << id << '\n';
      os << position.GetX() << '\n';
      os << position.GetY() << '\n';
      if (HasProperty("symbol")) os << GetProperty<char>("symbol") << '\n';
      else os << '*' << std::endl;
    }

  };

} // End of namespace cse491
