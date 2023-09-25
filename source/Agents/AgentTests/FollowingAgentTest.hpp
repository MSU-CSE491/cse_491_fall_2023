#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../FollowingAgent.hpp"
#include "../../core/GridPosition.hpp"

TEST_CASE( "Following Agent", "[agent]" ) {

    cse491::FollowingAgent agent1(1, "Following Agent 1");
    agent1.SetPosition(1.0, 1.0);

    REQUIRE( agent1.GetPosition().GetX() == 1.0 );
    REQUIRE( agent1.GetPosition().GetY() == 1.0 );

}