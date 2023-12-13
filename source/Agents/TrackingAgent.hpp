/**
 * This file is part of the Fall 2023, CSE 491 Course Project
 * @file TrackingAgent.hpp
 * @brief Agent that switches between user-defined custom movement pattern and
 * tracking a given agent
 * @note Status: ALPHA
 * @author David Rackerby
 * @author Matt Kight
 **/

#pragma once

#include <limits>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_set>
#include <variant>
#include <vector>

#include "../core/AgentBase.hpp"
#include "../core/GridPosition.hpp"
#include "AStarAgent.hpp"
#include "AgentLibary.hpp"
#include "PathAgent.hpp"

namespace walle {

/**
 * A single Alerter is responsible for forcefully changing the state of all trackers in its network
 * to TrackingState::TRACKING when a single TrackingAgent in the set of trackers
 * comes into range of its goal_pos
 */
class Alerter {
private:
  /// World the agents in the agent network are a part of
  cse491::WorldBase *world_ptr_;

  /// Set of all agents in a single network
  std::unordered_set<size_t> agent_network_set_;

public:
  // Must be constructed with an associated world
  Alerter() = delete;
  explicit Alerter(cse491::WorldBase *world_ptr);
  Alerter(cse491::WorldBase *world_ptr, size_t id);
  void AddAgent(size_t id);
  void RemoveAgent(size_t id);
  void AlertAllTrackingAgents(size_t caller_id);

  /// Returns an immutable reference to the Alerter's set of agents it knows about
  std::unordered_set<size_t> const &GetNetworkSet() const { return agent_network_set_; }
};  // Alerter

/**
 * Used to keep track of what action we are currently taking
 */
enum class TrackingState { RETURNING_TO_START, TRACKING, PATROLLING };

/**
 * Constrain possible inner types to only PathAgent and AStarAgent to avoid errors
 * @tparam T either PathAgent or AStarAgent
 */
template <typename T>
concept TrackingAgentInner_Type = std::is_same_v<PathAgent, T> || std::is_same_v<AStarAgent, T>;

/**
 * Agent that switches between user-defined custom movement pattern and
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

  /// How close the goal_pos needs to be to begin tracking
  double tracking_distance_ = 50;

  /// Alerter that this tracking agent belongs to; Null implies that it's not associated with an
  /// alerter
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
  TrackingAgent(size_t id, std::string const &name)
      : cse491::AgentBase(id, name), inner_(std::in_place_type<PathAgent>, id, name)
  {
  }

  /**
   * Constructor (vector)
   * @param id unique agent id
   * @param name name of path agent
   * @param offsets collection of offsets to move the agent
   * @param alerter alerter network to add agent to
   * @attention The sequence of offsets must not be empty
   * @attention alerter should be a nullptr if this tracker is not part of any group of tracking
   * agents
   */
  TrackingAgent(size_t id, std::string const &name, std::vector<cse491::GridPosition> &&offsets,
                std::shared_ptr<Alerter> &&alerter = nullptr)
      : cse491::AgentBase(id, name),
        inner_(std::in_place_type<PathAgent>, id, name, offsets),
        offsets_(offsets),
        alerter_(alerter)
  {
  }

  /**
   * Constructor (string view)
   * @param id unique agent id
   * @param name name of path agent
   * @param commands sequence of commands to be interpreted as offsets
   * @param alerter alerter network to add agent to
   * @attention The sequence of offsets must not be empty
   * @attention alerter should be a nullptr if this tracker is not part of any group of tracking
   * agents
   */
  TrackingAgent(size_t id, std::string const &name, std::string_view commands,
                std::shared_ptr<Alerter> &&alerter = nullptr)
      : cse491::AgentBase(id, name),
        inner_(std::in_place_type<PathAgent>, id, name, commands),
        offsets_(StrToOffsets(commands)),
        alerter_(alerter)
  {
  }

  /**
   * Destructor
   */
  ~TrackingAgent() override { RemoveFromAlerter(); }

  /**
   * Ensure that the TrackingAgent's internal PathAgent is correctly initialized
   * Verifies that it can currently index into a valid offset
   * @return true if so; false otherwise
   */
  bool Initialize() override
  {
    SetStartPosition(GetPosition());
    if (property_map.contains("path")) {
      auto view = GetProperty<std::basic_string_view<char>>("path");
      offsets_ = StrToOffsets(view);
      std::get<PathAgent>(inner_).SetProperty("path", view);
      std::get<PathAgent>(inner_).SetWorld(GetWorld());
      std::get<PathAgent>(inner_).SetPosition(GetPosition());

      if (property_map.contains("alerter")) {
        auto alerter_property = GetProperty<std::shared_ptr<Alerter>>("alerter");
        AddToAlerter(alerter_property);
      }
      return std::get<PathAgent>(inner_).Initialize();
    }
    return false;
  }

  /// Creates an alerter network and adds this tracking agent to it
  void MakeAlerter() { alerter_ = std::make_shared<Alerter>(&GetWorld(), id); }

