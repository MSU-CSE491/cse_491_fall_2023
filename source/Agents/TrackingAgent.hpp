/**
 * @file TrackingAgent.hpp
 * @author Matt Kight, David Rackerby
 *
 * Agent that switches between  user-defined custom movement pattern and
 * tracking a given agent
 */

#pragma once

#include "../core/AgentBase.hpp"
#include "../core/GridPosition.hpp"
#include "AStarAgent.hpp"
#include "PathAgent.hpp"

#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

namespace walle {

/**
 * Used to keep track of what action we are currently taking
 */
enum class TrackingState { RETURNING_TO_START, TRACKING, PATROLLING };

/**
 * Constrain possible inner types to only PathAgent and AStarAgent to avoid errors
 * @tparam T either PathAgent or AStarAgent
 */
template<typename T>
concept TrackingAgentInner = std::is_same_v<PathAgent, T> || std::is_same_v<AStarAgent, T>;

/**
 * Agent that switches between  user-defined custom movement pattern and
 * tracking a given agent
 */
class TrackingAgent : public cse491::AgentBase {
 private:
  /// Internal agent whose type depends on the state of the agent
  std::variant<PathAgent, AStarAgent> inner_;

  /// Internal state of the agent
  TrackingState state_ = TrackingState::PATROLLING;

  /// Patrol path when not tracking
  std::vector<cse491::GridPosition> offsets_;

  /// Starting position
  cse491::GridPosition start_pos_;

  /// Entity that the agent tracks and moves towards
  Entity *target_ = nullptr;

  /// How close the target needs to be to begin tracking
  double tracking_distance_ = 50;

 public:
  /**
   * Delete default constructor
   */
  TrackingAgent() = delete;

  /**
   * Constructor (default)
   * @param id unique agent id
   * @param name name of path agent
   */
  TrackingAgent(size_t id, std::string const &name) : cse491::AgentBase(id, name),
                                                      inner_(std::in_place_type<PathAgent>, id, name) {}

  /**
   * Constructor (vector)
   * @param id unique agent id
   * @param name name of path agent
   * @param offsets collection of offsets to move the agent
   * @attention The sequence of offsets must not be empty
   */
  TrackingAgent(size_t id, std::string const &name, std::vector<cse491::GridPosition> &&offsets) : cse491::AgentBase(id,
                                                                                                                     name),
                                                                                                   inner_(std::in_place_type<
                                                                                                              PathAgent>,
                                                                                                          id,
                                                                                                          name,
                                                                                                          offsets),
                                                                                                   offsets_(offsets) {}
  /**
   * Constructor (string view)
   * @param id unique agent id
   * @param name name of path agent
   * @param commands sequence of commands to be interpreted as offsets
   * @attention The sequence of offsets must not be empty
   */
  TrackingAgent(size_t id, std::string const &name, std::string_view commands) :
      cse491::AgentBase(id, name),
      inner_(std::in_place_type<PathAgent>, id, name, commands), offsets_(StrToOffsets(commands)) {}

  /**
   * Destructor
   */
  ~TrackingAgent() override = default;

  /**
   * Ensure that the TrackingAgent's internal PathAgent is correctly initialized
   * Verifies that it can currently index into a valid offset
   * @return true if so; false otherwise
   */
  bool Initialize() override {
      SetStartPosition(GetPosition());
      if (property_map.contains("path")) {
          auto view = GetProperty<std::basic_string_view<char>>("path");
          offsets_ = StrToOffsets(view);
          std::get<PathAgent>(inner_).SetProperty("path", view);
          std::get<PathAgent>(inner_).SetWorld(GetWorld());
          std::get<PathAgent>(inner_).SetPosition(GetPosition());
          return std::get<PathAgent>(inner_).Initialize();
      }
      return !offsets_.empty();
  }

