/**
 * This file is part of the Fall 2023, CSE 491 Course Project - Group 1 Fork - yousif_m Branch.
 * @brief An Inventory class that holds items for the Agent
 * @note Status: PROPOSAL
 * @author Yousif Murrani
 **/

#pragma once

#include <vector>

#include "AgentBase.hpp"

namespace walle {

    class Inventory {

    private:
        // Agent this inventory is a part of
        cse491::AgentBase* mAgent = nullptr;

        // amount of items the agent currently has
        int mItemCount = 0;


    public:
        Inventory(cse491::AgentBase* agent) : mAgent(agent) {}

        ~Inventory() = default;


    };

}
