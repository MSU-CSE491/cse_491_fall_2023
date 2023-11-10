/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A World that consists only of walls and open cells.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <cassert>

#include "../core/WorldBase.hpp"

namespace cse491 {

class MazeWorld : public WorldBase {
 protected:
  enum ActionType { REMAIN_STILL = 0, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };

  size_t floor_id; ///< Easy access to floor CellType ID.
  size_t wall_id;  ///< Easy access to wall CellType ID.

  /// Provide the agent with movement actions.
  void ConfigAgent(AgentBase &agent) override {
    agent.AddAction("up", MOVE_UP);
    agent.AddAction("down", MOVE_DOWN);
    agent.AddAction("left", MOVE_LEFT);
    agent.AddAction("right", MOVE_RIGHT);
  }

 public:
  MazeWorld() {
    floor_id = AddCellType("floor", "Floor that you can easily walk over.", ' ');
    wall_id = AddCellType("wall", "Impenetrable wall that you must find a way around.", '#');
    main_grid.Read("../assets/grids/default_maze.grid", type_options);
  }
  ~MazeWorld() = default;

  /// Allow the agents to move around the maze.
  int DoAction(AgentBase &agent, size_t action_id) override {
    // Determine where the agent is trying to move.
    GridPosition new_position;
    switch (action_id) {
      case REMAIN_STILL: new_position = agent.GetPosition();
        break;
      case MOVE_UP: new_position = agent.GetPosition().Above();
        break;
      case MOVE_DOWN: new_position = agent.GetPosition().Below();
        break;
      case MOVE_LEFT: new_position = agent.GetPosition().ToLeft();
        break;
      case MOVE_RIGHT: new_position = agent.GetPosition().ToRight();
        break;
    }

    // Don't let the agent move off the world or into a wall.
    if (!main_grid.IsValid(new_position)) { return false; }
    if (main_grid.At(new_position) == wall_id) { return false; }

    // Set the agent to its new postion.
    agent.SetPosition(new_position);
    return true;
  }

  /// Can walk on all tiles except for walls
  bool IsTraversable(const AgentBase & /*agent*/, cse491::GridPosition pos) const override {
    return main_grid.At(pos) != wall_id;
  }
};

} // End of namespace cse491
