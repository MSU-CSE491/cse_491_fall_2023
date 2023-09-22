/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief An Agent that will walk back and forth along a row or column.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <cassert>

#include "../core/AgentBase.hpp"

namespace cse491 {

  class PacingAgent : public AgentBase {
  protected:
    bool vertical=true; ///< Is this agent moving down&up?  False = right&left.
    bool reverse=false;  ///< Is this agent on their way back? (up/left?)

  public:
    PacingAgent(size_t id, const std::string & name) : AgentBase(id, name) { }
    ~PacingAgent() = default;

    /// @brief This agent needs a specific set of actions to function.
    /// @return Success.
    bool Initialize() override {
      return HasAction("up") && HasAction("down") && HasAction("left") && HasAction("right");
    }

    /// Choose the action to take a step in the appropriate direction.
    size_t SelectAction(const WorldGrid & /* grid*/,
                        const type_options_t & /* type_options*/,
                        const item_set_t & /* item_set*/,
                        const agent_set_t & /* agent_set*/) override
    {
      // If the last step failed, try going in the other direction.
      if (action_result == 0) {
        reverse = !reverse;
      }
      // Take as tep in the direction we are trying to go in.
      if (vertical) {
        if (reverse) return action_map["up"];
        else         return action_map["down"];
      } else {
        if (reverse) return action_map["left"];
        else         return action_map["right"];
      }
      return 0;  // Should never actually get here...
    }

  };

} // End of namespace cse491
