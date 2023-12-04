/**
 * @file TrackingAgent.cpp
 * @author David Rackerby
 */

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Agents/TrackingAgent.hpp"
#include "Worlds/MazeWorld.hpp"

#include <memory>

using namespace std;
using namespace walle;
using namespace cse491;

TEST_CASE("Tracking Agent Alert", "[Agents]") {

  // Avoid all the hurdles of encapsulation and just add the agents directly
  struct MockWorld : MazeWorld {
    int DoAction(cse491::AgentBase &, size_t) override { return 0; }
    agent_map_t & GetMap() { return agent_map; }
  };

  SECTION("Alerter network") {
    MockWorld world;
    Entity entity {0, "mock"};
    auto & map = world.GetMap();
    auto & first = map[0] = make_unique<TrackingAgent>(0, "first", "x");
    auto & second = map[1] = make_unique<TrackingAgent>(1, "second", "x");
    first->SetWorld(world);
    second->SetWorld(world);

    auto & first_tracking = dynamic_cast<TrackingAgent&>(*first);
    auto & second_tracking = dynamic_cast<TrackingAgent&>(*second);

    first_tracking.SetTarget(&entity);
    second_tracking.SetTarget(&entity);

    REQUIRE(first_tracking.GetAlerter() == nullptr);

    first_tracking.MakeAlerter();

    REQUIRE(first_tracking.GetAlerter() != nullptr);

    second_tracking.AddToAlerter(first_tracking.GetAlerter());

    REQUIRE(first_tracking.GetState() == TrackingState::PATROLLING);
    REQUIRE(second_tracking.GetState() == TrackingState::PATROLLING);

    // Call with an ID that is neither of the two above
    first_tracking.GetAlerter()->AlertAllTrackingAgents(42);

    REQUIRE(first_tracking.GetState() == TrackingState::TRACKING);
    REQUIRE(second_tracking.GetState() == TrackingState::TRACKING);
  }
}

