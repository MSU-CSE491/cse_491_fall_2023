
#pragma once

#include "../../core/AgentBase.hpp"
#include "../../core/WorldBase.hpp"
//#include "GPAgent.hpp"
#include "GPAgentBase.hpp"


#include "CGPAgent.hpp"

#include <thread>
#include <iostream>
#include <vector>
#include <ranges>
#include <cmath>
#include <filesystem>
#include <string>
#include <sstream>
#include <chrono>


#include "tinyxml2.h"
//#include <algorithm>


namespace cowboys {

    constexpr unsigned int TRAINING_SEED = 0; ///< If this is 0, then a random seed will be used

    template<class AgentType, class EnvironmentType>
    class GPTrainingLoop {
    private:

//        std::vector<cse491::WorldBase *> environments;
        std::vector<std::unique_ptr<cse491::WorldBase>> environments;
        std::vector<std::vector<cowboys::GPAgentBase *>> agents;


//        std::vector<std::vector<std::vector<cse491::GridPosition>>> TEMPinitialAgentPositions;


        std::vector<std::vector<double>> TEMPAgentFitness;
        std::vector<std::vector<cse491::GridPosition>> TEMPinitialAgentPositions;

        tinyxml2::XMLDocument doc;
        tinyxml2::XMLDocument lastGenerationsDoc;


        tinyxml2::XMLElement *root = doc.NewElement("GPLoop");


        tinyxml2::XMLElement *rootTEMP = lastGenerationsDoc.NewElement("GPLoop");

        std::vector<std::pair<int, int>> sortedAgents = std::vector<std::pair<int, int>>();

//        const std::vector<cse491::GridPosition> STARTPOSITIONS = {cse491::GridPosition(0,0), cse491::GridPosition(4,4), cse491::GridPosition(19,5), cse491::GridPosition(20,1) };
        const std::vector<cse491::GridPosition> STARTPOSITIONS = {cse491::GridPosition(0, 0)};

    public:

        GPTrainingLoop() {

          doc.InsertFirstChild(root);
          ResetMainTagLastGenerations();

        }

        void ResetMainTagLastGenerations() {
          rootTEMP = lastGenerationsDoc.NewElement("GPLoop");
          lastGenerationsDoc.InsertFirstChild(rootTEMP);
        }

        /**
         * @brief Initialize the training loop with a number of environments and agents per environment.
         * @param numArenas
         * @param NumAgentsForArena
         */
        void Initialize(size_t numArenas = 5, size_t NumAgentsForArena = 100) {

          unsigned int seed = TRAINING_SEED;
          if (seed == 0) {
            seed = std::random_device()();
          }
          std::cout << "Using seed: " << seed << std::endl;


          for (size_t i = 0; i < numArenas; ++i) {
            // instantiate a new environment
            environments.emplace_back(std::make_unique<EnvironmentType>(seed));


            agents.push_back(std::vector<cowboys::GPAgentBase *>());
            TEMPinitialAgentPositions.push_back(std::vector<cse491::GridPosition>());
            for (size_t j = 0; j < NumAgentsForArena; ++j) {

              cowboys::GPAgentBase &addedAgent = static_cast<cowboys::GPAgentBase &>(environments[i]->template AddAgent<AgentType>(
                      "Agent " + std::to_string(j)));
              addedAgent.SetPosition(0, 0);
              addedAgent.SetSeed(seed);
              cse491::GridPosition position = addedAgent.GetPosition();

              TEMPinitialAgentPositions[i].push_back(position);

              agents[i].emplace_back(&addedAgent);

            }

          }

          Printgrid(0);


        }


        double SimpleFitnessFunction(cse491::AgentBase &agent, cse491::GridPosition startPosition) {
          double fitness = 0;

          // Euclidean distance
          cse491::GridPosition currentPosition = agent.GetPosition();
          double distance = std::sqrt(std::pow(currentPosition.GetX() - startPosition.GetX(), 2) +
                                      std::pow(currentPosition.GetY() - startPosition.GetY(), 2));

          double score = distance;

          fitness += score;

          // Agent complexity, temporarily doing this in a bad way
          if (auto *cgp = dynamic_cast<CGPAgent *>(&agent)) {
            auto genotype = cgp->GetGenotype();
            double connection_complexity = static_cast<double>(genotype.GetNumConnections()) / genotype.GetNumPossibleConnections();

            double functional_nodes = genotype.GetNumFunctionalNodes();
            double node_complexity = functional_nodes / (functional_nodes + 1);

            double complexity = connection_complexity + node_complexity;
            fitness -= complexity;
          }

          return fitness;
        }

