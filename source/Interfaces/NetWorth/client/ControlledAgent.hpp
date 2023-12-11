/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief An Agent that will walk back and forth along a row or column.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <cassert>

#include "../../../core/AgentBase.hpp"
#include "ClientManager.hpp"

namespace netWorth
{

	class ControlledAgent : public cse491::AgentBase
	{
	private:
		ClientManager* m_manager = nullptr;       /// Client manager to access agent action map

	protected:

	public:
		ControlledAgent(size_t id, const std::string& name) : AgentBase(id, name)
		{
		}

		/// @brief This agent needs a specific set of actions to function.
		/// @return Success.
		bool Initialize() override
		{
			m_manager = GetProperty<ClientManager*>("manager");
			return HasAction("up") && HasAction("down") && HasAction("left") && HasAction("right");
		}

		/// Choose the action to take a step in the appropriate direction.
		size_t SelectAction(const cse491::WorldGrid& /* grid*/,
			const cse491::type_options_t& /* type_options*/,
			const cse491::item_map_t& /* item_map*/,
			const cse491::agent_map_t& /* agent_map*/) override
		{
			if (!m_manager->iDPresent(id))
			{
				return 0;
			}
			return m_manager->getActionID(id);
		}

	};

} // End of namespace cse491
