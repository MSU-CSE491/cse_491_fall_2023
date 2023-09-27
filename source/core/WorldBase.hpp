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

    /// @brief Determine if this tile is able to be walked on, defaults to every tile is walkable
    /// @param pos The grid position we are checking
    /// @return If an agent should be allowed on this square
    virtual bool is_walkable (cse491::GridPosition /*pos*/) {return true;}

    /// @brief Uses A* to return a  list of grid positions
    /// @param start Starting position for search
    /// @param end Ending position for the search
    /// @return vector of A* path from start to end, empty vector if no path exist
    std::vector<cse491::GridPosition> shortest_path(cse491::GridPosition start, cse491::GridPosition end);
  
    /// @brief Determine if this tile is able to be walked on, defaults to every
    /// tile is walkable
    /// @author @mdkdoc15
    /// @param pos The grid position we are checking
    /// @return If an agent should be allowed on this square
    virtual bool is_walkable(GridPosition /*pos*/) { return true; }

    /// @brief Uses A* to return a  list of grid positions
    /// @author @mdkdoc15
    /// @param start Starting position for search
    /// @param end Ending position for the search
    /// @return vector of A* path from start to end, empty vector if no path
    /// exist
    std::vector<GridPosition> shortest_path(GridPosition start, GridPosition end)
    {
      // TODO remove the use of new and this

      // Generated with the help of chat.openai.com
      const int rows = this->main_grid.GetWidth();
      const int cols = this->main_grid.GetHeight();
      std::vector<GridPosition> path;
      // If the start or end is not valid then return empty list
      if (!(this->main_grid.IsValid(start) && this->main_grid.IsValid(end)))
        return path;

      // Define possible movements (up, down, left, right)
      const int dx[] = {-1, 1, 0, 0};
      const int dy[] = {0, 0, -1, 1};

      // Create a 2D vector to store the cost to reach each cell
      std::vector<std::vector<int>> cost(rows, std::vector<int>(cols, INT_MAX));

      // Create an open list as a priority queue
      std::priority_queue<std::shared_ptr<walle::Node>, std::vector<std::shared_ptr<walle::Node>>, walle::CompareNodes> openList;

      // Create the start and end nodes
      auto startNode = std::make_shared<walle::Node>(start, 0, 0, nullptr);
      auto endNode = std::make_shared<walle::Node>(end, 0, 0, nullptr);

      openList.push(startNode);
      cost[start.GetX()][start.GetY()] = 0;

      while (!openList.empty())
      {
        auto current = openList.top();
        openList.pop();

        if (current->position == endNode->position)
        {

          // Reached the goal, reconstruct the path
          while (current != nullptr)
          {
            path.push_back(current->position);
            current = current->parent;
          }
          break;
        }

        // Explore the neighbors
        for (int i = 0; i < 4; ++i)
        {
          GridPosition newPos(current->position.GetX() + dx[i], current->position.GetY() + dy[i]);
          // Check if the neighbor is within bounds and is a valid move
          if (this->main_grid.IsValid(newPos) && this->is_walkable(newPos))
          {
            int newG = current->g + 1;                                                                                          // Assuming a cost of 1 to move to a neighbor
            int newH = std::abs(newPos.GetX() - endNode->position.GetX()) + std::abs(newPos.GetY() - endNode->position.GetY()); // Manhattan distance

            if (newG + newH < cost[newPos.GetX()][newPos.GetY()])
            {
              auto neighbor = std::make_shared<walle::Node>(newPos, newG, newH, current);
              openList.push(neighbor);
              cost[newPos.GetX()][newPos.GetY()] = newG + newH;
            }
          }
        }
      }

      return path;
    }
  };

} // End of namespace cse491