        std::filesystem::path getSystemPath(){
          /// XML save filename data
          std::string relativePath = "../../savedata/GPAgent/";
          std::filesystem::path absolutePath = std::filesystem::absolute(relativePath);
          std::filesystem::path normalizedAbsolutePath = std::filesystem::canonical(absolutePath);
          return normalizedAbsolutePath;
        }

        std::string getDateStr() {
          auto now = std::chrono::system_clock::now();
          std::time_t now_time = std::chrono::system_clock::to_time_t(now);

          // Format the date and time as a string (hour-minute-second)
          std::tm tm_time = *std::localtime(&now_time);
          std::ostringstream oss;
          oss << std::put_time(&tm_time, "%Y-%m-%d__%H_%M_%S");
          std::string dateTimeStr = oss.str();

          return dateTimeStr;
        }

        /**
         *
         * @param maxThreads
         * @param numberOfTurns
         */
        void ThreadTrainLoop(size_t maxThreads = 1, int numberOfTurns = 100) {
          std::vector<std::thread> threads;

          size_t threadsComplete = 0;


          for (size_t arena = 0; arena < environments.size(); ++arena) {
            if (maxThreads == 0 || threads.size() < maxThreads) {
              threads.emplace_back(&GPTrainingLoop::RunArena, this, arena, numberOfTurns);

            } else {
              // Wait for one of the existing threads to finish
              threads[0].join();
              threads.erase(threads.begin());
              threadsComplete++;
              threads.emplace_back(&GPTrainingLoop::RunArena, this, arena, numberOfTurns);
            }



            size_t barWidth = 64;
            float progress = (float)(threadsComplete) / environments.size();
            size_t pos = barWidth * progress;
            std::cout << "[";
            for (size_t i = 0; i < barWidth; ++i) {
              if (i < pos) std::cout << "=";
              else if (i == pos) std::cout << ">";
              else std::cout << " ";
            }
            std::cout << "] " << int(progress * 100.0) << " % - " << threadsComplete << " threads done\r";
            std::cout.flush();
          }

          // Wait for all threads to finish
          for (auto &thread: threads) {
//            thread.join();
            if (thread.joinable()) {
              thread.join();
            }
          }
        }

        /**
         * @brief: runs the Genetic Programming training loop for a number of generations to evolve the agents
         *
         * @param numGenerations
         * @param numberOfTurns
         * @param maxThreads
         */
        void Run(size_t numGenerations,
                 size_t numberOfTurns = 100,
                 size_t maxThreads = 0, bool saveData=false) {



          std::filesystem::path normalizedAbsolutePath = getSystemPath();
          std::string dateTimeStr = getDateStr();

          const std::string filename = "AgentData_" + dateTimeStr + ".xml";
          auto fullPath = normalizedAbsolutePath / filename;

          const std::string lastGenerationsFilename = "lastGenerations_" + dateTimeStr + ".xml";
          auto lastGenerationsFullPath = normalizedAbsolutePath / lastGenerationsFilename;


          auto startTime = std::chrono::high_resolution_clock::now();

          for (size_t generation = 0; generation < numGenerations; ++generation) {

            auto generationStartTime = std::chrono::high_resolution_clock::now();

            InitTEMPAgentFitness();


            ThreadTrainLoop(maxThreads, numberOfTurns);

//            std::cout.flush();
            std::cout << std::endl;

            sortedAgents.clear();
            SortThemAgents();

            int countMaxAgents = AgentAnalysisComputations(generation);
            if (generation % 10 == 0) {
              if(saveData){
                saveEverySoOften(fullPath.string(), lastGenerationsFullPath.string());
              }
              lastGenerationsDoc.Clear();
              ResetMainTagLastGenerations();

              std::cout << "@@@@@@@@@@@@@@@@@@@@@@  " << "DataSaved" << "  @@@@@@@@@@@@@@@@@@@@@@" << std::endl;

            }

            if (saveData){
              SerializeAgents(countMaxAgents, generation);
            }


            GpLoopMutateHelper();


            resetEnvironments();

            auto generationEndTime = std::chrono::high_resolution_clock::now();
            auto generationDuration = std::chrono::duration_cast<std::chrono::microseconds>(
                    generationEndTime - generationStartTime);
            std::cout << "Generation " << generation << " took " << generationDuration.count() / 1000000.0 << " seconds"
                      << std::endl;

          }

          auto endTime = std::chrono::high_resolution_clock::now();
          auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
          std::cout << "Time taken by run: " << duration.count() / 1000000.0 << " seconds" << std::endl;

          if(saveData){
          saveEverySoOften(fullPath.string(), lastGenerationsFullPath.string());
          std::cout << "@@@@@@@@@@@@@@@@@@@@@@  " << "DataSaved" << "  @@@@@@@@@@@@@@@@@@@@@@" << std::endl;
          }

          MemGOBYE();
        }

