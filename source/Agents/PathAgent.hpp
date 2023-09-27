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
  /// Collection of ways to offset the Agent's position
  /// @attention This is a *not* a sequence of direct coordinates on the WorldGrid, but a series of offsets
  std::vector<cse491::GridPosition> offsets_;

  /// Current index into offsets_
  int index_ = -1;

 public:
  PathAgent() = default;
  PathAgent(size_t id, std::string const& name, std::vector<cse491::GridPosition> && offsets = {});
  PathAgent(size_t id, std::string const& name, std::string_view commands);
  ~PathAgent() = default;

  bool Initialize() override;
  size_t SelectAction(const WorldGrid &, const type_options_t &, const item_set_t &, const agent_set_t &) override;

  PathAgent& SetProperties(std::vector<cse491::GridPosition> && offsets, size_t start_index = 0);
  PathAgent& SetProperties(std::string_view commands, size_t start_index = 0);

  static std::vector<cse491::GridPosition> str_to_offsets(std::string_view commands);
};

} // namespace walle

#endif //GROUP_1_PROJECT_SOURCE_AGENTS_PATHAGENT_H_
