/**
 * @file PathAgent.cpp
 * @author David Rackerby
 */

#include "PathAgent.hpp"

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
 * Constructor (vector)
 * @param id unique agent id
 * @param name name of path agent
 * @param offsets collection of offsets to move the agent
 * @attention The sequence of offsets must not be empty
 */
PathAgent::PathAgent(size_t id, std::string const& name,
                     std::vector<cse491::GridPosition> && offsets) : cse491::AgentBase(id, name), offsets_(offsets) {
  if (offsets_.empty()) {
    throw std::invalid_argument("Sequence of input offsets must not be empty");
  }
}

/**
 * Constructor (string)
 * @param id unique agent id
 * @param name name of path agent
 * @param commands sequence of commands to be interpreted as offsets
 * @attention The sequence of offsets must not be empty
 */
PathAgent::PathAgent(size_t id, std::string const& name,
                     std::string_view commands) : cse491::AgentBase(id, name), offsets_(StrToOffsets(commands)) {
  if (offsets_.empty()) {
    throw std::invalid_argument("Sequence of input offsets must not be empty");
  }
}

/**
 * Checks that the agent is able to move arbitrarily
 * Verifies that it can currently index into a valid offset
 * @return true if so; false otherwise
 */
bool PathAgent::Initialize() {
  return HasAction("move_arbitrary") && index_ >= 0 && static_cast<size_t>(index_) < offsets_.size();
}

/**
 * Increments the index into the offsets sequence
 */
 void PathAgent::IncrementIndex() {
  ++index_;

  // Wrap-around to front of offsets
  if (index_ >= static_cast<int>(offsets_.size())) {
    index_ = 0;
  }
 }

 /**
  * Decrements the index into the offsets sequence
  */
 void PathAgent::DecrementIndex() {
   --index_;

   // Wrap-around to back of offsets
   if (index_ < 0) {
     index_ = static_cast<int>(offsets_.size()) - 1;
   }
 }

 /**
  * Retrieves the position of the agent after applying the current offset
  * @return next position of the agent
  */
cse491::GridPosition PathAgent::GetNextPos() const {
  return offsets_[index_] + GetPosition();
}

/**
 * Convenience method
 * Applies the current offset to calculate the next position and then adjusts the index
 * @param increment decides whether to move in the forward or backward direction to allow for complex pathing
 * @return
 */
cse491::GridPosition PathAgent::UpdateAndGetNextPos(bool increment) {
  auto next_pos = GetNextPos();
  if (increment) {
    IncrementIndex();
  }
  else {
    DecrementIndex();
  }
  return next_pos;
}

/**
 * Tells world to
 * @return whether the update succeeded
 */
size_t PathAgent::SelectAction(cse491::WorldGrid const& /* grid*/,
                               cse491::type_options_t const& /* type_options*/,
                               cse491::item_set_t const& /* item_set*/,
                               cse491::agent_set_t const& /* agent_set*/) {
  assert(HasAction("move_arbitrary"));
  return action_map["move_arbitrary"];
}

/**
 * Assigns the offsets_member to a new series of offsets
 * @param offsets collection of grid positions used as the new offsets
 * @param start_index which offset to start indexing into (beginning by default)
 * @return self
 * @attention throws an `std::invalid_argument` when an invalid start index is provided
 */
PathAgent& PathAgent::SetPath(std::vector<cse491::GridPosition> && offsets, size_t start_index) {
  offsets_ = offsets;
  index_ = static_cast<int>(start_index);
  if (static_cast<size_t>(index_) >= offsets_.size()) {
    std::ostringstream what;
    what << "Out of bounds offset index to begin from: " << index_ << ", number of offsets: " << offsets_.size();
    throw std::invalid_argument(what.str());
  }
  return *this;
}