        /**
         * Computes agents analysis metrics
         *
         * @param generation
         * @return
         */
        int AgentAnalysisComputations(int generation) {
          // print average fitness
          double averageFitness = 0;
          double maxFitness = 0;

          std::pair<int, int> bestAgent = std::make_pair(-1, -1);
          int countMaxAgents = 0;
          for (size_t arena = 0; arena < environments.size(); ++arena) {
            for (size_t a = 0; a < agents[arena].size(); ++a) {
              averageFitness += TEMPAgentFitness[arena][a];


              if (abs(TEMPAgentFitness[arena][a] - maxFitness) > 0.01 && TEMPAgentFitness[arena][a] > maxFitness) {
                maxFitness = TEMPAgentFitness[arena][a];
                bestAgent = std::make_pair(arena, a);
                countMaxAgents = 1;
              }

              if (abs(TEMPAgentFitness[arena][a] - maxFitness) < 0.01) {
                countMaxAgents++;
              }
            }
          }

          averageFitness /= (environments.size() * agents[0].size());

          std::cout << "Generation " << generation << " complete" << std::endl;
          std::cout << "Average fitness: " << averageFitness << " ";
          std::cout << "Max fitness: " << maxFitness << std::endl;
          std::cout << "Best agent: AGENT[" << bestAgent.first << "," << bestAgent.second << "] " << std::endl;
          cse491::GridPosition bestAgentPosition = agents[bestAgent.first][bestAgent.second]->GetPosition();
          std::cout << "Best Agent Final Position: " << bestAgentPosition.GetX() << "," << bestAgentPosition.GetY()
                    << std::endl;
          std::cout << "Number of agents with max fitness: " << countMaxAgents << std::endl;
          std::cout << "------------------------------------------------------------------" << std::endl;
          return countMaxAgents;
        }

        /**
         * @brief: sort the agents based on their fitness
         */
        void SortThemAgents() {
          for (size_t arena = 0; arena < environments.size(); ++arena) {
            for (size_t a = 0; a < agents[arena].size(); ++a) {
              sortedAgents.push_back(std::make_pair(arena, a));
            }
          }

          std::sort(sortedAgents.begin(), sortedAgents.end(),
                    [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
                        return TEMPAgentFitness[a.first][a.second] > TEMPAgentFitness[b.first][b.second];
                    });
        }

        /**
         * @brief: save checkpoint for data
         *
         * @param fullPath
         * @param lastGenerationsFullPath
         */
        void saveEverySoOften(std::string fullPath, std::string lastGenerationsFullPath) {


          if (doc.SaveFile(fullPath.c_str()) == tinyxml2::XML_SUCCESS) {
            // std::filesystem::path fullPath = std::filesystem::absolute("example.xml");
            std::cout << "XML file saved successfully to: " << fullPath << std::endl;
          } else {
            std::cout << "Error saving XML file." << std::endl;
          }

          if (lastGenerationsDoc.SaveFile(lastGenerationsFullPath.c_str()) == tinyxml2::XML_SUCCESS) {
            std::cout << "XML file saved successfully to: " << "lastGenerations.xml" << std::endl;
          } else {
            std::cout << "Error saving XML file." << std::endl;
          }

        }


        /**
         * @brief: Serializes the agents to an XML file.
         *
         * @param countMaxAgents
         * @param generation
         * @param topN
         */
        void SerializeAgents(int /*countMaxAgents*/, int generation, size_t topN = 5) {

          const char *tagName = ("generation_" + std::to_string(generation)).c_str();
          auto *generationTag = doc.NewElement(tagName);
          auto *lastGenerationsRoot = lastGenerationsDoc.NewElement(tagName);


          root->InsertFirstChild(generationTag);
          rootTEMP->InsertFirstChild(lastGenerationsRoot);

          for (size_t i = 0; i < std::min(sortedAgents.size(), topN); ++i) {
            auto [arenaIDX, agentIDX] = sortedAgents[i];
            agents[arenaIDX][agentIDX]->Serialize(doc, generationTag, TEMPAgentFitness[arenaIDX][agentIDX]);

            agents[arenaIDX][agentIDX]->Serialize(lastGenerationsDoc, lastGenerationsRoot,
                                                  TEMPAgentFitness[arenaIDX][agentIDX]);
          }


        }

