/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A low-end text interface providing a bare-minimum level of interaction.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <iostream>
#include <vector>

#include "../../../core/Data.hpp"
#include "../../../core/InterfaceBase.hpp"
#include <SFML/Network/UdpSocket.hpp>

namespace NetWorth {

class ServerPlayerInterface : public cse491::InterfaceBase {
    private:

    protected:

    public:
        ServerPlayerInterface(size_t id, const std::string & name) : InterfaceBase(id, name) {
            SetProperty("isPlayer", true);
        }

        ~ServerPlayerInterface() = default; // Already virtual from Entity

        // -- AgentBase overrides --

        bool Initialize() override {
            return true;
        }

        size_t SelectAction(const cse491::WorldGrid & grid,
                            const cse491::type_options_t & type_options,
                            const cse491::item_set_t & item_set,
                            const cse491::agent_set_t & agent_set) override
        {
            // Do the action!
            return 0;
        }

    };

} // End of namespace NetWorth