/**
 * Assigns the offsets_ member to a new series of offsets, taking a command string
 * @param commands formatted string of commands used as offsets
 * @param start_index which command to begin indexing into (first command by default)
 * @return self
 * @attention throws an `std::invalid_argument` when mis-formatted commands an invalid index is provided
 */
PathAgent& PathAgent::SetPath(std::string_view commands, size_t start_index) {
  offsets_.clear();
  return SetPath(StrToOffsets(commands), start_index);
}

/**
 * Retrieves which step the agent is on
 * @return the current index into the offsets
 */
int PathAgent::GetIndex() const {
  return index_;
}

/**
 * Returns an immutable reference to this agent's current path
 * @return sequence of offsets
 */
std::vector<cse491::GridPosition> const& PathAgent::GetPath() const {
  return offsets_;
}

/**
 * Converts a string to a sequence of offsets
 *
 * This convenience method takes a string with a special formatting that allows one to specify
 * a sequence of whitespace-separated inputs in linear directions.
 * The format is [steps[*]]<direction> where
 * `steps` is a positive integer and optional (assumed to be 1 by default)
 * star `*` represents scaling the movement by `steps`. Optional, but cannot be used if `steps` is not provided
 * if the star is not present, then `steps` individual offsets are created in the direction `direction`
 * `direction` is a cardinal direction with the following logical mapping:
 * n: north
 * s: south
 * e: east
 * w: west
 * x: stay put
 * Example: "n w 3e 10*s 5*w x" should create the sequence of offsets
 * {0, -1}, {-1, 0}, {1, 0}, {1, 0}, {1, 0}, {0, 10}, {-5, 0}, {0, 0}
 * @param commands string in a format of sequential directions
 * @note throws an `std::invalid_argument` when input string is poorly formatted
 * @note this includes when a negative integer is passed as `steps`. If a zero is used, treated as the default (one)
 */
std::vector<cse491::GridPosition> StrToOffsets(std::string_view commands) {
  std::vector<cse491::GridPosition> positions;

  // Regex capturing groups logically mean the following:
  // Group 0: whole regex
  // Group 1: `steps` and `*` pair (optional)(unused)
  // Group 2: `steps` (optional)
  // Group 3: `*` (optional, only matches when Group 2 matches)
  // Group 4: direction
  std::regex pattern ("(([1-9]\\d*)(\\*?))?([nswex])");
  std::istringstream iss{std::string(commands)};
  iss >> std::skipws;

  std::string single_command;
    while (iss >> single_command) {
      std::smatch pattern_match;
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
          // Move up
          case 'n': {
            if (multiply) {
              positions.push_back(base_pos.Above(steps));
            } else {
              for (int i = 0; i < steps; ++i) {
                positions.push_back(base_pos.Above());
              }
            }
            break;
          }

            // Move down
          case 's': {
            if (multiply) {
              positions.push_back(base_pos.Below(steps));
            } else {
              for (int i = 0; i < steps; ++i) {
                positions.push_back(base_pos.Below());
              }
            }
            break;
          }

            // Move left
          case 'w': {
            if (multiply) {
              positions.push_back(base_pos.ToLeft(steps));
            } else {
              for (int i = 0; i < steps; ++i) {
                positions.push_back(base_pos.ToLeft());
              }
            }
            break;
          }

            // Move right
          case 'e': {
            if (multiply) {
              positions.push_back(base_pos.ToRight(steps));
            } else {
              for (int i = 0; i < steps; ++i) {
                positions.push_back(base_pos.ToRight());
              }
            }
            break;
          }

            // Stay
          case 'x': {
            // Using the `*` does nothing to scale the offset since it's scaling {0, 0}
            steps = multiply ? 1 : steps;

            for (int i = 0; i < steps; ++i) {
              positions.push_back(base_pos);
            }
          }
        }
      } else {
        std::ostringstream what;
        what << "Incorrectly formatted argument: " << single_command;
        throw std::invalid_argument(what.str());
      }

      iss >> std::skipws;
    }
  return positions;
}

} // namespace walle