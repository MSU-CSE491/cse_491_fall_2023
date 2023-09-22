/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A World that consists only of walls and open cells.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <cassert>

#include "../core/WorldBase.hpp"

namespace cse491_team8 {

  class NewWorld : public cse491::WorldBase {
  protected:
    enum ActionType { REMAIN_STILL=0, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, ATTACK };

    size_t floor_id; ///< Easy access to floor CellType ID.
    size_t tree_id;  ///< Easy access to tree CellType ID.
    size_t water_id;  ///< Easy access to water CellType ID.

    /// Provide the agent with movement actions.
    void ConfigAgent(cse491::AgentBase & agent) override {
      agent.AddAction("up", MOVE_UP);
      agent.AddAction("down", MOVE_DOWN);
      agent.AddAction("left", MOVE_LEFT);
      agent.AddAction("right", MOVE_RIGHT);
      agent.AddAction("attack", ATTACK);
    }

  public:
    NewWorld() {
      floor_id = AddCellType("floor", "Floor that you can easily walk over.", ' ');
      tree_id = AddCellType("tree", "Tree that you cannot pass without an axe.", '^');
      water_id = AddCellType("water", "Water that cannot be crossed without a boat.", '~');
      main_grid.Read("../assets/grids/new_grid.grid", type_options);
    }
    ~NewWorld() = default;

   
    inline bool CheckAround(cse491::GridPosition new_position)
    {
        for (auto& other_agent : agent_set)
        {
            if (other_agent->GetPosition() == new_position.Above() ||
                other_agent->GetPosition() == new_position.Below() ||
                other_agent->GetPosition() == new_position.ToLeft() ||
                other_agent->GetPosition() == new_position.ToRight())
            {
                std::cout << "someone is next to you lol" << std::endl;
                return true;
            }
        }
        return false;
    }

    void HandleNeighbors()
    {
        for (const auto& agent : agent_set)
        {
            std::cout << agent->GetPosition().GetX() << " " << agent->GetPosition().GetY() << "\n";
            if (agent->IsInterface())
            {
                CheckAround(agent->GetPosition());
                break;
            }
        }
        std::cout << std::endl;

    }

    void Run() override
    {
        run_over = false;
        while (!run_over) {
            RunAgents();
            UpdateWorld();
            HandleNeighbors();
        }
    }

    /// Allow the agents to move around the maze.
    int DoAction(cse491::AgentBase & agent, size_t action_id) override {
      // Determine where the agent is trying to move.
      cse491::GridPosition new_position;

      switch (action_id) {
      case REMAIN_STILL:
      {
          new_position = agent.GetPosition();
          break;
      }
      case MOVE_UP:      
      {
        new_position = agent.GetPosition().Above();
        break;
      }
      case MOVE_DOWN:    
      {
        new_position = agent.GetPosition().Below();
        break;
      }
      case MOVE_LEFT:    
      {
        new_position = agent.GetPosition().ToLeft();
        break;
      }
      case MOVE_RIGHT:   
      {
        new_position = agent.GetPosition().ToRight();
        break;
      }
      }

      // Don't let the agent move off the world or into a wall.
      if (!main_grid.IsValid(new_position)) { return false; }
      if (main_grid.At(new_position) == tree_id) { return false; }
      if (main_grid.At(new_position) == water_id) { return false; }

      // Set the agent to its new postion.
      agent.SetPosition(new_position);

      return true;
    }

  };

} // End of namespace cse491
