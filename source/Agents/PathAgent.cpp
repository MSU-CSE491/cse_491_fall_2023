/**
 * @file PathAgent.cpp
 * @author David Rackerby
 */

#include "PathAgent.h"

#include <regex>
#include <stdexcept>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "../core/AgentBase.hpp"
#include "../core/GridPosition.hpp"

namespace walle {

/**
 * Constructor
 * @param id unique agent id
 * @param name name of path agent
 * @param offsets collection of offsets to move the agent
 */
PathAgent::PathAgent(size_t id, std::string const& name,
                     std::vector<cse491::GridPosition> && offsets) : cse491::AgentBase(id, name), offsets_(offsets) {}

/**
 * Constructor that takes a string for convenience
 *
 * This convenience constructor takes a string with a special formatting that allows one to specify
 * a sequence of inputs in linear directions.
 * The format is as follows [steps[*]]<direction> where
 * `steps` is a positive integer and optional (assumed to be 1 by default)
 * the star `*` represents scaling the movement by `steps` and should not be used without `steps`
 * if the star is not present, there are instead `steps` separate steps are taken in the direction `direction`
 * Example: "n w 3e 10*s 5*w" should create the sequence of offsets
 * {-1, 0}, {0, 1}, {0, -1}, {0, -1}, {0, -1}, {10, 0}, {0, 5}
 * @param id unique agent id
 * @param name name of path agent
 * @param directions string in a format of sequential directions
 *
 * @note throws an `std::invalid_argument` when input string is poorly formatted
 * @note this includes when a negative integer is passed as `steps`. If a zero is used, treated as the default (one)
 */
PathAgent::PathAgent(size_t id, std::string const& name, std::string_view commands) : cse491::AgentBase(id, name) {
  // Regex capturing groups logically mean the following:
  // Group 0: whole regex
  // Group 1: `steps` and multiplicity pair (optional)
  // Group 2: `steps` (optional)
  // Group 3: `*` (optional, only matches when Group 2 matches)
  // Group 4: direction
  std::regex pattern (R"(([1-9]\d*)(\*?))?([nswe])");
  std::smatch pattern_match;

  std::istringstream iss(commands);
  iss >> std::skipws;

  std::string single_command;
  while(iss >> single_command) {
    if (std::regex_match(single_command, pattern_match, pattern)) {
      int steps = 1;

      if (pattern_match[2].length() > 0) {
        std::istringstream step_val(pattern_match[1].str());
        step_val >> steps;
      }

      bool multiply = pattern_match[3].length() > 0;

      char direction = pattern_match[4].str()[0];

      cse491::GridPosition base_pos;
      switch (direction) {
        case 'n': {
          if (multiply) {
            offsets_.push_back(base_pos.Above(steps));
          }
          else {
            for (int i = 0; i < steps; ++i) {
              offsets_.push_back(base_pos.Above());
            }
          }
          break;
        }

        case 's': {
          if (multiply) {
            offsets_.push_back(base_pos.Below(steps));
          }
          else {
            for (int i = 0; i < steps; ++i) {
              offsets_.push_back(base_pos.Below());
            }
          }
          break;
        }

        case 'e': {
          if (multiply) {
            offsets_.push_back(base_pos.ToLeft(steps));
          }
          else {
            for (int i = 0; i < steps; ++i) {
              offsets_.push_back(base_pos.ToLeft());
            }
          }
          break;
        }

        case 'w': {
          if (multiply) {
            offsets_.push_back(base_pos.ToRight(steps));
          }
          else {
            for (int i = 0; i < steps; ++i) {
              offsets_.push_back(base_pos.ToRight());
            }
          }
          break;
        }
      }
    }

    else {
      throw std::invalid_argument("Incorrectly formatted argument string to PathAgent constructor");
    }

    iss >> std::skipws;
  }
}

bool PathAgent::Initialize() {
  return HasAction("move_self");
}

size_t PathAgent::SelectAction(const WorldGrid & /* grid*/,
                               const type_options_t & /* type_options*/,
                               const item_set_t & /* item_set*/,
                               const agent_set_t & /* agent_set*/)
{
  cse491::GridPosition offset = offsets_[index_];
  SetPosition(position.CellX() + offset.CellX(), position.CellY() + offset.CellY());

  // Restart cycling through offsets
  ++index_;
  if (index_ >= offsets_.size()) {
    index_ = 0;
  }

  return action_map["move_self"];
}

} // namespace walle