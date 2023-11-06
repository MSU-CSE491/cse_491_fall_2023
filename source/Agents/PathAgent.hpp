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
#include "AgentLibary.hpp"

namespace walle {

/**
 * Agent that has a user-defined custom movement pattern
 * Passed a sequence of to be sequentially applied as the agent is updated
 */
class PathAgent : public cse491::AgentBase {
protected:
  /// Collection of ways to offset the Agent's position
  /// @attention This is a *not* a sequence of direct coordinates on the
  /// WorldGrid, but a series of offsets
  std::vector<cse491::GridPosition> offsets_;

  /// Current index into offsets_
  int index_ = 0;

public:
  PathAgent() = delete;
  PathAgent(size_t id, std::string const &name);
  PathAgent(size_t id, std::string const &name,
            std::vector<cse491::GridPosition> offsets);
  PathAgent(size_t id, std::string const &name, std::string_view commands);
  ~PathAgent() override = default;

  bool Initialize() override;

  void IncrementIndex();
  void DecrementIndex();

  [[nodiscard]] cse491::GridPosition CalcNextPos() const;

  virtual cse491::GridPosition UpdateAndGetNextPos(bool increment);

  size_t SelectAction(cse491::WorldGrid const&, cse491::type_options_t const&,
                      cse491::item_map_t const&, cse491::agent_map_t const&) override;

  [[nodiscard]] cse491::GridPosition GetNextPosition() override;

  PathAgent &SetPath(std::vector<cse491::GridPosition> &&offsets,
                     size_t start_index = 0);
  PathAgent &SetPath(std::string_view commands, size_t start_index = 0);

  [[nodiscard]] int GetIndex() const;
  [[nodiscard]] std::vector<cse491::GridPosition> const &GetPath() const;
  PathAgent &ResetIndex() {
    index_ = 0;
    return *this;
  }
};

} // namespace walle

#endif // GROUP_1_PROJECT_SOURCE_AGENTS_PATHAGENT_H_
