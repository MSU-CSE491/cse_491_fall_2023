/**
 * @file matt_k_first_agent.h
 * @author Matt Kight
 */
#pragma once

#include <vector>
#include "../core/AgentBase.hpp"
#include "AgentLibary.h"
#include <iostream>
namespace cse491
{
    namespace walle
    {
        /**
         * Class that describes a matt_k_first_agent class
         */
        class matt_k_first_agent : public cse491::AgentBase
        {
        private:
            std::vector<GridPosition> path;      // Path this agent is taking
            cse491::GridPosition goal_position;  // Where the agent wants to end up
            int recalculate_after_x_turns = 100; // How often agent recalculates moves
            int current_move_num = 0;            // What move # we are currently on
            WorldBase *world = nullptr;

        public:
            matt_k_first_agent(size_t id, const std::string &name) : AgentBase(id, name)
            {
            }
            ~matt_k_first_agent() = default;

            /**
             * @brief Set the word object
             *
             * @param world world this agent is a part of
             */
            void set_word(WorldBase *world) { this->world = world; }

            /// @brief This agent needs a specific set of actions to function.
            /// @return Success.
            bool Initialize() override
            {
                return HasAction("up") && HasAction("down") && HasAction("left") && HasAction("right");
            }

            void set_goal_position(const GridPosition gp)
            {
                goal_position = gp;
            }
            /// Choose the action to take a step in the appropriate direction.
            size_t SelectAction(const WorldGrid & /*grid*/,
                                const type_options_t & /* type_options*/,
                                const item_set_t & /* item_set*/,
                                const agent_set_t & /* agent_set*/) override
            {
                // TODO REMOVE THIS
                // We are taking an action so another turn has passed
                ++(this->current_move_num);
                // If the last step failed, or we need a new path the then regenerate the path
                if (action_result == 0 || this->path.empty() || this->current_move_num > this->recalculate_after_x_turns)
                {
                    this->path = this->world->shortest_path(GetPosition(), this->goal_position);
                }
                // Return whatever action gets us closer to our goal
                if (!this->path.empty())
                {
                    auto pos = path.back();
                    path.pop_back();
                    if (pos.GetX() == position.GetX() && pos.GetY() == position.GetY() - 1)
                        return action_map["up"];
                    if (pos.GetX() == position.GetX() && pos.GetY() == position.GetY() + 1)
                        return action_map["down"];
                    if (pos.GetX() == position.GetX() - 1 && pos.GetY() == position.GetY())
                        return action_map["left"];
                    if (pos.GetX() == position.GetX() + 1 && pos.GetY() == position.GetY())
                        return action_map["right"];
                }
                return 0; // If no path then do not do anything
            }
        };
    };
}
