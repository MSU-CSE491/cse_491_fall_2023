/**
 * @file PathAgent.cpp
 * @author David Rackerby
 */

#include "PathAgent.hpp"

#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "../core/AgentBase.hpp"
#include "../core/GridPosition.hpp"
namespace walle {

/**
 * Constructor (agent default)
 * @param id unique agent id
 * @param name name of path agent
 * @note When this constructor is called, the agent must still be assigned a
 * path before a call to Initialize
 */
PathAgent::PathAgent(size_t id, std::string const &name)
    : cse491::AgentBase(id, name) {}

/**
 * Constructor (vector)
 * @param id unique agent id
 * @param name name of path agent
 * @param offsets collection of offsets to move the agent
 * @attention The sequence of offsets must not be empty
 */
PathAgent::PathAgent(size_t id, std::string const &name,
                     std::vector<cse491::GridPosition> &&offsets)
    : cse491::AgentBase(id, name), offsets_(offsets) {
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
PathAgent::PathAgent(size_t id, std::string const &name,
                     std::string_view commands)
    : cse491::AgentBase(id, name), offsets_(StrToOffsets(commands)) {
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
  if (property_map.contains("path")) {
    offsets_ = StrToOffsets(GetProperty<std::basic_string_view<char>>("path"));
  } else {
    return false;
  }
  return HasAction("move_arbitrary") && index_ >= 0 &&
         static_cast<size_t>(index_) < offsets_.size();
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
 * Applies the current offset to calculate the next position and then adjusts
 * the index
 * @param increment decides whether to move in the forward or backward direction
 * to allow for complex pathing
 * @return
 */
cse491::GridPosition PathAgent::UpdateAndGetNextPos(bool increment) {
  auto next_pos = GetNextPos();
  if (increment) {
    IncrementIndex();
  } else {
    DecrementIndex();
  }
  return next_pos;
}

/**
 * Tells world to
 * @return whether the update succeeded
 */
size_t PathAgent::SelectAction(cse491::WorldGrid const & /* grid*/,
                               cse491::type_options_t const & /* type_options*/,
                               cse491::item_set_t const & /* item_set*/,
                               cse491::agent_set_t const & /* agent_set*/) {
  assert(HasAction("move_arbitrary"));
  return action_map["move_arbitrary"];
}

/**
 * Assigns the offsets_member to a new series of offsets
 * @param offsets collection of grid positions used as the new offsets
 * @param start_index which offset to start indexing into (beginning by default)
 * @return self
 * @attention throws an `std::invalid_argument` when an invalid start index is
 * provided
 */
PathAgent &PathAgent::SetPath(std::vector<cse491::GridPosition> &&offsets,
                              size_t start_index) {
  offsets_ = offsets;
  index_ = static_cast<int>(start_index);
  if (static_cast<size_t>(index_) >= offsets_.size()) {
    std::ostringstream what;
    what << "Out of bounds offset index to begin from: " << index_
         << ", number of offsets: " << offsets_.size();
    throw std::invalid_argument(what.str());
  }
  return *this;
}

/**
 * Assigns the offsets_ member to a new series of offsets, taking a command
 * string
 * @param commands formatted string of commands used as offsets
 * @param start_index which command to begin indexing into (first command by
 * default)
 * @return self
 * @attention throws an `std::invalid_argument` when mis-formatted commands an
 * invalid index is provided
 */
PathAgent &PathAgent::SetPath(std::string_view commands, size_t start_index) {
  offsets_.clear();
  return SetPath(StrToOffsets(commands), start_index);
}

/**
 * Retrieves which step the agent is on
 * @return the current index into the offsets
 */
int PathAgent::GetIndex() const { return index_; }

/**
 * Returns an immutable reference to this agent's current path
 * @return sequence of offsets
 */
std::vector<cse491::GridPosition> const &PathAgent::GetPath() const {
  return offsets_;
}

} // namespace walle