        /**
         * @brief: initialize the TEMP agent fitness vector
         */
        void InitTEMPAgentFitness() {
          for (size_t arena = 0; arena < environments.size(); ++arena) {
            TEMPAgentFitness.push_back(std::vector<double>(agents[arena].size(), 0));
          }
        }


        /**
         * @brief Helper function for the GP loop mutate function.
         *         This function mutates the agents.
         *         This function is called in a thread.
         *
         * @param start : The start index of the agents to mutate.
         * @param end : The end index of the agents to mutate.
         * @param sortedAgents : The sorted agents' index vector.
         * @param agents : The agents vector.
         * @param mutationRate: The mutation rate.
         */
        void MutateAgents(int start, int end, const std::vector<std::pair<int, int>> &sortedAgents,
                          std::vector<std::vector<cowboys::GPAgentBase *>> &agents, double mutationRate) {
          for (int i = start; i < end; i++) {
            auto [arenaIDX, agentIDX] = sortedAgents[i];
            agents[arenaIDX][agentIDX]->MutateAgent(mutationRate);

            if (i % (sortedAgents.size() / 10) == 0) {
              std::cout << " --- mutation complete " << (i * 1.0 / sortedAgents.size()) << std::endl;
            }
          }
        }

        /**
         * @brief Helper function for the GP loop mutate function.
         *          This function copies the elite agents and mutates them.
         *          This function is called in a thread.
         *          for th
         * @param start
         * @param end
         * @param sortedAgents
         * @param agents
         * @param elitePopulationSize
         */
        void MutateAndCopyAgents(int start, int end, const std::vector<std::pair<int, int>> &sortedAgents,
                                 std::vector<std::vector<cowboys::GPAgentBase *>> &agents, int elitePopulationSize) {
          for (int i = start; i < end; i++) {
            auto [arenaIDX, agentIDX] = sortedAgents[i];
            auto eliteINDEX = rand() % elitePopulationSize;
            auto [eliteArenaIDX, eliteAgentIDX] = sortedAgents[eliteINDEX];

            agents[arenaIDX][agentIDX]->Copy(*agents[eliteArenaIDX][eliteAgentIDX]);
            agents[arenaIDX][agentIDX]->MutateAgent(0.01);

            if (i % (sortedAgents.size() / 10) == 0) {
              std::cout << " --- mutation complete " << (i * 1.0 / sortedAgents.size()) << std::endl;
            }
          }
        }

        /**
         * @brief Helper function for the GP loop mutate function.
         *
         */
        void GpLoopMutateHelper() {

          constexpr double ELITE_POPULATION_PERCENT = 0.1;
          constexpr double UNFIT_POPULATION_PERCENT = 0.2;


          const int ELITE_POPULATION_SIZE = int(ELITE_POPULATION_PERCENT * sortedAgents.size());


          double averageEliteFitness = 0;
          for (int i = 0; i < ELITE_POPULATION_SIZE; i++) {
            auto [arenaIDX, agentIDX] = sortedAgents[i];
            averageEliteFitness += TEMPAgentFitness[arenaIDX][agentIDX];
          }
          averageEliteFitness /= ELITE_POPULATION_SIZE;

          std::cout << " --- average elite score " << averageEliteFitness << "------ " << std::endl;


          const int MIDDLE_MUTATE_ENDBOUND = int(sortedAgents.size() * (1 - UNFIT_POPULATION_PERCENT));
          const int MIDDLE_MUTATE_STARTBOUND = int(ELITE_POPULATION_PERCENT * sortedAgents.size());

          // Determine the number of threads to use
          const int num_threads = std::thread::hardware_concurrency();

          std::vector<std::thread> threads;

          // Calculate the number of agents per thread
          int agents_per_thread = (MIDDLE_MUTATE_ENDBOUND - MIDDLE_MUTATE_STARTBOUND) / num_threads;

          // Launch threads for the first loop
          for (int i = 0; i < num_threads; ++i) {
            int start = MIDDLE_MUTATE_STARTBOUND + i * agents_per_thread;
            int end = (i == num_threads - 1) ? MIDDLE_MUTATE_ENDBOUND : start + agents_per_thread;
            threads.push_back(std::thread([this, start, end] {
                this->MutateAgents(start, end, sortedAgents, agents, 0.05);
            }));
          }

          // Join the threads
          for (auto &t: threads) {
            t.join();
          }

          threads.clear();

          // Second loop - copy and mutate agents
          // int unfitAgents = int(sortedAgents.size() * UNFIT_POPULATION_PERCENT);
          agents_per_thread = (sortedAgents.size() - MIDDLE_MUTATE_ENDBOUND) / num_threads;
          for (int i = 0; i < num_threads; ++i) {
            int start = MIDDLE_MUTATE_ENDBOUND + i * agents_per_thread;
            int end = (i == num_threads - 1) ? sortedAgents.size() : start + agents_per_thread;

            threads.push_back(std::thread([this, start, end, ELITE_POPULATION_SIZE] {
                this->MutateAndCopyAgents(start, end, sortedAgents, agents, ELITE_POPULATION_SIZE);
            }));
          }

          for (auto &t: threads) {
            t.join();
          }


        }

