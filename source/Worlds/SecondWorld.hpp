/**
 * @file SecondWorld.hpp
 * @author Jayson Van Dam
 */

#pragma once

#include "MazeWorld.hpp"

namespace group4 {
    /**
     * Creates a world with agents and a win flag
     */
    class SecondWorld : public cse491::MazeWorld {
    protected:
        enum class ActionType {
            REMAIN_STILL = 0,
            MOVE_UP,
            MOVE_DOWN,
            MOVE_LEFT,
            MOVE_RIGHT,
            TOUCH_GOAL
        };

        /// Easy access to flag CellType ID.
        size_t flag_id;

    public:
        /**
         * Constructor
         */
        SecondWorld() {
            // Call parent class' constructor
            cse491::MazeWorld();

            flag_id = cse491::WorldBase::AddCellType("flag", "Goal flag for a game end state", 'g');
            main_grid.Read("../assets/grids/group4_maze.grid", type_options);
        }

        /**
         * Desructor
         */
        ~SecondWorld() = default;

        /**
         * Allows agents to move around the maze.
         * Also checks if the next agent position will be the win flag.
         */
        int DoAction(cse491::AgentBase & agent, size_t action_id) override {
            cse491::MazeWorld::DoAction(agent, action_id);

            cse491::GridPosition new_position;
            switch (action_id) {
            case REMAIN_STILL: new_position = agent.GetPosition(); break;
            case MOVE_UP:      new_position = agent.GetPosition().Above(); break;
            case MOVE_DOWN:    new_position = agent.GetPosition().Below(); break;
            case MOVE_LEFT:    new_position = agent.GetPosition().ToLeft(); break;
            case MOVE_RIGHT:   new_position = agent.GetPosition().ToRight(); break;
            }
            if (!main_grid.IsValid(new_position)) { return false; }
            if (main_grid.At(new_position) == wall_id) { return false; }

            if (main_grid.At(new_position) == flag_id) {
                // Set win flag to true
                std::cout << "flag found" << std::endl;
            }

            return true;
        }
    };
}