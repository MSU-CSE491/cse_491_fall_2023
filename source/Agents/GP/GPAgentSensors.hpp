
#pragma once

#include "../../core/WorldGrid.hpp"

// #define DEBUG

/**
 * @brief yeeeeeeeehaaaaaaaaa
 *
 * @static currently a static class
 *
 * @note TODO: might have to move this over to core of the project
 * @note TODO: might have to refactor to make it one function???
 *
 * @author @amantham20
 * @details currenly supports only wall distance sensors for left, right, top
 * and bottom. this is library of sensors for a given agent return
 */
namespace cowboys {

static constexpr int WALL =
    2;  // TODO: work with world teams to better define impenetrable objects
// FIXME: Group 1 has made a class that can check if a position is penetrable
// or not. We This will be used instead

enum SensorDirection { LEFT, RIGHT, ABOVE, BELOW };

class Sensors {
 private:
  /**
   * @brief print the positions of the agent only during debug mode
   * @param printstring
   */
  [[maybe_unused]] static void debugPosition(const std::string &printstring) {
#ifndef NDEBUG
    std::cout << printstring << std::endl;
#endif
  }

 public:
  /// Constructor
  Sensors() = default;

  /// Destructor
  ~Sensors() = default;

  /**
   * @brief returns the distance to the wall in a given direction from the agent
   *
   * @param grid grid maze of the world
   * @param agent agent to find the distance from
   * @param direction direction to find the distance (LEFT, RIGHT, ABOVE, BELOW)
   * @return distance to the wall
   */
  static int wallDistance(const cse491::WorldGrid &grid,
                          const cse491::AgentBase &agent,
                          SensorDirection direction) {
    int distance = 0;
    cse491::GridPosition position = agent.GetPosition();

    while (grid.IsValid(position) && grid.At(position) != WALL) {
      if (direction == LEFT) {
        position = position.ToLeft();
      } else if (direction == RIGHT) {
        position = position.ToRight();
      } else if (direction == ABOVE) {
        position = position.Above();
      } else if (direction == BELOW) {
        position = position.Below();
      }
      distance++;
    }

    std::string directionStr;
    if (direction == LEFT) {
      directionStr = "left";
    } else if (direction == RIGHT) {
      directionStr = "right";
    } else if (direction == ABOVE) {
      directionStr = "top";
    } else if (direction == BELOW) {
      directionStr = "bottom";
    }

    debugPosition(directionStr +
                  " distance to the wall:  " + std::to_string(distance - 1));

    return distance - 1;
  }

  // make a function that takes in a string and maps the SensorDirectionEnum
  static SensorDirection getSensorDirectionEnum(const std::string &direction) {
    if (direction == "getLeft") {
      return SensorDirection::LEFT;
    } else if (direction == "getRight") {
      return SensorDirection::RIGHT;
    } else if (direction == "getUp") {
      return SensorDirection::ABOVE;
    } else if (direction == "getDown") {
      return SensorDirection::BELOW;
    }

    throw std::invalid_argument("Invalid direction");
    return SensorDirection::LEFT;
  }
};
}  // namespace cowboys