  /**
   * Adds this tracking agent to an already-existing alerter network
   * @param alerter alerter that this agent should be associated with
   * @note alerter must not be null
   */
  void AddToAlerter(std::shared_ptr<Alerter> alerter)
  {
    assert(alerter != nullptr);
    alerter->AddAgent(id);
    alerter_ = alerter;
  }

  /**
   * Removes this tracking agent from it's own tracking network
   * @note called from the TrackingAgent destructor
   */
  void RemoveFromAlerter()
  {
    if (alerter_ != nullptr) {
      alerter_->RemoveAgent(id);
      alerter_ = nullptr;
    }
  }

  /**
   * Used to expand the alerter network by adding other tracking agents to it
   * @return a copy of this tracking agent's alerter
   * @note it's expected that this function is used when calling AddToAlerter on a different
   * tracking agent
   * @note may be null
   */
  [[nodiscard]] std::shared_ptr<Alerter> GetAlerter() const { return alerter_; }

  /// Tells the alerter to notify all other tracking agents in network
  void CallAlerter(size_t agent_id)
  {
    if (alerter_ != nullptr) {
      alerter_->AlertAllTrackingAgents(agent_id);
    }
  }

  /**
   * Handles focusing the agent onto a goal_pos, returning it to its original location, and
   * patrolling
   * @param alerting determines whether this agent should alert all other TrackingAgents in its
   * network when its goal_pos comes into range
   * @note the inner variant type will be AStarAgent when tracking OR returning to a location, but
   * PathAgent when patrolling
   */
  void UpdateState(bool alerting = true)
  {
    SetPosition(std::visit(
        [](TrackingAgentInner_Type auto const &agent) { return agent.GetPosition(); }, inner_));
    switch (state_) {
      // Tracking can transition only to Returning
      case TrackingState::TRACKING: {
        // Reached goal position
        if (GetPosition() == std::get<AStarAgent>(inner_).GetGoalPosition()) {
          if (target_ != nullptr &&
              GetPosition().Distance(target_->GetPosition()) < tracking_distance_) {
            // Target is still in range of goal position so
            std::get<AStarAgent>(inner_).SetGoalPosition(target_->GetPosition());

            // Alert all trackers
            if (alerting) {
              CallAlerter(id);
            }
          } else {
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
        if (target_ != nullptr &&
            GetPosition().Distance(target_->GetPosition()) < tracking_distance_) {
          state_ = TrackingState::TRACKING;
          std::get<AStarAgent>(inner_).SetGoalPosition(target_->GetPosition());
          std::get<AStarAgent>(inner_).RecalculatePath();
          std::get<AStarAgent>(inner_).SetActionResult(1);

          // Alert other trackers
          if (alerting) {
            CallAlerter(id);
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
        if (target_ != nullptr &&
            GetPosition().Distance(target_->GetPosition()) < tracking_distance_) {
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
          if (alerting) {
            CallAlerter(id);
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
  [[nodiscard]] cse491::GridPosition GetNextPosition() override
  {
    auto pos = std::get<PathAgent>(inner_).GetNextPosition();
    std::get<PathAgent>(inner_).SetPosition(pos);
    return pos;
  }

  /// Select action for PathAgent inner type
  size_t SelectInnerAction(PathAgent &agent, cse491::WorldGrid const &grid,
                           cse491::type_options_t const &type, cse491::item_map_t const &item_set,
                           cse491::agent_map_t const &agent_set)
  {
    return agent.SelectAction(grid, type, item_set, agent_set);
  }

  /// Select action for AStarAgent inner type
  size_t SelectInnerAction(AStarAgent &agent, cse491::WorldGrid const &grid,
                           cse491::type_options_t const &type, cse491::item_map_t const &item_set,
                           cse491::agent_map_t const &agent_set)
  {
    auto next_pos = agent.GetNextPosition();
    auto res = agent.SelectAction(grid, type, item_set, agent_set);
    agent.SetPosition(next_pos);
    return res;
  }

  /// Updates the internal state of the TrackingAgent and calls the internal agent's select action
  /// method
  size_t SelectAction(cse491::WorldGrid const &grid, cse491::type_options_t const &type,
                      cse491::item_map_t const &item_set,
                      cse491::agent_map_t const &agent_set) override
  {
    UpdateState();
    return std::visit(
        [&](TrackingAgentInner_Type auto &agent) {
          return SelectInnerAction(agent, grid, type, item_set, agent_set);
        },
        inner_);
  }

  /**
   * Set where this agent "patrol area" starts
   * @param gp grid position of position
   * @return self
   */
  TrackingAgent &SetStartPosition(cse491::GridPosition pos)
  {
    start_pos_ = pos;
    return *this;
  }

  /**
   * Set where this agent "patrol area" starts
   * @param x x-coordinate of start pos
   * @param y y-coordinate of start pos
   * @return self
   */
  TrackingAgent &SetStartPosition(double x, double y)
  {
    start_pos_ = cse491::GridPosition(x, y);
    return *this;
  }

  /**
   * Set which agent we are following
   * @param agent we want to track
   * @return self
   */
  TrackingAgent &SetTarget(Entity *agent)
  {
    target_ = agent;
    return *this;
  }

  /**
   * Get the distance around this tracker that it surveys
   * @return tracking distance
   */
  [[nodiscard]] double GetTrackingDistance() const { return tracking_distance_; }

  /**
   * Set how close goal_pos has to be to start tracking
   * @param dist to start tracking at
   * @return calling object
   */
  TrackingAgent &SetTrackingDistance(double dist)
  {
    tracking_distance_ = dist;
    return *this;
  }

  /**
   * Set both the world for the current agent and the agents it is a part of
   * @param in_world new world to associate the agent with
   * @return calling agent
   */
  TrackingAgent &SetWorld(cse491::WorldBase &in_world) override
  {
    Entity::SetWorld(in_world);
    std::visit(
        [&in_world](TrackingAgentInner_Type auto &agent) {
          agent.SetWorld(in_world);
          std::as_const(in_world).ConfigAgent(agent);
        },
        inner_);
    return *this;
  }

  /**
   * Retrieves the current internal state of the Tracking Agent
   * @return current state
   */
  TrackingState GetState() { return state_; }

  /**
   * Sets the patrolling path of the TrackingAgent
   * @param offsets grid position offsets creating the path
   * @return
   */
  TrackingAgent &SetPath(std::vector<cse491::GridPosition> offsets)
  {
    offsets_ = std::move(offsets);
    if (offsets_.empty()) {
      std::ostringstream what;
      what << "TrackingAgent cannot have empty path. If you meant to make the agent stay still, "
              "use \"x\"";
      throw std::invalid_argument(what.str());
    }
    return *this;
  }

  /**
   * Sets the patrolling path of the TrackingAgent
   * @param offsets grid position offsets creating the path
   * @return
   */
  TrackingAgent &SetPath(std::string_view offsets) { return SetPath(StrToOffsets(offsets)); }

  /**
   * Returns an immutable reference to this agent's current path
   * @return sequence of offsets
   */
  std::vector<cse491::GridPosition> const &GetPath() const { return offsets_; }

  /**
   * Returns an immutable pointer to this agent's target
   * @return ptr to entity
   */
  cse491::Entity const *GetTarget() const { return target_; }

};  // TrackingAgent

/**
 * Alerter constructor (only knows its world and has no agents in its network)
 * @param world_ptr the world this alerter is associated with
 */
Alerter::Alerter(cse491::WorldBase *world_ptr) : world_ptr_(world_ptr)
{
  assert(world_ptr != nullptr);
}

/**
 * Alerter constructor (adds the TrackingAgent with that id to the alerter network)
 * @param id id of agent to be added
 * @param world_ptr the world this alerter is associated with
 */
Alerter::Alerter(cse491::WorldBase *world_ptr, size_t id) : world_ptr_(world_ptr)
{
  assert(world_ptr != nullptr);
  AddAgent(id);
}

/**
 * Adds a TrackingAgent to the network
 * @param id id of agent to be added
 */
void Alerter::AddAgent(size_t id)
{
  // Note: GetAgent already handles checking that the agent exists, but we must type-check
  assert(dynamic_cast<TrackingAgent *>(&(world_ptr_->GetAgent(id))) != nullptr);
  agent_network_set_.insert(id);
}

/**
 * Removes a TrackingAgent to the network
 * @param id id of agent to be removed
 * @note no assertions here since we may want to allow the TrackingAgent to join another network
 * later if it's not being destructed
 */
void Alerter::RemoveAgent(size_t id) { agent_network_set_.erase(id); }

/**
 * Uses UpdateState to focus all trackers onto their goal_pos regardless of the distance away
 * @param caller_id the original id of the agent that came into range of its goal_pos; does not need
 * to be updated
 */
void Alerter::AlertAllTrackingAgents(size_t caller_id)
{
  for (auto id : agent_network_set_) {
    // Do not update the caller who gave the alert
    if (caller_id == id) {
      continue;
    }
    auto &tracking_agent = DownCastAgent<TrackingAgent>(world_ptr_->GetAgent(id));
    // UpdateState sets an agent's TrackingState to TRACKING if it is within some distance of the
    // goal_pos if this distance is positive infinity, then the state will always be reset (given
    // that there IS a goal_pos) Important: UpdateState must be called with alerting == false in
    // order to avoid infinite recursion from recursively calling AlertAllTrackingAgents
    double old_tracking_dist = tracking_agent.GetTrackingDistance();
    tracking_agent.SetTrackingDistance(std::numeric_limits<double>::infinity());
    tracking_agent.UpdateState(false);
    tracking_agent.SetTrackingDistance(old_tracking_dist);
  }
}

}  // namespace walle
