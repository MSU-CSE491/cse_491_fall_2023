
#pragma once

#include "../core/AgentBase.hpp"
#include "../core/WorldBase.hpp"
#include "GPAgent.hpp"
#include "GPAgent_.hpp"


#include "CGPAgent.hpp"

#include <thread>
#include <iostream>
#include <vector>
#include <ranges>
//#include <algorithm>


#ifdef __APPLE__
#include <Metal/Metal.h>
#endif


namespace cowboys
{

template<class AgentType, class EnvironmentType>
class GPTrainingLoop
{
private:
//        cse491::AgentBase& agent;
//        cse491::WorldBase& environment;

    std::vector<cse491::WorldBase *> environments;
    std::vector<std::vector<cowboys::GPAgent_ *>> agents;


    std::vector<std::vector<cse491::GridPosition>> TEMPinitialAgentPositions ;


    std::vector<std::vector<double>> TEMPAgentFitness ;

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

            agents.emplace_back(std::vector<cowboys::GPAgent_ *>());
            TEMPinitialAgentPositions.emplace_back(std::vector<cse491::GridPosition>());
            for(size_t j = 0; j < NumAgentsForArena; ++j)
            {

                cowboys::GPAgent_ &addedAgent = static_cast<cowboys::GPAgent_ &>(environments[i]->template AddAgent<AgentType>("Agent " + std::to_string(j)));
                addedAgent.SetPosition(0, 0);
                cse491::GridPosition position = addedAgent.GetPosition();

                TEMPinitialAgentPositions[i].emplace_back(position);

                agents[i].emplace_back(&addedAgent);

            }

        }

    }


    double simpleFitnessFunction(cse491::AgentBase &agent, cse491::GridPosition startPosition)
    {

      cse491::GridPosition currentPosition = agent.GetPosition();

      // Eucledian distance
      double distance = std::sqrt(std::pow(currentPosition.GetX() - startPosition.GetX(), 2) + std::pow(currentPosition.GetY() - startPosition.GetY(), 2));

      return distance;
    }



    void ComputeFitness() {

    }

    void run(size_t numGenerations,
             size_t numberOfTurns = 100,
             size_t maxThreads = 0)
    {

        // number of arenas



        for(size_t generation = 0; generation < numGenerations; ++generation)
        {

            std::vector<std::thread> threads;

            for (size_t arena = 0; arena < environments.size(); ++arena)
            {
                if (maxThreads == 0 || threads.size() < maxThreads)
                {
                    threads.emplace_back(&GPTrainingLoop::runArena, this, arena, numberOfTurns);
                }
                else
                {
                    // Wait for one of the existing threads to finish
                    threads[0].join();
                    threads.erase(threads.begin());
                    threads.emplace_back(&GPTrainingLoop::runArena, this, arena, numberOfTurns);
                }
            }

            // Wait for all threads to finish
            for (auto& thread : threads)
            {
                thread.join();
                //progress for the threads
                std::cout << ".";

            }


            // calculate fitness
            for(size_t arena = 0; arena < environments.size(); ++arena)
            {
              TEMPAgentFitness.emplace_back(std::vector<double>());
              for(size_t a = 0; a < agents[arena].size(); ++a)
                {
                    double fitness = simpleFitnessFunction(*agents[arena][a], TEMPinitialAgentPositions[arena][a]);
                    TEMPAgentFitness[arena].push_back(fitness);
                }
            }

            // print average fitness
            double averageFitness = 0;
            double maxFitness = 0;

            std::pair<int, int> bestAgent = std::make_pair(-1, -1);
            for(size_t arena = 0; arena < environments.size(); ++arena)
            {
              for(size_t a = 0; a < agents[arena].size(); ++a)
                {
                    averageFitness += TEMPAgentFitness[arena][a];
                    if(TEMPAgentFitness[arena][a] > maxFitness) {
                      maxFitness = TEMPAgentFitness[arena][a];
                      bestAgent = std::make_pair(arena, a);
                    }
                }
            }

            averageFitness /= (environments.size() * agents[0].size());

            std::cout << "Generation " << generation << " complete" << std::endl;
            std::cout << "Average fitness: " << averageFitness  << " ";
            std::cout << "Max fitness: " << maxFitness << std::endl;
            std::cout << "Best agent: AGENT[" << bestAgent.first << "," << bestAgent.second << "] "<< std::endl;
            cse491::GridPosition bestAgentPosition = agents[bestAgent.first][bestAgent.second]->GetPosition();
            std::cout << "Best Agent Final Position: " << bestAgentPosition.GetX()  << "," << bestAgentPosition.GetY()  << std::endl;
            std::cout << "------------------------------------------------------------------" << std::endl;


            GpLoopMutateHelper();


          resetEnvironments();

          std::cout << "  ========= =========" << std::endl;

        }
    }


    void GpLoopMutateHelper(){

      constexpr double ELITE_POPULATION_PERCENT = 0.2;
      constexpr double UNFIT_POPULATION_PERCENT = 0.05;

      // sort based on fitness function
      std::vector<std::pair<int, int>> sortedAgents = std::vector<std::pair<int, int>>();

      for(size_t arena = 0; arena < environments.size(); ++arena)
      {
        for(size_t a = 0; a < agents[arena].size(); ++a)
        {
          sortedAgents.push_back(std::make_pair(arena, a));
        }
      }
      const int ELITE_POPULATION_SIZE = int(ELITE_POPULATION_PERCENT * sortedAgents.size());


      double averageEliteFitness = 0;
      for(int i = 0; i < ELITE_POPULATION_SIZE; i++){
        auto [arenaIDX, agentIDX] = sortedAgents[i];
        averageEliteFitness += TEMPAgentFitness[arenaIDX][agentIDX];
      }

      std::cout << " --- average elite percent " << averageEliteFitness << "------ " << std::endl;


      std::sort(sortedAgents.begin(), sortedAgents.end(), [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
          return TEMPAgentFitness[a.first][a.second] < TEMPAgentFitness[b.first][b.second];
      });



      const int MIDDLE_MUTATE_ENDBOUND = int(sortedAgents.size() * ( 1 - UNFIT_POPULATION_PERCENT));
      const int MIDDLE_MUTATE_STARTBOUND = int(ELITE_POPULATION_PERCENT * sortedAgents.size());


      for(int i = MIDDLE_MUTATE_STARTBOUND; i < MIDDLE_MUTATE_ENDBOUND; i++){
        auto [arenaIDX, agentIDX] = sortedAgents[i];
        agents[arenaIDX][agentIDX]->MutateAgent(0.6);

        if ( i % (sortedAgents.size() / 10) == 0){
          std::cout << " --- mutation " << i << " complete " << (i * 1.0/ sortedAgents.size()) << std::endl;
        }
      }

//      int unfitAgents = int(sortedAgents.size() * UNFIT_POPULATION_PERCENT);
      for(size_t i = MIDDLE_MUTATE_ENDBOUND; i < sortedAgents.size(); i++){
        auto [arenaIDX, agentIDX] = sortedAgents[i];
//        auto eliteINDEX = rand() % ELITE_POPULATION_SIZE;

        agents[arenaIDX][agentIDX]->MutateAgent(0.9);
      }


      std::cout << " --- mutation complete --- " << std::endl;
    }


    void resetEnvironments()
    {

      for (size_t arena = 0; arena < environments.size(); ++arena)
      {
        for (size_t a = 0; a < agents[arena].size(); ++a)
        {
          agents[arena][a]->SetPosition(TEMPinitialAgentPositions[arena][a]);
        }
      }

      TEMPAgentFitness.clear();
    }

    void runArena(size_t arena, size_t numberOfTurns)
    {
        for (size_t turn = 0; turn < numberOfTurns; turn++)
        {
            environments[arena]->RunAgents();
            environments[arena]->UpdateWorld();
        }
    }

    ~GPTrainingLoop() = default;

};
}