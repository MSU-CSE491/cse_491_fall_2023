/**
 * @file matt_k_first_agent.h
 * @author Matt Kight
 */
#pragma once

#include <vector>
#include <algorthim>
#include "../core/AgentBase.hpp"
#include "AgentLibary.h"

namespace walle
{
    /**
     * Class that describes a matt_k_first_agent class
     */
    class matt_k_first_agent : public cse491::AgentBase
    {
    private:
        std::vector<size_t> path;           // Path this agent is taking
        cse491::GridPosition goal_position; // Where the agent wants to end up
        int recalculate_after_x_turns = 5;  // How often agent recalculates moves
        int current_move_num = 0;           // What move # we are currently on
    public:
        matt_k_first_agent(size_t id, const std::string &name) : AgentBase(id, name) {}
        ~matt_k_first_agent() = default;

        /// @brief This agent needs a specific set of actions to function.
        /// @return Success.
        bool Initialize() override
        {
            return HasAction("up") && HasAction("down") && HasAction("left") && HasAction("right");
        }

        /// Choose the action to take a step in the appropriate direction.
        size_t SelectAction(const cse491::WorldGrid &grid,
                            const type_options_t & /* type_options*/,
                            const item_set_t & /* item_set*/,
                            const agent_set_t & /* agent_set*/) override
        {
            // We are taking an action so another turn has passed
            ++(this->current_move_num)
                // If the last step failed, or we need a new path the then regenerate the path
                if (action_result == 0 || this->path.empty() || this->current_move_num > this->recalculate_after_x_turns)
            {
                this->path = this->world.find_path(GetPosition(), this->goal_position, grid);
                std::reverse(this->path.begin(), this->path.end());
            }
            // Return whatever action gets us closer to our goal
            if (!this->path.empty())
            {
                return this->path.pop_back()
            }
            return 0; // If no path then do not do anything
        }
    };
};
}
