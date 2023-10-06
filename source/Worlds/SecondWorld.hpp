/**
 * @file SecondWorld.hpp
 * @author Jayson Van Dam
 * @author Satvik Ravipati
 */

#pragma once

#include "MazeWorld.hpp"
#include "../core/WorldBase.hpp"
#include <fstream>

namespace group4 {
/**
 * Creates a world with agents and a win flag
 */
class SecondWorld : public cse491::MazeWorld {
 private:
  bool flag_hit = false;
  bool run_over = false;

 protected:
  /// Easy access to flag CellType ID.
  size_t flag_id;

 public:
  /**
   * Constructor with no arguments
   */
  SecondWorld() {
    // Call parent class' constructor
    cse491::MazeWorld();

    flag_id = cse491::WorldBase::AddCellType(
        "flag", "Goal flag for a game end state", 'g');
    main_grid.Read("../assets/grids/group4_maze.grid", type_options);
  }

  /**
   * Destructor
   */
  ~SecondWorld() = default;

  void UpdateWorld() override {
    if (flag_hit) {
      run_over = true;
    }
  }

  virtual void Run() override {
    while (!run_over) {
      RunAgents();
      UpdateWorld();
    }

    std::string filename = "output.json";

    std::ofstream ofs(filename);

    ofs << "[" << std::endl;
    size_t index = 0;

    for (const auto& other_agent : agent_set) {
      auto new_position = other_agent->GetPosition();

      std::string agent_name = other_agent->GetName();

      double x_pos = new_position.GetX();
      double y_pos = new_position.GetY();

      ofs << "  {" << std::endl;
      ofs << "    \"name\": "
          << "\"" << agent_name << "\""
          << ",\n";

      ofs << "    \"x\": " << x_pos << "," << std::endl;
      ofs << "    \"y\": " << y_pos << std::endl;

      if (index == agent_set.size() - 1) {
        ofs << "  }" << std::endl;
      } else {
        ofs << "  }," << std::endl;
      }

      index++;
    }
    ofs << "]" << std::endl;
  }

  /**
   * Allows agents to move around the maze.
   * Also checks if the next agent position will be the win flag.
   */
  int DoAction(cse491::AgentBase& agent, size_t action_id) override {
    cse491::MazeWorld::DoAction(agent, action_id);

    cse491::GridPosition new_position;
    switch (action_id) {
      case REMAIN_STILL:
        new_position = agent.GetPosition();
        break;
      case MOVE_UP:
        new_position = agent.GetPosition().Above();
        break;
      case MOVE_DOWN:
        new_position = agent.GetPosition().Below();
        break;
      case MOVE_LEFT:
        new_position = agent.GetPosition().ToLeft();
        break;
      case MOVE_RIGHT:
        new_position = agent.GetPosition().ToRight();
        break;
    }
    if (!main_grid.IsValid(new_position)) {
      return false;
    }
    if (main_grid.At(new_position) == wall_id) {
      return false;
    }

    if (main_grid.At(new_position) == flag_id) {
      // Set win flag to true
      std::cout << "flag found" << std::endl;
      flag_hit = true;
    }

    return true;
  }
};
}