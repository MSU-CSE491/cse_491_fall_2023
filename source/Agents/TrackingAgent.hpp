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

#include <memory>
#include <limits>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_set>
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

  /**
   * A single Alerter is responsible for forcefully changing the state of all trackers in its network
   * to TrackingState::TRACKING when a single TrackingAgent in the set of trackers
   * comes into range of its target
   */
  class Alerter {
   private:
    /// World the agents in the agent network are a part of
    cse491::WorldBase* world_ptr_;

    /// Set of all agents in a single network
    std::unordered_set<size_t> agent_network_set_;

   public:
    Alerter(size_t id, cse491::WorldBase* world_ptr) : world_ptr_(world_ptr) {
      assert(world_ptr != nullptr);
      agent_network_set_.insert(id);
    }

    void AddAgent(size_t id) {
      agent_network_set_.insert(id);
    }

    void RemoveAgent(size_t id) {
      agent_network_set_.erase(id);
    }

    /**
     * Uses UpdateState to focus all trackers onto their target regardless of the distance away
     * @param caller_id the original id of the agent that came into range of its target; does not need to be updated
     */
    void AlertAllTrackingAgents(size_t caller_id) {
      for (auto id : agent_network_set_) {
        // Does not need to update the caller who gave the alert
        if (caller_id == id) {
          continue;
        }
        auto & agent = world_ptr_->GetAgent(id);
        assert(dynamic_cast<TrackingAgent*>(&agent) != nullptr);
        auto & tracking_agent = static_cast<TrackingAgent&>(agent);

        // UpdateState sets an agent's TrackingState to TRACKING if it is within some distance of the target
        // if this distance is positive infinity, then the state will always be reset (given that there IS a target)
        // Important: UpdateState must be called with alerting == false in order to
        // avoid infinite recursion from recursively calling AlertAllTrackingAgents
        double old_tracking_dist = tracking_agent.GetTrackingDistance();
        tracking_agent.SetTrackingDistance(std::numeric_limits<double>::infinity());
        tracking_agent.UpdateState(false);
        tracking_agent.SetTrackingDistance(old_tracking_dist);
      }
    }
  }; // Alerter

  std::shared_ptr<Alerter> alerter_ = nullptr;

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
  TrackingAgent(size_t id, std::string const &name) :
    cse491::AgentBase(id, name),
    inner_(std::in_place_type<PathAgent>, id, name) {}

  /**
   * Constructor (vector)
   * @param id unique agent id
   * @param name name of path agent
   * @param offsets collection of offsets to move the agent
   * @param alerter alerter network to add agent to
   * @attention The sequence of offsets must not be empty
   * @attention alerter should be a nullptr if this tracker is not part of any group of tracking agents
   */
  TrackingAgent(size_t id,
                std::string const &name,
                std::vector<cse491::GridPosition> &&offsets,
                std::shared_ptr<Alerter> alerter = nullptr) :
    cse491::AgentBase(id, name),
    inner_(std::in_place_type<PathAgent>, id, name, offsets),
    offsets_(offsets),
    alerter_(alerter){}

  /**
   * Constructor (string view)
   * @param id unique agent id
   * @param name name of path agent
   * @param commands sequence of commands to be interpreted as offsets
   * @attention The sequence of offsets must not be empty
   */
  TrackingAgent(size_t id,
                std::string const &name,
                std::string_view commands,
                std::shared_ptr<Alerter> alerter = nullptr) :
    cse491::AgentBase(id, name),
    inner_(std::in_place_type<PathAgent>, id, name, commands),
    offsets_(StrToOffsets(commands)),
    alerter_(alerter){}

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

      if (property_map.contains("alerter")) {
        auto alerter = GetProperty<std::shared_ptr<Alerter>>("alerter");
        AddToAlerter(alerter_);
      }
      return std::get<PathAgent>(inner_).Initialize();
    }
    return false;
  }

  void MakeAlerter() {
    alerter_ = std::make_shared<Alerter>(id, &GetWorld());
  }

  void AddToAlerter(std::shared_ptr<Alerter> alerter) {
    alerter_ = alerter;
    alerter->AddAgent(id);
  }

  void RemoveFromAlerter() {
    alerter_->RemoveAgent(id);
    alerter_ = nullptr;
  }

  std::shared_ptr<Alerter> GetAlerter() {
    return alerter_;
  }

  /**
   * Handles focusing the agent onto a target, returning it to its original location, and patrolling
   * @param alerting determines whether this agent should alert all other TrackingAgents in its network when its
   * target comes into range
   * @note the inner variant type will be AStarAgent when tracking OR returning to a location, but PathAgent when patrolling
   */
  void UpdateState(bool alerting = true) {
    SetPosition(std::visit([]<TrackingAgentInner Agent>(Agent const &agent) { return agent.GetPosition(); }, inner_));
//    bool changed_internal_agent = false;
    switch (state_) {
      // Tracking can transition only to Returning
      case TrackingState::TRACKING: {
        // Reached goal position
        if (GetPosition() == std::get<AStarAgent>(inner_).GetGoalPosition()) {
          // Target is still in range of goal position
          if (target_ != nullptr && GetPosition().Distance(target_->GetPosition()) < tracking_distance_) {
            std::get<AStarAgent>(inner_).SetGoalPosition(target_->GetPosition());

            // Alert all trackers
            if (alerting && alerter_ != nullptr) {
              alerter_->AlertAllTrackingAgents(id);
            }
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

          // Alert other trackers
          if (alerting && alerter_ != nullptr) {
            alerter_->AlertAllTrackingAgents(id);
          }
        }

          // Returned to the beginning, start patrolling again
        else if (GetPosition() == start_pos_) {
          state_ = TrackingState::PATROLLING;
          inner_.emplace<PathAgent>(id, name);
          std::get<PathAgent>(inner_).SetPosition(GetPosition());
          std::get<PathAgent>(inner_).SetPath(std::vector(offsets_));

          // Inner world_ptr needs to be reset
          SetWorld(GetWorld());
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

          // Inner world_ptr needs to be set
          SetWorld(GetWorld());

          std::get<AStarAgent>(inner_).RecalculatePath();
          std::get<AStarAgent>(inner_).SetActionResult(1);

          // Alert all other trackers
          if (alerting && alerter_ != nullptr) {
            alerter_->AlertAllTrackingAgents(id);
          }
        }
        break;
      }
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

  template<TrackingAgentInner Agent>
  size_t SelectInnerAction(Agent &agent, cse491::WorldGrid const &grid,
                           cse491::type_options_t const &type,
                           cse491::item_map_t const &item_set,
                           cse491::agent_map_t const &agent_set) {
    return agent.SelectAction(grid, type, item_set, agent_set);
  }

  template<>
  size_t SelectInnerAction<AStarAgent>(AStarAgent &agent, cse491::WorldGrid const &grid,
                                       cse491::type_options_t const &type,
                                       cse491::item_map_t const &item_set,
                                       cse491::agent_map_t const &agent_set) {
    auto next_pos = agent.GetNextPosition();
    auto res = agent.SelectAction(grid, type, item_set, agent_set);
    agent.SetPosition(next_pos);
    return res;
  }

  size_t SelectAction(cse491::WorldGrid const &grid,
                      cse491::type_options_t const &type,
                      cse491::item_map_t const &item_set,
                      cse491::agent_map_t const &agent_set) override {
    UpdateState();
    return std::visit([&]<TrackingAgentInner Agent>(Agent &agent) {
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
   * Get the distance around this tracker that it surveys
   * @return tracking distance
   */
  [[nodiscard]] double GetTrackingDistance() { return tracking_distance_; }

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
   * Set both the world for the current agent and the agents it is a part of
   * @param in_world
   * @return
   */
  TrackingAgent &SetWorld(cse491::WorldBase &in_world) override {
    Entity::SetWorld(in_world);
    std::visit([&in_world]<TrackingAgentInner Agent>(Agent &agent) {
      agent.SetWorld(in_world);
      std::as_const(in_world).ConfigAgent(agent);
    }, inner_);
    return *this;
  }

  /**
   * Retrieves the current internal state of the Tracking Agent
   * @return
   */
  TrackingState GetState() {
    return state_;
  }
};
} // namespace walle
