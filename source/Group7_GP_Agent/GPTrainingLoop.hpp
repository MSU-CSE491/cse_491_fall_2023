#include <vector>

#pragma once

#include "../core/AgentBase.hpp"
#include "../core/WorldBase.hpp"
#include "../Group7_GP_Agent/GPAgent.hpp"

namespace cowboys
{

template<class AgentType, class EnvironmentType>
class GPTrainingLoop
{
private:
//        cse491::AgentBase& agent;
//        cse491::WorldBase& environment;

    std::vector<cse491::WorldBase *> environments;
    std::vector<std::vector<cse491::AgentBase *>> agents;

public:

    GPTrainingLoop() {}

    void initialize(size_t numArenas = 5, size_t NumAgentsForArena = 100)
    {

//            static_assert(std::is_base_of<cse491::AgentBase, AgentType>::value, "AgentType must be derived from cse491::AgentBase");
//            static_assert(std::is_base_of<cse491::WorldBase, EnvironmentType>::value, "EnvironmentType must be derived from cse491::WorldBase");

        for(size_t i = 0; i < numArenas; ++i)
        {
            // instantiate a new environment
            environments.emplace_back(new EnvironmentType());

            agents.emplace_back(std::vector<cse491::AgentBase *>());
            for(size_t j = 0; j < NumAgentsForArena; ++j)
            {

                cse491::AgentBase
                    &addedAgent = environments[i]->template AddAgent<AgentType>("Agent " + std::to_string(j));
                agents[i].emplace_back(&addedAgent);
            }

        }

    }

    void run(size_t numGenerations,
             size_t numberOfTurns = 100)
    {

        // number of arenas

        for(size_t generation = 0; generation < numGenerations; ++generation)
        {
            for(size_t arena = 0; arena < environments.size(); ++arena)
            {
                for(size_t turn = 0; turn < numberOfTurns; turn++)
                {
                    environments[arena]->RunAgents();
                    environments[arena]->UpdateWorld();
                }
            }

        }
    }

    ~GPTrainingLoop() = default;

};
}