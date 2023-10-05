/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A World that consists only of walls and open cells.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <cassert>
#include <random>

#include "../core/WorldBase.hpp"

namespace cse491 {

  class MazeWorld : public WorldBase {
  protected:
    enum ActionType { REMAIN_STILL=0, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, REMOVE_TAR };

    size_t floor_id; ///< Easy access to floor CellType ID.
    size_t wall_id;  ///< Easy access to wall CellType ID.

    size_t spike_id;  ///< Easy access to spike CellType ID.
    size_t tar_id; ///< Easy access to tar CellTypeID

    size_t grass_id;
    size_t dirt_id;

    unsigned int seed; ///< Seed used for generator
    std::mt19937 randomGenerator;


    /// Provide the agent with movement actions.
    void ConfigAgent(AgentBase & agent) override {
      agent.AddAction("up", MOVE_UP);
      agent.AddAction("down", MOVE_DOWN);
      agent.AddAction("left", MOVE_LEFT);
      agent.AddAction("right", MOVE_RIGHT);
      agent.AddAction("tar", REMOVE_TAR);
    }

  public:
    /**
     * Initializes world with cell types and random generator
     * @param seed Seed used for RNG. Default is current time
     */
    explicit MazeWorld(unsigned int seed = time(nullptr)) : seed(seed) {
      floor_id = AddCellType("floor", "Floor that you can easily walk over.", ' ');
      wall_id = AddCellType("wall", "Impenetrable wall that you must find a way around.", '#');

      spike_id = AddCellType("spike", "Dangerous spike that resets the game.", 'X');

      tar_id = AddCellType("tar", "Slow tile that makes you take two steps to get through it", 'O');

      grass_id = AddCellType("grass", "Grass you can walk on.", 'M');
      dirt_id  = AddCellType("dirt", "Dirt you can walk on.", '~');

      main_grid.Read("../assets/grids/default_maze.grid", type_options);

      randomGenerator.seed(seed);
    }
    ~MazeWorld() = default;

    /// Allow the agents to move around the maze.
    int DoAction(AgentBase & agent, size_t action_id) override {
      // Determine where the agent is trying to move.
      GridPosition new_position;
      switch (action_id) {
      case REMAIN_STILL: new_position = agent.GetPosition(); break;
      case MOVE_UP:      new_position = agent.GetPosition().Above(); break;
      case MOVE_DOWN:    new_position = agent.GetPosition().Below(); break;
      case MOVE_LEFT:    new_position = agent.GetPosition().ToLeft(); break;
      case MOVE_RIGHT:   new_position = agent.GetPosition().ToRight(); break;
      case REMOVE_TAR:   new_position = agent.GetPosition(); break;
      }

      // Don't let the agent move off the world or into a wall.
      if (!main_grid.IsValid(new_position)) { return false; }
      if (main_grid.At(new_position) == wall_id) { return false; }

      // Check if the agent is trying to move onto a spike.
      if (main_grid.At(new_position) == spike_id) {

      std::cout << "Game over, try again!" << std::endl;
      exit(0);  // Halting the program

      return false;
      }


      // Set the agent to its new postion.
      agent.SetPosition(new_position);

      return true;
    }

  };

} // End of namespace cse491
