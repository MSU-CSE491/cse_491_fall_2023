/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A prototype agent that is used to send data across a network to a server world.
 * @note Status: PROTOTYPE
 **/

#pragma once

#include <cassert>
#include <string>
#include <SFML/Network.hpp>
#include "../../../Agents/PacingAgent.hpp"

using namespace sf;

namespace cse491 {
    namespace netWorth{
        class NetworkingAgent : public PacingAgent {
        private:
            // UdpSocket mClientSocket;

        protected:
            bool vertical=true; ///< Is this agent moving down&up?  False = right&left.
            bool reverse=false;  ///< Is this agent on their way back? (up/left?)

        public:
            NetworkingAgent(size_t id, const std::string & name) : PacingAgent(id, name) { }
            ~NetworkingAgent() = default;

            /// Choose the action to take a step in the appropriate direction.
            size_t SelectAction(const WorldGrid & /* grid*/,
                                const type_options_t & /* type_options*/,
                                const item_set_t & /* item_set*/,
                                const agent_set_t & /* agent_set*/) override
            {
            // If the last step failed, try going in the other direction.
            if (action_result == 0) {
                reverse = !reverse;
            }
            // Take as tep in the direction we are trying to go in.
            if (vertical) {
                if (reverse) return action_map["up"];
                else         return action_map["down"];
            } else {
                if (reverse) return action_map["left"];
                else         return action_map["right"];
            }
            return 0;  // Should never actually get here...
            }


            void CreatePacket(std::string action){
                //Creating packet to send to server of actions
                Packet packet;
                size_t action_num = action_map[action];
                packet << static_cast<int>(action_num);
            }


            void SendPacket(){
                //We'll do this in a sec
            }

        };
    }
  

} // End of namespace cse491