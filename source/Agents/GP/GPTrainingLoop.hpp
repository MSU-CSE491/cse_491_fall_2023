
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
#include "../Worlds/GenerativeWorld.hpp"
#include "../Worlds/ManualWorld.hpp"
#include "../Worlds/SecondWorld.hpp"
#include <type_traits>


namespace cowboys {

    constexpr unsigned int TRAINING_SEED = 0; ///< If this is 0, then a random seed will be used

    template<class AgentType, class EnvironmentType>
    class GPTrainingLoop {
    private:

        std::vector<std::unique_ptr<cse491::WorldBase>> environments;
        std::vector<std::vector<cowboys::GPAgentBase *>> agents;
        std::vector<std::vector<double>> TEMPAgentFitness;


        tinyxml2::XMLDocument topAgentsDoc;
        tinyxml2::XMLDocument lastGenerationsTopAgentsDoc; // <- Saves the last 5 generations
        tinyxml2::XMLDocument allOfLastGeneration;
        tinyxml2::XMLDocument metaData;


        tinyxml2::XMLElement *rootTopAllGenerations = topAgentsDoc.NewElement("GPLoopTopOfAllGeneration");
        tinyxml2::XMLElement *rootTopLastGenerations = lastGenerationsTopAgentsDoc.NewElement("GPLoopLastNGeneration");
        tinyxml2::XMLElement *rootAllOfLastGeneration = allOfLastGeneration.NewElement("GPLoopAllOfLastGeneration");
        tinyxml2::XMLElement *rootMetaData = metaData.NewElement("GPLoopMetaData");


        std::vector<std::pair<int, int>> sortedAgents = std::vector<std::pair<int, int>>();

        const std::vector<cse491::GridPosition> STARTPOSITIONS = {cse491::GridPosition(0,0), cse491::GridPosition(22,5) , cse491::GridPosition(22,1) , cse491::GridPosition(0,8), cse491::GridPosition(22,8)};
//        const std::vector<cse491::GridPosition> STARTPOSITIONS = {cse491::GridPosition(0,0), cse491::GridPosition(22,5) };
//        const std::vector<cse491::GridPosition> STARTPOSITIONS = {cse491::GridPosition(22,5) };
//        const std::vector<cse491::GridPosition> STARTPOSITIONS = {cse491::GridPosition(0,0)};


        /// ArenaIDX, AgentIDX, EndPosition
        std::vector<std::vector<std::vector<cse491::GridPosition>>> endPostions = std::vector<std::vector<std::vector<cse491::GridPosition>>>();


    public:

        GPTrainingLoop() {

          topAgentsDoc.InsertFirstChild(rootTopAllGenerations);
          ResetMainTagLastGenerations();
        }

        void ResetMainTagLastGenerations() {
          rootTopLastGenerations = lastGenerationsTopAgentsDoc.NewElement("GPLoop");
          lastGenerationsTopAgentsDoc.InsertFirstChild(rootTopLastGenerations);

          rootMetaData = metaData.NewElement("GPLoopMetaData");
          metaData.InsertFirstChild(rootMetaData);

          rootAllOfLastGeneration = allOfLastGeneration.NewElement("GPLoopAllOfLastGeneration");
          allOfLastGeneration.InsertFirstChild(rootAllOfLastGeneration);
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


            if constexpr (std::is_same<EnvironmentType, cse491_team8::ManualWorld>::value)
            {
              /// GROUP 8

              environments.emplace_back( std::make_unique<EnvironmentType>());
            }
            else if constexpr (std::is_same<EnvironmentType, group6::GenerativeWorld>::value)
            {
              /// GROUP 6
              static const unsigned int SEED = 5;
              auto biome = group6::BiomeType::Maze; // specify biome type here

              environments.emplace_back( std::make_unique<EnvironmentType>(biome, 100, 20,TRAINING_SEED));
            }
            else if constexpr (std::is_same<EnvironmentType,  group4::SecondWorld>::value){
              environments.emplace_back( std::make_unique<EnvironmentType>());
            }
            else
            {
              environments.emplace_back( std::make_unique<EnvironmentType>(TRAINING_SEED));
            }



            agents.push_back(std::vector<cowboys::GPAgentBase *>());

            endPostions.push_back(std::vector<std::vector<cse491::GridPosition>>());

            for (size_t j = 0; j < NumAgentsForArena; ++j) {

              endPostions[i].push_back(std::vector<cse491::GridPosition>());

              for (size_t k = 0; k < STARTPOSITIONS.size(); ++k) {
                endPostions[i][j].push_back(cse491::GridPosition(0, 0));
              }

              cowboys::GPAgentBase &addedAgent = static_cast<cowboys::GPAgentBase &>(environments[i]->template AddAgent<AgentType>(
                      "Agent " + std::to_string(j)));
              addedAgent.SetPosition(0, 0);
              addedAgent.SetSeed(seed);

              agents[i].emplace_back(&addedAgent);

            }

          }

