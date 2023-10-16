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
#include <algorithm>

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

    item_set_t item_set;     ///< Vector of pointers to non-agent entities
    agent_set_t agent_set;   ///< Vector of pointers to agent entities

    bool run_over = false;   ///< Should the run end?

    /// Helper function that is run whenever a new agent is created.
    /// @note Override this function to provide agents with actions or other setup.
    virtual void ConfigAgent(AgentBase & /* agent */) { }

    /// @brief  Add a new type of cell to this world.
    /// @param name A unique name for this cell type
    /// @param desc A longer description of the cell type
    /// @param symbol An (optional) unique symbol for text IO (files, command line)
    /// @return A unique ID associated with this cell type (position in type_options vector)
    size_t AddCellType(const std::string & name, const std::string & desc="",
                           char symbol='\0') {
      type_options.push_back(CellType{name, desc, symbol});
      return type_options.size() - 1;
    }

  public:
    WorldBase() {
      // The first cell type (ID 0) should be reserved for errors or empty spots in a grid.
      AddCellType("Unknown", "This is an invalid cell type and should not be reachable.");
    }
    virtual ~WorldBase() = default;

    // -- Accessors --

    /// Get the total number of NON-agent entities
    [[nodiscard]] size_t GetNumItems() const { return item_set.size(); }

    /// Get the total number of AGENT entities
    [[nodiscard]] size_t GetNumAgents() const { return agent_set.size(); }

    /// Return a reference to an agent with a given ID.
    [[nodiscard]] Entity & GetItem(size_t id) {
      assert(id < item_set.size());
      return *item_set[id];
    }

    /// Return a reference to an agent with a given ID.
    [[nodiscard]] AgentBase & GetAgent(size_t id) {
      assert(id < agent_set.size());
      return *agent_set[id];
    }

    /// Return an editable version of the current grid for this world (main_grid by default) 
    virtual WorldGrid & GetGrid() { return main_grid; }

    /// Return the current grid for this world (main_grid by default) 
    virtual const WorldGrid & GetGrid() const { return main_grid; }

    /// Determine if the run has ended.
    virtual bool GetRunOver() const { return run_over; }

    // -- Agent Management --

    /// @brief Build a new agent of the specified type
    /// @tparam AGENT_T The type of agent to build
    /// @tparam PROPERTY_Ts Types for any properties to set at creation (automatic)
    /// @param agent_name The name of this agent
    /// @param properties Name/value pairs for any properties set at creation
    /// @return A reference to the newly created agent
    template <typename AGENT_T, typename... PROPERTY_Ts>
    AgentBase & AddAgent(std::string agent_name="None", PROPERTY_Ts... properties) {
      auto agent_ptr = std::make_unique<AGENT_T>(agent_set.size(), agent_name);
      agent_ptr->SetProperties(std::forward<PROPERTY_Ts>(properties)...);
      ConfigAgent(*agent_ptr);
      if (agent_ptr->Initialize() == false) {
        std::cerr << "Failed to initialize agent '" << agent_name << "'." << std::endl;
      }
      agent_set.emplace_back(std::move(agent_ptr));
      return *agent_set.back();
    }

    /// @brief Build a new item
    /// @tparam PROPERTY_Ts Types for any properties to set at creation (automatic)
    /// @param entity_name The name of this item
    /// @param properties Name/value pairs for any properties set at creation
    /// @return A reference to the newly created entity
    template <typename... PROPERTY_Ts>
    Entity & AddItem(std::string entity_name="None", PROPERTY_Ts... properties) {
        auto entity_ptr = std::make_unique<Entity>(item_set.size(), entity_name);
        entity_ptr->SetProperties(std::forward<PROPERTY_Ts>(properties)...);
        item_set.emplace_back(std::move(entity_ptr));
        return *item_set.back();
    }

    /// @brief Remove an agent from the agent set
    /// @param agent_name The name of this agent
    /// @return None
    void RemoveAgent(std::string agent_name="None") {
        if (agent_name == "Interface")
        {
            return;
        }
        agent_set_t ::iterator agent_pointer =
                std::find_if(agent_set.begin(), agent_set.end(),
                        [&](std::unique_ptr<AgentBase> & agent){ return agent->GetName() == agent_name;}
                );
        agent_set.erase(std::remove(agent_set.begin(), agent_set.end(), *agent_pointer));
    }

    /// @brief Remove an item from the item set
    /// @param entity_name The name of this entity
    /// @return None
    void RemoveItem(std::string entity_name = "None") {
        item_set_t ::iterator entity_pointer =
            std::find_if(item_set.begin(), item_set.end(),
                [&](std::unique_ptr<Entity>& entity) { return entity->GetName() == entity_name; }
        );
        item_set.erase(std::remove(item_set.begin(), item_set.end(), *entity_pointer));
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
          agent_ptr->SelectAction(main_grid, type_options, item_set, agent_set);
        int result = DoAction(*agent_ptr, action_id);
        agent_ptr->SetActionResult(result);
      }
    }

    /// @brief UpdateWorld() is run after every agent has a turn.
    /// Override this function to manage background events for a world.
    virtual void UpdateWorld() { }

    /// @brief Run all agents repeatedly until an end condition is met.
    virtual void Run() {
      run_over = false;
      while (!run_over) {
        RunAgents();
        UpdateWorld();
      }
    }

    // CellType management.

    // Return a const vector of all of the possible cell types.
    [[nodiscard]] const type_options_t & GetCellTypes() const { return type_options; }


    /// @brief Return the ID associated with the cell type name.
    /// @param name The unique name of the cell type
    /// @return The unique ID of the cell type (or 0 if it doesn't exist.)
    [[nodiscard]] size_t GetCellTypeID(const std::string & name) const { 
      for (size_t i=1; i < type_options.size(); ++i) {
        if (type_options[i].name == name) return i;
      }
      return 0;
    }

    [[nodiscard]] const std::string & GetCellTypeName(size_t id) const {
      if (id >= type_options.size()) return type_options[0].name;
      return type_options[id].name;
    }

    [[nodiscard]] char GetCellTypeSymbol(size_t id) const {
      if (id >= type_options.size()) return type_options[0].symbol;
      return type_options[id].symbol;
    }
  };

} // End of namespace cse491