  /**
   * Handles focusing the agent onto a target, returning it to its original location, and patrolling
   * @note the inner variant type will be AStarAgent when tracking OR returning to a location, but PathAgent when patrolling
   */
  void UpdateState() {
      SetPosition(std::visit([]<TrackingAgentInner Agent>(Agent const &agent) { return agent.GetPosition(); }, inner_));
      bool changed_internal_agent = false;
      switch (state_) {
          // Tracking can transition only to Returning
          case TrackingState::TRACKING: {
              // Reached goal position
              if (GetPosition() == std::get<AStarAgent>(inner_).GetGoalPosition()) {
                  // Target is still in range of goal position
                  if (target_ != nullptr && GetPosition().Distance(target_->GetPosition()) < tracking_distance_) {
                      std::get<AStarAgent>(inner_).SetGoalPosition(target_->GetPosition());
                  }
                      // Target moved out of range of goal position, return to start
                  else {
                      state_ = TrackingState::RETURNING_TO_START;
                      std::get<AStarAgent>(inner_).SetGoalPosition(start_pos_);
                  }
                  std::get<AStarAgent>(inner_).RecalculatePath();
                  std::get<AStarAgent>(inner_).SetActionResult(1);
              }
              break;
          }

              // Returning can transition to either Patrolling or Tracking
          case TrackingState::RETURNING_TO_START: {
              // Within tracking range, start tracking again
              if (target_ != nullptr && GetPosition().Distance(target_->GetPosition()) < tracking_distance_) {
                  state_ = TrackingState::TRACKING;
                  std::get<AStarAgent>(inner_).SetGoalPosition(target_->GetPosition());
                  std::get<AStarAgent>(inner_).RecalculatePath();
                  std::get<AStarAgent>(inner_).SetActionResult(1);
              }

                  // Returned to the beginning, start patrolling again
              else if (GetPosition() == start_pos_) {
                  state_ = TrackingState::PATROLLING;
                  inner_.emplace<PathAgent>(id, name);
                  std::get<PathAgent>(inner_).SetPosition(GetPosition());
                  std::get<PathAgent>(inner_).SetPath(std::vector(offsets_));
                  changed_internal_agent = true;
              }
              break;
          }

              // Patrolling can transition only to Tracking
          case TrackingState::PATROLLING: {
              // Within tracking range, needs internal object replacement
              if (target_ != nullptr && GetPosition().Distance(target_->GetPosition()) < tracking_distance_) {
                  state_ = TrackingState::TRACKING;
                  inner_.emplace<AStarAgent>(id, name);
                  // Set internal AStarAgent's position to the outer position
                  std::get<AStarAgent>(inner_).SetPosition(GetPosition());
                  std::get<AStarAgent>(inner_).SetGoalPosition(target_->GetPosition());
                  std::get<AStarAgent>(inner_).RecalculatePath();
                  std::get<AStarAgent>(inner_).SetActionResult(1);
                  changed_internal_agent = true;
              }
              break;
          }
      }

      if (changed_internal_agent) {
          // Reset world configuration
          std::visit([&in_world = GetWorld()]<TrackingAgentInner Agent>(Agent &agent) {
                       std::as_const(in_world).ConfigAgent(agent);
                       agent.SetWorld(in_world);
                     },
                     inner_);
      }
  }

  /**
   * Overrides the AgentBase getter to retrieve the next calculated position
   * @return inner PathAgent's next position
   */
  [[nodiscard]] cse491::GridPosition GetNextPosition() override {
      auto pos = std::get<PathAgent>(inner_).GetNextPosition();
      std::get<PathAgent>(inner_).SetPosition(pos);
      return pos;
  }

	/// Select action for PathAgent inner type
	size_t SelectInnerAction(PathAgent &agent,
							 cse491::WorldGrid const &grid,
							 cse491::type_options_t const &type,
							 cse491::item_map_t const &item_set,
							 cse491::agent_map_t const &agent_set) {
		return agent.SelectAction(grid, type, item_set, agent_set);
	}

	/// Select action for AStarAgent inner type
	size_t SelectInnerAction(AStarAgent &agent,
							 cse491::WorldGrid const &grid,
							 cse491::type_options_t const &type,
							 cse491::item_map_t const &item_set,
							 cse491::agent_map_t const &agent_set) {
		auto next_pos = agent.GetNextPosition();
		auto res = agent.SelectAction(grid, type, item_set, agent_set);
		agent.SetPosition(next_pos);
		return res;
	}

	/// Updates the internal state of the TrackingAgent and calls the internal agent's select action method
	size_t SelectAction(cse491::WorldGrid const &grid,
						cse491::type_options_t const &type,
						cse491::item_map_t const &item_set,
						cse491::agent_map_t const &agent_set) override {
		UpdateState();
		return std::visit([&]<TrackingAgentInner AgentInner>(AgentInner &agent) {
							  return SelectInnerAction(agent, grid, type, item_set, agent_set);
						  },
						  inner_);
	}

  /**
   * Set where this agent "patrol area" starts
   * @param gp grid position of position
   * @return self
   */
  TrackingAgent &SetStartPosition(cse491::GridPosition gp) {
      start_pos_ = gp;
      return *this;
  }

  /**
   * Set where this agent "patrol area" starts
   * @param x x-coor of start pos
   * @param y y-coor of start pos
   * @return self
   */
  TrackingAgent &SetStartPosition(double x, double y) {
      start_pos_ = cse491::GridPosition(x, y);
      return *this;
  }

  /**
   * Set which agent we are following
   * @param agent we want to track
   * @return self
   */
  TrackingAgent &SetTarget(Entity *agent) {
      target_ = agent;
      return *this;
  }

  /**
   * Set how close target has to be to start tracking
   * @param dist to start tracking at
   * @return self
   */
  TrackingAgent &SetTrackingDistance(double dist) {
      tracking_distance_ = dist;
      return *this;
  }

  /**
   * Set both the world for our currernt agent and the agents it is a part of
   * @param in_world
   * @return
   */
  TrackingAgent &SetWorld(cse491::WorldBase &in_world) override {
      Entity::SetWorld(in_world);
      std::visit([&in_world]<TrackingAgentInner Agent>(Agent &agent) {
        std::as_const(in_world).ConfigAgent(agent);
      }, inner_);
      return *this;
  }

  TrackingAgent &SetPath(std::vector<cse491::GridPosition> offsets) {
      offsets_ = std::move(offsets);
      return *this;
  }
};
} // namespace walle
