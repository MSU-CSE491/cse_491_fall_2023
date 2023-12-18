/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A World that consists only of walls and open cells.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <cassert>

#include "../Agents/PathAgent.hpp"
#include "../core/WorldBase.hpp"

namespace cse491 {

class MazeWorld : public WorldBase {
 protected:
  enum ActionType {
    REMAIN_STILL = 0,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_ARBITRARY
  };

  size_t floor_id; ///< Easy access to floor CellType ID.
  size_t wall_id;  ///< Easy access to wall CellType ID.

 public:
  MazeWorld(unsigned int seed = 0) : WorldBase(seed) {
    // Create cell types

    floor_id =
        AddCellType("floor", "Floor that you can easily walk over.", ' ');
    wall_id = AddCellType(
        "wall", "Impenetrable wall that you must find a way around.", '#');
    // Set cell type properties
    type_options.at(floor_id).SetProperty(CellType::CELL_WALL);
    // Load map
    main_grid.Read("../assets/grids/default_maze.grid", type_options);
  }
  ~MazeWorld() = default;
  
  using WorldBase::ConfigAgent;
  void ConfigAgent(AgentBase &agent) const override {
    agent.AddAction("up", MOVE_UP);
    agent.AddAction("down", MOVE_DOWN);
    agent.AddAction("left", MOVE_LEFT);
    agent.AddAction("right", MOVE_RIGHT);
    agent.AddAction("move_arbitrary", MOVE_ARBITRARY);
  }

  void ConfigAgent(AgentBase &agent) override {
    agent.AddAction("up", MOVE_UP);
    agent.AddAction("down", MOVE_DOWN);
    agent.AddAction("left", MOVE_LEFT);
    agent.AddAction("right", MOVE_RIGHT);
    agent.AddAction("move_arbitrary", MOVE_ARBITRARY);
  }

  /// Allow the agents to move around the maze.
  int DoAction(AgentBase &agent, size_t action_id) override {
      // Determine where the agent is trying to move.
      GridPosition new_position;
      switch (action_id) {
          case REMAIN_STILL:new_position = agent.GetPosition();
              break;
          case MOVE_UP:new_position = agent.GetPosition().Above();
              break;
          case MOVE_DOWN:new_position = agent.GetPosition().Below();
              break;
          case MOVE_LEFT:new_position = agent.GetPosition().ToLeft();
              break;
          case MOVE_RIGHT:new_position = agent.GetPosition().ToRight();
              break;
          case MOVE_ARBITRARY:new_position = agent.GetNextPosition();
              break;
      }

    // Don't let the agent move off the world or into a wall.
    if (!main_grid.IsValid(new_position)) { return false; }
    if (!IsTraversable(agent, new_position)) { return false; }

      // Set the agent to its new position.
      agent.SetPosition(new_position);
      return true;
  }


  [[nodiscard]] bool IsTraversable(const AgentBase & /*agent*/, cse491::GridPosition pos) const override {
    //return !GetCellTypes().at(main_grid.At(pos)).HasProperty(CellType::CELL_WALL);
    // ^ This doesn't work because we're not assigning any properties to the cell types, so a band-aid solution is to use name
    return !(GetCellTypes().at(main_grid.At(pos)).name == CellType::CELL_WALL);
  }
};

} // End of namespace cse491
