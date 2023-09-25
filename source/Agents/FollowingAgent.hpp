/**
 * This file is part of the Fall 2023, CSE 491 Course Project - Group 1 Fork - yousif_m Branch.
 * @brief An Agent that will move around and perform actions.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <cassert>
#include "../core/AgentBase.hpp"
#include "../core/GridPosition.hpp"

using namespace cse491;

namespace walle {

    class FollowingAgent : public AgentBase {

        protected:

            bool mMoving = false;              // is the agent currently in route
            bool mAlive = true;                // is this agent still alive

            int mSpeed = 10;                    // could use this to determine how fast the agent is moving
            int mHealth = 20;                   // an agents health, could be used if we add an attack action
            int mDamage = 5;                    // how much damage the agent can do

            int mLevel = 5;                     // agents level, can be used to decide difficulty
            std::vector<GridPosition> mPath;    // vector of positions on map for agent to follow (from A* Star)


        public:
            FollowingAgent(size_t id, const std::string & name) : AgentBase(id, name) { }
            ~FollowingAgent() = default;

            /// @brief This agent needs a specific set of actions to function.
            /// @return Success.
            bool Initialize() override 
            {
                return HasAction("up") && HasAction("down") && HasAction("left") && HasAction("right");
                // could add HasAction("attack") and HasAction("track")
            }

            /// function to take damage, update health and update the agents life status
            void TakeDamage(int damage)
            {
                mHealth -= damage;
                if(mHealth <= 0){
                    mAlive = false;
                }
            }

            bool isAlive() const { return mAlive; }                     /// function to return the agents current life status
            int GetHealth() const { return mHealth; }                   /// function to get the agents health
            GridPosition GetPosition() const { return GetPosition(); }  /// function to get the agents current position

            void SetHealth(int hp) { mHealth = hp; } /// function to set the agents health

            /// Choose the action to take a step in the appropriate direction.
            size_t SelectAction(const WorldGrid & /* grid*/,
                                const type_options_t & /* type_options*/,
                                const item_set_t & /* item_set*/,
                                const agent_set_t & /* agent_set*/) override 
            {
            
                // call track function and begin moving towards target using the mPath member variable

                for(auto pos : mPath)
                {
                    // adjust current x to the x of the next point in the path
                    if(GetPosition().GetX() < pos.GetX())
                    {
                        return action_map["right"];
                    }
                    else
                    {
                        return action_map["left"];
                    }

                    // adjust current y to the y of the next point in the path
                    if(GetPosition().GetY() < pos.GetY())
                    {
                        return action_map["up"];
                    }
                    else
                    {
                        return action_map["down"];
                    }
                }

            }

            /// @brief Function to track an agent 
            /// @return GridPosition
            void Track(GridPosition destination)
            {
                // use A* Star to fill out the mPath
            }

    };

}