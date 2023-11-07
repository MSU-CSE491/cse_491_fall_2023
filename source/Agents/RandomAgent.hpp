/**
 * This file is part of the Fall 2023, CSE 491 Course Project - Group 1 Fork - yousif_m Branch.
 * @brief An Agent that will move around using random actions
 * @note Status: PROPOSAL
 * @author Yousif Murrani
 **/

#pragma once

#include <cassert>
#include "../core/AgentBase.hpp"
#include "../core/GridPosition.hpp"
#include "../core/WorldBase.hpp"

namespace walle {

/**
 * Class for the Random Agent
 */
class RandomAgent : public cse491::AgentBase {

	private:

	public:
		RandomAgent(size_t id, const std::string &name) : AgentBase(id, name) {
		}

		~RandomAgent() = default;

		/// @brief This agent needs a specific set of actions to function.
		/// @return Success.
		bool Initialize() override {
			return HasAction("up") && HasAction("down") && HasAction("left") && HasAction("right");
		}

	/// Choose the action to take a step in the random direction
	size_t SelectAction(const cse491::WorldGrid & /*grid*/,
						const cse491::type_options_t & /* type_options*/,
						const cse491::item_map_t & /* item_map*/,
						const cse491::agent_map_t & /* agent_map*/) override {
		// We are taking an action so another turn has passed

		auto random_num = GetWorld().GetRandom(4);

		if(random_num < 1.0){
			return action_map["up"];
		}
		else if(random_num < 2.0){
			return action_map["down"];
		}
		else if(random_num < 3.0){
			return action_map["left"];
		}
		else{
			return action_map["right"];
		}

		return 0; // should not reach this point
	}

};

}