        /**
         * @brief Prints the grid for a single arena.
         * @param arenaId
         * @author: @amantham20
         */
        void Printgrid(int arena) {
          auto &grid = environments[arena]->GetGrid();
          std::vector<std::string> symbol_grid(grid.GetHeight());


          const auto &type_options = environments[arena]->GetCellTypes();
          // Load the world into the symbol_grid;
          for (size_t y = 0; y < grid.GetHeight(); ++y) {
            symbol_grid[y].resize(grid.GetWidth());
            for (size_t x = 0; x < grid.GetWidth(); ++x) {
              symbol_grid[y][x] = type_options[grid.At(x, y)].symbol;
            }
          }


          const auto &agent_set = agents[arena];
          for (const auto &agent_ptr: agent_set) {
            cse491::GridPosition pos = agent_ptr->GetPosition();
            char c = '*';
            if (agent_ptr->HasProperty("symbol")) {
              c = agent_ptr->template GetProperty<char>("symbol");
            }
            symbol_grid[pos.CellY()][pos.CellX()] = c;
          }

          // Print out the symbol_grid with a box around it.
          std::cout << '+' << std::string(grid.GetWidth(), '-') << "+\n";
          for (const auto &row: symbol_grid) {
            std::cout << "|";
            for (char cell: row) {
              // std::cout << ' ' << cell;
              std::cout << cell;
            }
            std::cout << "|\n";
          }

          std::cout << '+' << std::string(grid.GetWidth(), '-') << "+\n";
          std::cout << std::endl;
        }


        /**
         * @brief Prints the grid for each arena.
         *
         * @author: @amantham20
         */
        void PrintGrids() {
          for (size_t arena = 0; arena < environments.size(); ++arena) {
            std::cout << "Arena " << arena << std::endl;
            Printgrid(arena);
          }
        }


        /**
         * @brief Resets the environments to their initial state.
         *     This function is called after each generation.
         *     This function currently only soft resets the environments.
         */
        void resetEnvironments() {

          for (size_t arena = 0; arena < environments.size(); ++arena) {
            for (size_t a = 0; a < agents[arena].size(); ++a) {
              agents[arena][a]->SetPosition(TEMPinitialAgentPositions[arena][a]);
              agents[arena][a]->Reset();
            }
          }

          TEMPAgentFitness.clear();
        }

        /**
         * @brief Runs the training loop for a single arena.
         *      This function is called in a thread.
         *      Each arena is run in a separate thread.
         *
         * @author: @amantham20
         * @param arena : The arena to run.
         * @param numberOfTurns : The number of turns to run the arena for.
         */
        void RunArena(size_t arena, size_t numberOfTurns) {
          for (size_t size = 0; size < STARTPOSITIONS.size(); ++size) {
            agents[arena][size]->SetPosition(STARTPOSITIONS[size]);

            for (size_t turn = 0; turn < numberOfTurns; turn++) {
              environments[arena]->RunAgents();
              environments[arena]->UpdateWorld();
            }
            for (size_t a = 0; a < agents[arena].size(); ++a) {
              double tempscore = SimpleFitnessFunction(*agents[arena][a], STARTPOSITIONS[size]);
              TEMPAgentFitness[arena][a] += tempscore;
            }
          }

          for (size_t a = 0; a < agents[arena].size(); ++a) {
            TEMPAgentFitness[arena][a] /= STARTPOSITIONS.size();
          }


        }

        /**
         * @brief Clears the memory of the training loop.
         */
        void MemGOBYE() {

//          TEMPinitialAgentPositions.clear();
          for (auto &postions: TEMPinitialAgentPositions) {
            postions.clear();
          }
          const int length = TEMPinitialAgentPositions.size();
          for (int i = 0; i < length; ++i) {
            TEMPinitialAgentPositions.pop_back();
          }

          TEMPAgentFitness.clear();
          environments.clear();
          agents.clear();
          sortedAgents.clear();

        }

        ~GPTrainingLoop() = default;
    };
}