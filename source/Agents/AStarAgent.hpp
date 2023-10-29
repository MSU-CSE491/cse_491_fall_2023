/**
 * @file AStarAgent.h
 * @author Matt Kight
 */
#pragma once

#include <vector>
#include "../core/AgentBase.hpp"
#include "AgentLibary.hpp"

namespace walle {
/**
 * Class that describes a AStarAgent class
 */
class AStarAgent : public cse491::AgentBase {
 private:
  std::vector<cse491::GridPosition> path;       ///< Path this agent is taking
  cse491::GridPosition goal_position;           ///< Where the agent wants to end up
  int recalculate_after_x_turns = 100;          ///< How often agent recalculates moves
  int current_move_num = 0;                     ///< What move # we are currently on

 public:
  AStarAgent(size_t id, const std::string &name) : AgentBase(id, name) {
  }

  ~AStarAgent() = default;

  /// @brief This agent needs a specific set of actions to function.
  /// @return Success.
  bool Initialize() override {
    return HasAction("up") && HasAction("down") && HasAction("left") && HasAction("right");
  }

  /**
   * @brief Set where the agent should head towards
   * @param x x-coordinate of the goal position
   * @param y y-coordinate of the goal position
   */
  void SetGoalPosition(const double x, const double y) {
    goal_position = cse491::GridPosition(x, y);
  }

  /**
   * @brief Set where the agent should head towards
   * @param gp position agent should go towards
   */
  void SetGoalPosition(const cse491::GridPosition gp) {
    goal_position = gp;
  }
  /**
   * @brief Set how many moves should occur before recalculating path
   *  A lower number will react faster to updates in the world but will call A* search more often
   * @param recalculate How often path should be recalculated
   */
  void SetRecalculate(const int recalculate) {
    recalculate_after_x_turns = recalculate;
  }

  /**
   * @brief Update the path to go to goal position
   */
  void RecalculatePath() {
    path = GetShortestPath(GetPosition(), goal_position, GetWorld(), *this);
    current_move_num = 0;
  }
  /// Choose the action to take a step in the appropriate direction.
  size_t SelectAction(const cse491::WorldGrid & /*grid*/,
                      const cse491::type_options_t & /* type_options*/,
                      const cse491::item_set_t & /* item_set*/,
                      const cse491::agent_set_t & /* agent_set*/) override {
    // We are taking an action so another turn has passed
    ++current_move_num;
    // If the last step failed, or we need a new path the then regenerate the path
    if (action_result == 0 || path.empty() || current_move_num > recalculate_after_x_turns) {
      RecalculatePath();
    }
    // Return whatever action gets us closer to our goal
    if (!path.empty()) {
      auto pos = path.back();
      path.pop_back();
      if (pos == position.Above())
        return action_map["up"];
      if (pos == position.Below())
        return action_map["down"];
      if (pos == position.ToLeft())
        return action_map["left"];
      if (pos == position.ToRight())
        return action_map["right"];
    }
    return 0; // If no path then do not do anything
  }
};
};

