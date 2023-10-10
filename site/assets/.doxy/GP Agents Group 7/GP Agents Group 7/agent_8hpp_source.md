

# File agent.hpp

[**File List**](files.md) **>** [**Group7\_GP\_Agent**](dir_2917a1bbf314837e5575b308b680803f.md) **>** [**agent.hpp**](agent_8hpp.md)

[Go to the documentation of this file](agent_8hpp.md)

```C++


#pragma once

#include <cassert>
#include <map>
#include <string>
#include <iostream>

#include <random>

#include "../core/AgentBase.hpp"

namespace cowboys
{


    class GPAgent : public cse491::AgentBase
    {
    protected:
        const std::vector<std::string> predefinedMovement = {"down", "down","down","down", "right", "right", "up", "up", "up","up", "right", "right",  "right",  "right",  "right",  "right",  "right", "right", "right",  "right", "right", "left", "left", "left"};

        size_t movementIndex = 0; // current move of the agent

    public:
        GPAgent(size_t id, const std::string &name) : AgentBase(id, name) {
            std::random_device rd;
            std::mt19937 gen(rd());


            std::uniform_int_distribution<int> dist(0, 3); // size of the random set to 3 as we only have 4 actions initally
        }
        ~GPAgent() override = default;

        bool Initialize() override
        {
            return true;
        }


        size_t SelectAction(const cse491::WorldGrid &grid,
                            const cse491::type_options_t &type_options,
                            const cse491::item_set_t &item_set,
                            const cse491::agent_set_t &agent_set) override
        {
            if (movementIndex >= predefinedMovement.size()){
                return action_map["right"]; // do nothing if it is out of bound for defined movement
            }

            auto action = action_map[predefinedMovement[movementIndex++]];
            return action;
        }
    };

}

```

