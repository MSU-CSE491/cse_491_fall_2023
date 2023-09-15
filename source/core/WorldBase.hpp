/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A base class for all World modules.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <vector>

#include "AgentBase.hpp"
#include "Data.hpp"
#include "Entity.hpp"
#include "WorldGrid.hpp"

namespace cse491 {

  class WorldBase {
  protected:
    /// This is the main grid for this world;
    /// derived worlds may choose to have more than one grid.
    WorldGrid main_grid;
    type_options_t type_options; ///< Vector of types of cells in grids for this world.

    entity_set_t entity_set; ///< Vector of pointers to non-agent entities
    agent_set_t agent_set;   ///< Vector of pointers to agent entities

    bool run_over = false;   ///< Should the run end?

    /// Helper function that is run whenever a new agent is created.
    /// @note Override this function to provide agents with actions or other setup.
    virtual void ConfigAgent(AgentBase & /* agent */) { }

    CellType & AddCellType(const std::string & name, const std::string & desc="") {
      type_options.push_back(CellType{name, desc, type_options.size()});
      return type_options.back();
    }

  public:
    WorldBase() {
      // The first cell type (ID 0) should be reserved for errors or empty spots in a grid.
      AddCellType("Unknown", "This is an invalid cell type and should not be reachable.");
    }
    virtual ~WorldBase() = default;

    // -- Accessors --

    /// Get the total number of NON-agent entities
    [[nodiscard]] size_t GetNumEntities() const { return entity_set.size(); }

    /// Get the total number of AGENT entities
    [[nodiscard]] size_t GetNumAgents() const { return agent_set.size(); }

    /// Return a reference to an agent with a given ID.
    [[nodiscard]] Entity & GetEntity(size_t id) {
      assert(id < entity_set.size());
      return *entity_set[id];
    }

    /// Return a reference to an agent with a given ID.
    [[nodiscard]] AgentBase & GetAgent(size_t id) {
      assert(id < agent_set.size());
      return *agent_set[id];
    }


    // -- Agent Management --

    /// @brief Build a new agent of the specified type.
    /// @tparam AGENT_T The type of agent to build.
    /// @param agent_name The name of this agent.
    /// @return A reference to the newly created agent.
    template <typename AGENT_T>
    AgentBase & AddAgent(std::string agent_name="None") {
      auto agent_ptr = std::make_unique<AGENT_T>(agent_set.size(), agent_name);
      ConfigAgent(*agent_ptr);
      agent_ptr->Initialize();
      agent_set.emplace_back(std::move(agent_ptr));
      return *agent_set.back();
    }


    // -- Action Management --

    /// @brief Central function for an agent to take any action
    /// @param agent The specific agent taking the action
    /// @param action The id of the action to take
    /// @return The result of this action (usually 0/1 to indicate success)
    /// @note Thus function must be overridden in any derived world.
    virtual int DoAction(AgentBase & agent, size_t action_id) = 0;

    /// @brief Step through each agent giving them a chance to take an action.
    /// @note Override this function if you want to control which grid the agents receive.
    virtual void RunAgents() {
      for (const auto & agent_ptr : agent_set) {
        size_t action_id =
          agent_ptr->SelectAction(main_grid, type_options, entity_set, agent_set);
        int result = DoAction(*agent_ptr, action_id);
        agent_ptr->SetActionResult(result);
      }
    }

    /// @brief UpdateWorld() is run after every agent has a turn.
    /// Override this function to manage background events for a world.
    virtual void UpdateWorld() { }

    /// @brief Run all agents repeatedly until an end condition is met.
    void Run() {
      run_over = false;
      while (!run_over) {
        RunAgents();
        UpdateWorld();
      }
    }

    // CellType management.

    /// @brief Return the ID associated with the cell type name.
    /// @param name The unique name of the cell type
    /// @return The unique ID of the cell type (or 0 if it doesn't exist.)
    [[nodiscard]] size_t GetCellTypeID(const std::string & name) const { 
      for (const auto & type : type_options) {
        if (type.name == name) return type.id;
      }
      return 0;
    }

    [[nodiscard]] const std::string & GetCellTypeName(size_t id) const {
      if (id >= type_options.size()) return type_options[0].name;
      return type_options[id].name;
    }
  };

} // End of namespace cse491
