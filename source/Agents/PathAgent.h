/**
 * @file PathAgent.h
 * @author David Rackerby
 *
 * Agent to move in predefined path
 */

#ifndef GROUP_1_PROJECT_SOURCE_AGENTS_PATHAGENT_H_
#define GROUP_1_PROJECT_SOURCE_AGENTS_PATHAGENT_H_

#include <string>
#include <string_view>
#include <vector>

#include "../core/AgentBase.hpp"
#include "../core/GridPosition.hpp"

namespace walle {

/**
 * Agent that has a user-defined custom movement pattern
 * Passed a sequence of to be sequentially applied as the agent is updated
 */
class PathAgent : public cse491::AgentBase {
 private:
  /// The current index into the path that the agent is on
  size_t index_ = 0;

  /// The directions that the agent should move in
  /// @attention This is a *not* sequence of coordinates on the WorldGrid, but a series of offsets to move the agent in
  std::vector<cse491::GridPosition> offsets_;

 public:
  PathAgent() = delete;
  PathAgent(size_t id, std::string const& name, std::vector<cse491::GridPosition> && offsets);
  PathAgent(size_t id, std::string const& name, std::string_view commands);
  ~PathAgent() = default;

  bool Initialize() override;
  size_t SelectAction(const WorldGrid &, const type_options_t &, const item_set_t &, const agent_set_t &) override;

};

} // namespace walle

#endif //GROUP_1_PROJECT_SOURCE_AGENTS_PATHAGENT_H_
