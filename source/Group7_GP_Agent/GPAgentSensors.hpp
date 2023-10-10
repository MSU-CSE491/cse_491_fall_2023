
#pragma once

#include "../core/WorldGrid.hpp"

const int WALL = 2; //TODO: work with world teams to better define impenetrable objects

//#define DEBUG

/**
 * @brief yeeeeeeeehaaaaaaaaa
 *
 * @static currently a static class
 *
 * @note TODO: might have to move this over to core of the project
 * @note TODO: might have to refactor to make it one function???
 *
 * @author @amantham20
 * @details currenly supports only wall distance sensors for left, right, top and bottom. this is library of sensors for a given agent return
 */
namespace cowboys {

    enum SensorDirection {
        LEFT,
        RIGHT,
        ABOVE,
        BELOW
    };

    class Sensors {
    private:
        /**
         * @brief print the positions of the agent only during debug mode
         * @param printstring
         */
        [[maybe_unused]] static void printPositions(const std::string& printstring){
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
        static int wallDistance(const cse491::WorldGrid &grid, const cse491::AgentBase &agent, SensorDirection direction) {
            int distance = 0;
            cse491::GridPosition position = agent.GetPosition();

            while ( grid.IsValid(position) && grid.At(position) != WALL) {
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

            printPositions(directionStr + " distance to the wall:  " + std::to_string(distance - 1));
            return distance - 1;
        }

    };
}