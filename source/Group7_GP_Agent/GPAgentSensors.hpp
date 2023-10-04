
#pragma once

#include "source/core/WorldGrid.hpp"

const int WALL = 2;

//#define DEBUG

/**
 * @brief yeeeeeeeehaaaaaaaaa
 * @static this is library of sensors for a given agent return
 *
 */
namespace cowboys {

    class Sensors {
    private:
        static void printPositions(std::string printstring){
            #ifdef DEBUG
            std::cout << printstring << std::endl;
            #endif
        }

    public:
        Sensors() = default;
        ~Sensors() = default;

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