          Printgrid(STARTPOSITIONS);

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
            double connection_complexity =
                    static_cast<double>(genotype.GetNumConnections()) / genotype.GetNumPossibleConnections();

            double functional_nodes = genotype.GetNumFunctionalNodes();
            double node_complexity = functional_nodes / (functional_nodes + 1);

            double complexity = connection_complexity + node_complexity;
            fitness -= complexity;
          }

          return fitness;
        }

        static std::filesystem::path getSystemPath() {
          /// XML save filename data
          std::string relativePath = "../../savedata/GPAgent/";
          std::filesystem::path absolutePath = std::filesystem::absolute(relativePath);
          std::filesystem::path normalizedAbsolutePath = std::filesystem::canonical(absolutePath);
          return normalizedAbsolutePath;
        }

        static std::string getDateStr() {
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
            float progress = (float) (arena+1) / environments.size();
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
            if (thread.joinable()) {
              thread.join();
              threadsComplete+=maxThreads;
            }
          }

          std::cout << std::endl;
          std::cout << "All threads done" << std::endl;


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
                 size_t maxThreads = 0, bool saveData = false) {

          auto startTime = std::chrono::high_resolution_clock::now();

          SaveDataParams saveDataParams(0);
          saveDataParams.save = saveData;
          saveDataParams.saveAllAgentData = true;

          saveDataParams.saveTopAgents = true;
          saveDataParams.saveLastGenerations = true;

          for (size_t generation = 0; generation < numGenerations; ++generation) {

            auto generationStartTime = std::chrono::high_resolution_clock::now();
            saveDataParams.updateGeneration(generation);

            InitTEMPAgentFitness();
            ThreadTrainLoop(maxThreads, numberOfTurns);

            std::cout << std::endl;

            sortedAgents.clear();
            SortThemAgents();

            int countMaxAgents = AgentsAnalysisComputationsAndPrint(generation);

            saveDataParams.countMaxAgents = countMaxAgents;
            SaveDataCheckPoint(saveDataParams);

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

          if (saveData) {

          }

          MemGOBYE();
        }


        struct SaveDataParams {
            size_t generation;
            bool save = false;
            bool saveAllAgentData = false;
            bool saveMetaData = true;
            bool saveTopAgents = false;
            bool saveLastGenerations = false;

            size_t countMaxAgents = 0;

            std::string dateTimeStr = getDateStr();
            std::filesystem::path normalizedAbsolutePath = getSystemPath();

            size_t checkPointEvery = 5;

            SaveDataParams(size_t gen) : generation(gen) {}

            void updateGeneration(size_t gen) { generation = gen; }
        };


        void SaveDataCheckPoint(const SaveDataParams &params) {
          if (!params.save) {
            return;
          }

          allOfLastGeneration.Clear();
          rootAllOfLastGeneration = allOfLastGeneration.NewElement("GPLoopAllOfLastGeneration");
          allOfLastGeneration.InsertFirstChild(rootAllOfLastGeneration);

          size_t totalNumberOfAgents = agents.size() * agents[0].size();
          SerializeAgents(params.generation, rootAllOfLastGeneration, allOfLastGeneration, totalNumberOfAgents);
//          SerializeAgents(params.generation, rootTopAllGenerations, topAgentsDoc);
//          SerializeAgents(params.generation, rootTopLastGenerations, lastGenerationsTopAgentsDoc, 5);

          std::string dateTimeStr = params.dateTimeStr;
          std::filesystem::path normalizedAbsolutePath = params.normalizedAbsolutePath;
          if (params.saveAllAgentData) {
            const std::string allAgentDataFilename = "allAgentData_" + dateTimeStr + ".xml";
            auto allAgentDataFullPath = normalizedAbsolutePath / allAgentDataFilename;
            saveXMLDoc(allOfLastGeneration, allAgentDataFullPath.string());
          }


          if (params.generation % params.checkPointEvery != 0) {
            return;
          }


          if (params.saveMetaData) {

          }

//          if (params.saveTopAgents) {
//            const std::string filename = "AgentData_" + dateTimeStr + ".xml";
//            auto fullPath = normalizedAbsolutePath / filename;
//
//            saveXMLDoc(topAgentsDoc, fullPath.string());
//          }
//
//          if (params.saveLastGenerations) {
//            const std::string lastGenerationsFilename = "lastGenerations_" + dateTimeStr + ".xml";
//            auto lastGenerationsFullPath = normalizedAbsolutePath / lastGenerationsFilename;
//            saveXMLDoc(lastGenerationsTopAgentsDoc, lastGenerationsFullPath.string());
//          }


          std::cout << "@@@@@@@@@@@@@@@@@@@@@@  " << "DataSaved" << "  @@@@@@@@@@@@@@@@@@@@@@" << std::endl;

          lastGenerationsTopAgentsDoc.Clear();
          ResetMainTagLastGenerations();
        }

        /**
         * Computes agents analysis metrics
         *
         * @param generation
         * @return
         */
        int AgentsAnalysisComputationsAndPrint(int generation, double deltaForMaxFitness = 0.1) {
          // print average fitness
          double averageFitness = 0;
          double maxFitness = 0;


          std::pair<int, int> bestAgent = std::make_pair(-1, -1);

          int countMaxAgents = 0;
          for (size_t arena = 0; arena < environments.size(); ++arena) {
            for (size_t a = 0; a < agents[arena].size(); ++a) {
              averageFitness += TEMPAgentFitness[arena][a];


              if (abs(TEMPAgentFitness[arena][a] - maxFitness) > deltaForMaxFitness &&
                  TEMPAgentFitness[arena][a] > maxFitness) {
                maxFitness = TEMPAgentFitness[arena][a];
                bestAgent = std::make_pair(arena, a);
                countMaxAgents = 1;
              }

              if (abs(TEMPAgentFitness[arena][a] - maxFitness) < deltaForMaxFitness) {
                countMaxAgents++;
              }
            }
          }

          averageFitness /= (environments.size() * agents[0].size());




          std::cout << "Generation " << generation << " complete" << std::endl;
          std::cout << "Average fitness: " << averageFitness << " ";
          std::cout << "Max fitness: " << maxFitness << std::endl;
          std::cout << "Best agent: AGENT[" << bestAgent.first << "," << bestAgent.second << "] " << std::endl;
          std::cout << "Start Positions";
          for (auto pos : STARTPOSITIONS) {
            std::cout << "[" << pos.GetX() << "," << pos.GetY() << "] ";
          }
          std::cout << std::endl;

          std::cout << "Best Agent Final Positions: ";

          for (size_t i = 0; i < endPostions[bestAgent.first][bestAgent.second].size(); ++i) {
            std::cout << "[" <<endPostions[bestAgent.first][bestAgent.second][i].GetX() << ","
            << endPostions[bestAgent.first][bestAgent.second][i].GetY() << "] ";
          }
          std::cout << "with a score of " << TEMPAgentFitness[bestAgent.first][bestAgent.second] << std::endl;
          std::cout << std::endl;

          Printgrid(endPostions[bestAgent.first][bestAgent.second], 'A');

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


        void saveXMLDoc(tinyxml2::XMLDocument &paramdoc, std::string fullPath) {
          if (paramdoc.SaveFile(fullPath.c_str()) == tinyxml2::XML_SUCCESS) {
            // std::filesystem::path fullPath = std::filesystem::absolute("example.xml");
            std::cout << "XML file saved successfully to: " << fullPath << std::endl;
          } else {
            std::cout << "Error saving XML file." << std::endl;
//            std::cout << "Error ID: " << paramdoc.ErrorID() << std::endl;
            std::cout << "Error for path" << fullPath << std::endl;
          }
        }


        /**
         * @brief: Serializes the agents to an XML file.
         *
         * @param countMaxAgents
         * @param generation
         * @param topN
         */
        void SerializeAgents(int generation, tinyxml2::XMLElement *rootElement, tinyxml2::XMLDocument &paramDocument,
                             size_t topN = 5) {

          const char *tagName = ("generation_" + std::to_string(generation)).c_str();

          auto *generationTag = paramDocument.NewElement(tagName);

          rootElement->InsertEndChild(generationTag);

          for (size_t i = 0; i < std::min(sortedAgents.size(), topN); ++i) {
            auto [arenaIDX, agentIDX] = sortedAgents[i];
            agents[arenaIDX][agentIDX]->Serialize(paramDocument, generationTag, TEMPAgentFitness[arenaIDX][agentIDX]);
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
        void Printgrid(const std::vector<cse491::GridPosition> &positions, char symbol = 'S') {

          if (environments.empty()) {
            std::cout << "No environments to print" << std::endl;
            return;
          }

          size_t arena = 0;
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



          for (size_t pos_idx = 0; pos_idx < positions.size(); ++pos_idx) {
            symbol_grid[positions[pos_idx].CellY()][positions[pos_idx].CellX()] = symbol;
          }


//          const auto &agent_set = agents[arena];
//          for (const auto &agent_ptr: agent_set) {
//            cse491::GridPosition pos = agent_ptr->GetPosition();
//            char c = '*';
//            if (agent_ptr->HasProperty("symbol")) {
//              c = agent_ptr->template GetProperty<char>("symbol");
//            }
//            symbol_grid[pos.CellY()][pos.CellX()] = c;
//          }

          std::cout << "    ";
          for (size_t x = 0; x < grid.GetWidth(); ++x) {
            if (x % 10 == 0 && x != 0) {
              std::cout << x / 10; // Print the ten's place of the column number
            } else {
              std::cout << " "; // Space for non-marker columns
            }
          }
          std::cout << "\n";

          // Print column numbers
          std::cout << "    "; // Space for row numbers
          for (size_t x = 0; x < grid.GetWidth(); ++x) {
            std::cout << x % 10; // Print only the last digit of the column number
          }
          std::cout << "\n";

          // Print out the symbol_grid with a box around it.
          std::cout << "   +" << std::string(grid.GetWidth(), '-') << "+\n";
          for (size_t y = 0; y < grid.GetHeight(); ++y) {

            if (y % 10 == 0 && y != 0) {
              std::cout << y / 10 << " "; // Print the ten's place of the row number
            } else {
              std::cout << "  "; // Space for non-marker rows
            }

            // Print row number
            std::cout << y % 10 << "|"; // Print only the last digit of the row number
            for (char cell: symbol_grid[y]) {
              std::cout << cell;
            }
            std::cout << "|\n";
          }

          std::cout << "   +" << std::string(grid.GetWidth(), '-') << "+\n";
          std::cout << std::endl;
        }



        /**
         * @brief Resets the environments to their initial state.
         *     This function is called after each generation.
         *     This function currently only soft resets the environments.
         */
        void resetEnvironments() {

          for (size_t arena = 0; arena < environments.size(); ++arena) {
            for (size_t a = 0; a < agents[arena].size(); ++a) {
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
          for (size_t startPos_idx = 0; startPos_idx < STARTPOSITIONS.size(); ++startPos_idx) {
            for(size_t a = 0; a < agents[arena].size(); ++a) {
              agents[arena][a]->SetPosition(STARTPOSITIONS[startPos_idx]);
            }

            for (size_t turn = 0; turn < numberOfTurns; turn++) {
              environments[arena]->RunAgents();
              environments[arena]->UpdateWorld();
            }
            for (size_t a = 0; a < agents[arena].size(); ++a) {
              double tempscore = SimpleFitnessFunction(*agents[arena][a], STARTPOSITIONS[startPos_idx]);
              auto tempEndPosition = agents[arena][a]->GetPosition();
              endPostions[arena][a][startPos_idx] = tempEndPosition;
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

          TEMPAgentFitness.clear();
          environments.clear();
          agents.clear();
          sortedAgents.clear();

        }

        ~GPTrainingLoop() = default;
    };
}