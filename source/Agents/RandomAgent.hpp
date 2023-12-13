/**
 * This file is part of the Fall 2023, CSE 491 Course Project
 * @file RandomAgent.hpp
 * @brief An Agent that will move around using random actions
 * @note Status: ALPHA
 * @author Yousif Murrani
 **/

#pragma once

#include <cassert>

#include "../core/AgentBase.hpp"
#include "../core/GridPosition.hpp"
#include "../core/WorldBase.hpp"

namespace walle {

/**
 * Class for the Random Agent
 */
class RandomAgent : public cse491::AgentBase {
private:
  double random_val = 4.0;  /// Random value used to determine direction
  bool moving = true;  /// Is the agent moving?

public:
  /**
   * @brief Construct a new Random Agent object
   * @param id id of the agent
   * @param name name of the agent
   */
  RandomAgent(size_t id, const std::string &name) : AgentBase(id, name) {}

  ~RandomAgent() = default;

  /**
   * @brief This agent needs a specific set of actions to function.
   * @return Success.
   */
  bool Initialize() override
  {
    return HasAction("up") && HasAction("down") && HasAction("left") && HasAction("right");
  }

  /**
   * @brief Choose the action to take a step in the random direction
   */
  size_t SelectAction(const cse491::WorldGrid & /*grid*/,
                      const cse491::type_options_t & /* type_options*/,
                      const cse491::item_map_t & /* item_map*/,
                      const cse491::agent_map_t & /* agent_map*/) override
  {
    // We are taking an action so another turn has passed

    CalculateRandom(random_val);

    if (moving) {
      if (random_val < 1.0) {
        return action_map["up"];
      } else if (random_val < 2.0) {
        return action_map["down"];
      } else if (random_val < 3.0) {
        return action_map["left"];
      } else {
        return action_map["right"];
      }
    }

    return 0;  // should not reach this point
  }

  /**
   * @brief Function to calculate the random direction
   * @param multiplier double: random multiplier
   */
  void CalculateRandom(double multiplier) { random_val = GetWorld().GetRandom(multiplier); }

  /**
   * @brief Set the Direction object
   * @param direction direction to set
   */
  void SetDirection(double direction) { random_val = direction; }

  /**
   * @brief Set the Moving object
   * @param move move to set
   */
  void SetMoving(bool move) { moving = move; }

  /**
   * @brief Get the Random object
   * @return double random member variable
   */
  double GetRandom() const { return random_val; }

  /**
   * @brief Get the Moving object
   * @return true
   * @return false
   */
  bool GetMoving() const { return moving; }
};
}  // namespace walle