/**
 * This file is part of the Fall 2023, CSE 491 Course Project - Group 1 Fork - yousif_m Branch.
 * @brief An Agent that will move around and perform actions.
 * @note Status: PROPOSAL
 * @author Yousif Murrani
 **/

#pragma once

#include <cassert>
#include "../core/AgentBase.hpp"
#include "../core/GridPosition.hpp"


namespace walle {

    class FollowingAgent : public cse491::AgentBase {

        protected:

            bool mMoving = false;              // is the agent currently in route
            bool mAlive = true;                // is this agent still alive

            double mSpeed = 10.0;                    // could use this to determine how fast the agent is moving
            double mHealth = 20.0;                   // an agents health, could be used if we add an attack action
            double mDamage = 5.0;                    // how much damage the agent can do

            int mLevel = 5;                     // agents level, can be used to decide difficulty
            std::vector<cse491::GridPosition> mPath;    // vector of positions on map for agent to follow (from A* Star)


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
            void TakeDamage(double damage)
            {
                mHealth -= damage;
                if(mHealth <= 0.0){
                    mAlive = false;
                }
            }

            bool isAlive() const { return mAlive; }                     /// function to return the agents current life status
            double GetHealth() const { return mHealth; }                   /// function to get the agents health
            cse491::GridPosition GetPosition() const { return Entity::GetPosition(); }  /// function to get the agents current position

            void SetHealth(double hp) { mHealth = hp; } /// function to set the agents health

            /// Choose the action to take a step in the appropriate direction.
            size_t SelectAction(const cse491::WorldGrid & /* grid*/,
                                const cse491::type_options_t & /* type_options*/,
                                const cse491::item_set_t & /* item_set*/,
                                const cse491::agent_set_t & /* agent_set*/) override
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
//                    if(GetPosition().GetY() < pos.GetY())
//                    {
//                        return action_map["up"];
//                    }
//                    else
//                    {
//                        return action_map["down"];
//                    }
                }

            }

            /// @brief Function to track an agent 
            /// @return GridPosition
//            void Track(cse491::GridPosition destination)
//            {
//                // use A* Star to fill out the mPath
//                return;
//            }

    };

}