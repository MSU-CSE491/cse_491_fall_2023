
#pragma once

#include "source/core/WorldGrid.hpp"

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
         * @brief returns the distance to the wall on the left of the agent
         *
         * @param grid grid maze of the world
         * @param agent agent to find the distance from
         * @return distance to the wall
         */
        static int leftWallDistance(const cse491::WorldGrid& grid, const cse491::AgentBase & agent) {
            int distance = 0;
            cse491::GridPosition position = agent.GetPosition();


            while (grid.At(position) != WALL && grid.IsValid(position)) {
                position = position.ToLeft();
                distance++;
            }


            printPositions("left distance to the wall:  " + std::to_string(distance));
            return distance;
        }

        /**
         * @brief returns the distance to the wall on the right of the agent
         *
         * @param grid grid maze of the world
         * @param agent agent to find the distance from
         * @return distance to the wall
         */
        static int rightWallDistance(const cse491::WorldGrid& grid, const cse491::AgentBase & agent) {
            int distance = 0;
            cse491::GridPosition position = agent.GetPosition();

            while (grid.At(position) != WALL && grid.IsValid(position)) {
                position = position.ToRight();
                distance++;
            }

            printPositions("right distance to the wall:  " + std::to_string(distance));
            return distance;
        }


        /**
         * @brief returns the distance to the wall on the top of the agent
         *
         * @param grid grid maze of the world
         * @param agent agent to find the distance from
         * @return distance to the wall
         */
        static int topWallDistance(const cse491::WorldGrid& grid, const cse491::AgentBase & agent) {
            int distance = 0;
            cse491::GridPosition position = agent.GetPosition();

            while (grid.At(position) != WALL && grid.IsValid(position)) {
                position = position.Above();
                distance++;
            }


            printPositions("top distance to the wall:  " + std::to_string(distance));
            return distance;
        }

        /**
         * @brief returns the distance to the wall on the bottom of the agent
         *
         * @param grid grid maze of the world
         * @param agent agent to find the distance from
         * @return distance to the wall
         */
        static int bottomWallDistance(const cse491::WorldGrid& grid, const cse491::AgentBase & agent) {
            int distance = 0;
            cse491::GridPosition position = agent.GetPosition();

            while (grid.At(position) != WALL && grid.IsValid(position)) {
                position = position.Below();
                distance++;
            }

            printPositions("bottom distance to the wall:  " + std::to_string(distance));
            return distance;
        }

    };
}