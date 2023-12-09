
#pragma once

#include "../../core/AgentBase.hpp"
#include "../../core/WorldBase.hpp"
//#include "GPAgent.hpp"
#include "GPAgentBase.hpp"


#include "CGPAgent.hpp"
#include "LGPAgent.hpp"

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
#include "GPAgentAnalyze.h"


namespace cowboys {

    constexpr unsigned int TRAINING_SEED = 0; ///< If this is 0, then a random seed will be used

    template<class AgentType, class EnvironmentType>
    class GPTrainingLoop {
    private:

        std::vector<std::unique_ptr<cse491::WorldBase>> environments;
        std::vector<std::vector<cowboys::GPAgentBase *>> agents;
        std::vector<std::vector<double>> TEMPAgentFitness;

        GPAgentAnalyzer analyzer;

        tinyxml2::XMLDocument topAgentsDoc;
        tinyxml2::XMLDocument lastGenerationsTopAgentsDoc; // <- Saves the last 5 generations
        tinyxml2::XMLDocument allOfLastGeneration;
        tinyxml2::XMLDocument metaData;


        tinyxml2::XMLElement *rootTopAllGenerations = topAgentsDoc.NewElement("GPLoopTopOfAllGeneration");
        tinyxml2::XMLElement *rootTopLastGenerations = lastGenerationsTopAgentsDoc.NewElement("GPLoopLastNGeneration");
        tinyxml2::XMLElement *rootAllOfLastGeneration = allOfLastGeneration.NewElement("GPLoopAllOfLastGeneration");
        tinyxml2::XMLElement *rootMetaData = metaData.NewElement("GPLoopMetaData");


        std::vector<std::pair<int, int>> sortedAgents = std::vector<std::pair<int, int>>();

            /**
             * Default Grid
             */
        const std::vector<cse491::GridPosition> STARTPOSITIONS = {cse491::GridPosition(0,0), cse491::GridPosition(22,5) , cse491::GridPosition(22,1) , cse491::GridPosition(0,8), cse491::GridPosition(22,8)};
//        const std::vector<cse491::GridPosition> STARTPOSITIONS = {cse491::GridPosition(0,0), cse491::GridPosition(22,5) };
//        const std::vector<cse491::GridPosition> STARTPOSITIONS = {cse491::GridPosition(22,5) };
//        const std::vector<cse491::GridPosition> STARTPOSITIONS = {cse491::GridPosition(0,0)};


/**
 * Group 8 Grid
 */
//        const std::vector<cse491::GridPosition> STARTPOSITIONS = {cse491::GridPosition(0,2), cse491::GridPosition(49,2) , cse491::GridPosition(49,19) , cse491::GridPosition(4,19), cse491::GridPosition(28,10)};


/**
 * Default Grid    2
 */
//        const std::vector<cse491::GridPosition> STARTPOSITIONS = {cse491::GridPosition(0,0), cse491::GridPosition(50,0) , cse491::GridPosition(0,28) , cse491::GridPosition(50,28)};

        /// ArenaIDX, AgentIDX, EndPosition
        std::vector<std::vector<std::vector<cse491::GridPosition>>> endPositions = std::vector<std::vector<std::vector<cse491::GridPosition>>>();
        std::vector<std::vector<std::vector<double>>> independentAgentFitness = std::vector<std::vector<std::vector<double>>>();

        int global_max_threads = std::thread::hardware_concurrency();
        int rollingRandomSeed = 0;

        double ELITE_POPULATION_PERCENT = 0.1;
        double UNFIT_POPULATION_PERCENT = 0.2;

    public:
        bool ScavengerQueuing = false;

        /**
         * @brief: constructor
         */
        GPTrainingLoop(const bool scavengerQueuing = false) : ScavengerQueuing(scavengerQueuing) {

          topAgentsDoc.InsertFirstChild(rootTopAllGenerations);

          rootMetaData = metaData.NewElement("GPLoopMetaData");
          metaData.InsertFirstChild(rootMetaData);
          srand(TRAINING_SEED);
          ResetMainTagLastGenerations();
        }

        /**
         * Resets the xml for data that needs to be overwritten
         */
        void ResetMainTagLastGenerations() {
          rootTopLastGenerations = lastGenerationsTopAgentsDoc.NewElement("GPLoop");
          lastGenerationsTopAgentsDoc.InsertFirstChild(rootTopLastGenerations);

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
            environments.emplace_back(std::make_unique<EnvironmentType>(seed));


            agents.push_back(std::vector<cowboys::GPAgentBase *>());

            endPositions.push_back(std::vector<std::vector<cse491::GridPosition>>());
            independentAgentFitness.push_back(std::vector<std::vector<double>>());

            for (size_t j = 0; j < NumAgentsForArena; ++j) {

              endPositions[i].push_back(std::vector<cse491::GridPosition>());
              independentAgentFitness[i].push_back(std::vector<double>());

              for (size_t k = 0; k < STARTPOSITIONS.size(); ++k) {
                endPositions[i][j].push_back(cse491::GridPosition(0, 0));
                independentAgentFitness[i][j].push_back(0);
              }

              cowboys::GPAgentBase &addedAgent = static_cast<cowboys::GPAgentBase &>(environments[i]->template AddAgent<AgentType>(
                      "Agent " + std::to_string(j)));
              addedAgent.SetPosition(0, 0);
              addedAgent.SetSeed(seed);

              agents[i].emplace_back(&addedAgent);

            }

          }

          loadLastGeneration();

          Printgrid(STARTPOSITIONS);


          const size_t numAgents = numArenas * NumAgentsForArena;

          std::stringstream ss;
          // ss.imbue(std::locale(""));
          ss << std::fixed << numAgents;

          std::cout << "number of agents " << std::fixed << ss.str() << std::endl;

        }


        void loadLastGeneration() {
          if (ScavengerQueuing)
          {

            auto lastFile = FullLoadGrabLatestGeneration();

            if (lastFile == "NOTTAFILE")
            {
              std::cout << "No last file found" << std::endl;
              return;
            }

            allOfLastGeneration.LoadFile(lastFile.string().c_str());
            rootAllOfLastGeneration = allOfLastGeneration.FirstChildElement("GPLoopAllOfLastGeneration");

            std::cout << "Loaded last file" << std::endl;

//            if (auto agentType = dynamic_cast<CGPAgent *>(&agents[0][0])) {
//              std::cout << "Agent Type is CGPAgent" << std::endl;
//              agentType->Import(lastFile.string());
//            }
//            else if (auto agentType = dynamic_cast<LGPAgent *>(&agents[0][0])) {
//              std::cout << "Agent Type is LGPAgent" << std::endl;
//              agentType->Import(lastFile.string());
//            }
//            else {
//              std::cout << "Agent Type is not CGPAgent or LGPAgent" << std::endl;
//            }
//              use typetraits to check the type of the agent
            if (std::is_same<AgentType, CGPAgent>::value) {
              std::cout << "Agent Type is CGPAgent" << std::endl;
            }
            else if (std::is_same<AgentType, LGPAgent>::value) {
              std::cout << "Agent Type is LGPAgent" << std::endl;
              assert(false); //TODO: Agent not implemented for import
            }
            else {
              std::cout << "Agent Type is not CGPAgent or LGPAgent" << std::endl;
            }


            auto *latestGenerationElem = rootAllOfLastGeneration->FirstChildElement();
            std::cout << latestGenerationElem->Name() << std::endl;
            auto *agentElem = latestGenerationElem->FirstChildElement();
            tinyxml2::XMLElement* generationElem = nullptr;

            std::cout << agentElem->Name() << std::endl;
            size_t numArenas = agents.size();
            size_t NumAgentsForArena = agents.at(0).size();
            for (size_t i = 0; i < numArenas; ++i) {
              for (size_t j = 0; j < NumAgentsForArena; ++j) {

                generationElem = agentElem->FirstChildElement();
                const char *genotypeData = generationElem->GetText();
                agents.at(i).at(j)->Import(genotypeData);
                agentElem = agentElem->NextSiblingElement();
              }
            }

            std::cout << "Agents LoadComplete" << std::endl;

          }
        }

        std::filesystem::path FullLoadGrabLatestGeneration() {
          std::filesystem::path normalizedAbsolutePath = getSystemPath();

          std::string lastGenerationsPrefix = "allAgentData_";
          std::string lastGenerationsFilenameExtension = ".xml";

          std::vector<std::filesystem::path> matchingFiles;
          for (const auto & entry : std::filesystem::directory_iterator(normalizedAbsolutePath))
          {
            if (entry.path().extension() == lastGenerationsFilenameExtension && entry.path().filename().string().find(lastGenerationsPrefix) != std::string::npos)
            {
              matchingFiles.push_back(entry.path());
            }
          }
          std::sort(matchingFiles.begin(), matchingFiles.end());

          if (matchingFiles.empty())
          {
            return "NOTTAFILE";
          }

          std::filesystem::path lastFile = matchingFiles.back();

          std::cout << "Last File: " << lastFile << std::endl;

          return lastFile;
        }


        /**
         * Simple and temporary fitness function
         * @param agent
         * @param startPosition
         * @return
         */
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
            fitness -= cgp->GetComplexity();
          }

          return fitness;
        }

//        STARTPOSITIONS[startPos_idx], agents[arena][a]->GetPosition(), arena, a
        double AStarFitnessFunction(const cse491::GridPosition  & startpos, const cse491::GridPosition & endpos, int arena, int a)
        {
          double fitness = 0;

          // Euclidean distance
          cse491::GridPosition currentPosition = endpos;
          double distance = static_cast<double>(
                  Get_A_StarDistance(startpos, currentPosition, arena, a)
                  );


          double score = distance;

          fitness += score;

          cse491::AgentBase &agent = *agents[arena][a];
          // Agent complexity, temporarily doing this in a bad way
          if (auto *cgp = dynamic_cast<CGPAgent *>(&agent)){
            fitness -= cgp->GetComplexity();
          }

          return fitness;
        }

        /**
         * Gets the path of the save location
         * @return
         */
        static std::filesystem::path getSystemPath() {
          /// XML save filename data
          std::string relativePath = "../../savedata/GPAgent/";
          std::filesystem::path absolutePath = std::filesystem::absolute(relativePath);
          std::filesystem::path normalizedAbsolutePath = std::filesystem::canonical(absolutePath);
          return normalizedAbsolutePath;
        }

        /**
         * Gets the date and time as a string
         * @return
         */
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

          global_max_threads = maxThreads;


          SaveDataParams saveDataParams(0);
          saveDataParams.save = saveData;
          saveDataParams.saveMetaData = true;
          saveDataParams.saveAllAgentData = true;
//
//          saveDataParams.saveTopAgents = true;
//          saveDataParams.saveLastGenerations = true;



//          check to see if meta data exists
          std::filesystem::path normalizedAbsolutePath = getSystemPath();
          std::string metaDataFilename = "metaData.xml";
//          check to see if the file exists
          std::filesystem::path metaDataFullPath = normalizedAbsolutePath / metaDataFilename;


          size_t generation = 0; // <- the generation to start at

          if (std::filesystem::exists(metaDataFullPath) && ScavengerQueuing) {
            std::cout << "MetaData file exists" << std::endl;
            metaData.LoadFile(metaDataFullPath.string().c_str());
            rootMetaData = metaData.FirstChildElement("GPLoopMetaData");
            auto *generationTag = rootMetaData->FirstChildElement();
            generation = generationTag->UnsignedAttribute("generation") + 1;
            rollingRandomSeed = generationTag->UnsignedAttribute("randSeed");
            std::cout << "Starting at generation " << generation << std::endl;

          } else {
            if (ScavengerQueuing)
            {
              std::cout << "MetaData file does not exist Starting a new Scavenger Queue" << std::endl;
            }
            rollingRandomSeed = TRAINING_SEED;
            rootMetaData = metaData.NewElement("GPLoopMetaData");
            metaData.InsertFirstChild(rootMetaData);
          }

          for (; generation < numGenerations; ++generation) {
            srand(rollingRandomSeed);
            rollingRandomSeed = rand();

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
          analyzer.saveToFile();

          }

          auto endTime = std::chrono::high_resolution_clock::now();
          auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
          std::cout << "Time taken by run: " << duration.count() / 1000000.0 << " seconds" << std::endl;

          if (saveData) {

          }

          MemGOBYE();
        }

        /**
         * SaveDataParams for saving data in checkpoints
         */
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


        /**
         * Saves checkpoint data to XML files everyso often
         * @param params
         */
        void SaveDataCheckPoint(const SaveDataParams &params) {
          if (!params.save) {
            return;
          }

          allOfLastGeneration.Clear();
          rootAllOfLastGeneration = allOfLastGeneration.NewElement("GPLoopAllOfLastGeneration");
          allOfLastGeneration.InsertFirstChild(rootAllOfLastGeneration);

          size_t totalNumberOfAgents = agents.size() * agents[0].size();

          SerializeAgents(params.generation, rootAllOfLastGeneration, allOfLastGeneration, totalNumberOfAgents);

          if (params.saveTopAgents) {
            SerializeAgents(params.generation, rootTopAllGenerations, topAgentsDoc);
          }

          if (params.saveLastGenerations){
            SerializeAgents(params.generation, rootTopLastGenerations, lastGenerationsTopAgentsDoc, 5);
          }

          std::string dateTimeStr = params.dateTimeStr;
          std::filesystem::path normalizedAbsolutePath = params.normalizedAbsolutePath;


          if (params.generation % params.checkPointEvery != 0) {
            return;
          }


          if (params.saveMetaData) {
            const std::string metaDataFilename = "metaData_" + dateTimeStr + ".xml";
            auto metaDataFullPath = normalizedAbsolutePath / metaDataFilename;
            saveXMLDoc(metaData, metaDataFullPath.string());
          }

          if (params.saveAllAgentData) {
            const std::string allAgentDataFilename = "allAgentData_" + dateTimeStr + ".xml";
            auto allAgentDataFullPath = normalizedAbsolutePath / allAgentDataFilename;
            saveXMLDoc(allOfLastGeneration, allAgentDataFullPath.string());
          }



          if (params.saveTopAgents) {
            const std::string filename = "AgentData_" + dateTimeStr + ".xml";
            auto fullPath = normalizedAbsolutePath / filename;

            saveXMLDoc(topAgentsDoc, fullPath.string());
          }

          if (params.saveLastGenerations) {
            const std::string lastGenerationsFilename = "lastGenerations_" + dateTimeStr + ".xml";
            auto lastGenerationsFullPath = normalizedAbsolutePath / lastGenerationsFilename;
            saveXMLDoc(lastGenerationsTopAgentsDoc, lastGenerationsFullPath.string());
          }


          std::cout << "@@@@@@@@@@@@@@@@@@@@@@  " << "DataSaved" << "  @@@@@@@@@@@@@@@@@@@@@@" << std::endl;

//            analyzer.saveToFile(getSystemPath() / "fitness.csv");
          lastGenerationsTopAgentsDoc.Clear();
          ResetMainTagLastGenerations();
        }


        /**
         * Helper function to format the data analysis
         * @param pos
         * @param precision
         * @return
         */
        std::string FormatPosition(const cse491::GridPosition & pos, int precision = 0) {
          std::stringstream ss;
          ss << std::fixed << std::setprecision(precision) << "[" << pos.GetX() << "," << pos.GetY() << "]";
          return ss.str();
        }

        size_t Get_A_StarDistance(const cse491::GridPosition & startpos, const cse491::GridPosition & endpos, int arenaIDX, int agentIDX) {
          auto& agent = agents[arenaIDX][agentIDX];
          auto& world = environments[arenaIDX];


          auto distance = walle::GetShortestPath(startpos, endpos, *world, *agent).size() - 1;
          assert(distance >= 0);
          return distance;
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
          double maxFitness = -10000;


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
          analyzer.addAverageFitness(averageFitness);

          std::cout << "Max fitness: " << maxFitness << std::endl;
          analyzer.addMaxFitness(maxFitness);



          std::string tagName = "generation_" + std::to_string(generation);
          auto *generationTag = metaData.NewElement(tagName.c_str());
          generationTag->SetAttribute("generation", generation);

          generationTag->SetAttribute("averageFitness", averageFitness);
          generationTag->SetAttribute("maxFitness", maxFitness);
          generationTag->SetAttribute("bestAgentIDX", bestAgent.second);

          generationTag->SetAttribute("Rand", rollingRandomSeed);

          rootMetaData->InsertFirstChild(generationTag);




          std::cout << "Best agent: AGENT[" << bestAgent.first << "," << bestAgent.second << "] " << std::endl;

          std::cout << "Best Agent Final Positions" << std::endl;

          Printgrid(endPositions[bestAgent.first][bestAgent.second], 'A');

          auto& agent = agents[bestAgent.first][bestAgent.second];
          auto& startPosition = STARTPOSITIONS;
          auto& endPosition = endPositions[bestAgent.first][bestAgent.second];

          auto& world = environments[bestAgent.first];


          auto calculateDistance = [](const cse491::GridPosition& startPosition, const cse491::GridPosition & currentPosition) {
              return std::sqrt(std::pow(currentPosition.GetX() - startPosition.GetX(), 2) +
                               std::pow(currentPosition.GetY() - startPosition.GetY(), 2));
          };

          int columnWidth = 10; // Adjust as needed

          std::cout << std::left << std::setw(columnWidth) << "Start"
                    << std::setw(columnWidth) << "Final"
                  << std::setw(columnWidth) << "Distance"
                  << "A* Distance\n";
          for (size_t i = 0; i < STARTPOSITIONS.size(); ++i) {
            std::cout << std::setw(columnWidth) << FormatPosition(STARTPOSITIONS[i])
                      << std::setw(columnWidth) << FormatPosition(endPositions[bestAgent.first][bestAgent.second][i]);


            // Calculate and print Euclidean distance
            double distance = calculateDistance(STARTPOSITIONS[i], endPositions[bestAgent.first][bestAgent.second][i]);
            std::cout << std::fixed << std::setprecision(2) << std::setw(12) << distance;


            // Calculate and print A* distance
            size_t astarDistance = Get_A_StarDistance(startPosition[i], endPosition[i], bestAgent.first, bestAgent.second);
            std::cout << std::setw(12) << astarDistance;


            std::cout << std::endl;
          }

          std::cout << "with best agent weighted score of " << TEMPAgentFitness[bestAgent.first][bestAgent.second] << std::endl;
          std::cout << std::endl;
          analyzer.addAverageScore(TEMPAgentFitness[bestAgent.first][bestAgent.second]);

          std::cout << "Number of agents with max fitness: " << countMaxAgents << std::endl;
          std::cout << "------------------------------------------------------------------" << std::endl;
          analyzer.addNumAgentsWithMaxFitness(countMaxAgents);
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

          std::string tagName = "generation_" + std::to_string(generation);
          auto *generationTag = paramDocument.NewElement(tagName.c_str());


          rootElement->InsertFirstChild(generationTag);

          for (size_t i = 0; i < std::min(sortedAgents.size(), topN); ++i) {
            auto [arenaIDX, agentIDX] = sortedAgents[i];
            agents[arenaIDX][agentIDX]->SerializeGP(paramDocument, generationTag, TEMPAgentFitness[arenaIDX][agentIDX]);
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

//          constexpr double ELITE_POPULATION_PERCENT = 0.1;
//          constexpr double UNFIT_POPULATION_PERCENT = 0.2;


          const int ELITE_POPULATION_SIZE = int(ELITE_POPULATION_PERCENT * sortedAgents.size());


          double averageEliteFitness = 0;
          for (int i = 0; i < ELITE_POPULATION_SIZE; i++) {
            auto [arenaIDX, agentIDX] = sortedAgents[i];
            averageEliteFitness += TEMPAgentFitness[arenaIDX][agentIDX];
          }
          averageEliteFitness /= ELITE_POPULATION_SIZE;

          std::cout << " --- average elite score " << averageEliteFitness << "------ " << std::endl;
          analyzer.addEliteScore(averageEliteFitness);

          const int MIDDLE_MUTATE_ENDBOUND = int(sortedAgents.size() * (1 - UNFIT_POPULATION_PERCENT));
          const int MIDDLE_MUTATE_STARTBOUND = int(ELITE_POPULATION_PERCENT * sortedAgents.size());

          // Determine the number of threads to use
          const int num_threads = global_max_threads;
//          const int num_threads = std::min(static_cast<int>(std::thread::hardware_concurrency()), global_max_threads);

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

            assert(positions[pos_idx].CellY() < symbol_grid.size());
            assert(positions[pos_idx].CellX() < symbol_grid[positions[pos_idx].CellY()].size());
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
              // Reset the agent before each run
              agents[arena][a]->Reset();
              // Set the starting position
              agents[arena][a]->SetPosition(STARTPOSITIONS[startPos_idx]);
            }

            for (size_t turn = 0; turn < numberOfTurns; turn++) {
              environments[arena]->RunAgents();
              environments[arena]->UpdateWorld();
            }
            for (size_t a = 0; a < agents[arena].size(); ++a) {
//              double tempscore = SimpleFitnessFunction(*agents[arena][a], STARTPOSITIONS[startPos_idx]);
              double tempscore = AStarFitnessFunction(STARTPOSITIONS[startPos_idx], agents[arena][a]->GetPosition(), arena, a);
              auto tempEndPosition = agents[arena][a]->GetPosition();
              endPositions[arena][a][startPos_idx] = tempEndPosition;
              independentAgentFitness[arena][a][startPos_idx] = tempscore;
              TEMPAgentFitness[arena][a] += tempscore;

            }

          }

          for (size_t a = 0; a < agents[arena].size(); ++a) {
            std::vector<double> scores = independentAgentFitness[arena][a];
            auto computeMedian = [&scores]() -> double {
                std::vector<double> temp(scores);  // Copy the data
                std::sort(temp.begin(), temp.end());

                size_t n = temp.size();
                return n % 2 ? temp[n / 2] : (temp[n / 2 - 1] + temp[n / 2]) / 2.0;
            };


//            TEMPAgentFitness[arena][a] /= STARTPOSITIONS.size();
//            TEMPAgentFitness[arena][a] += computeMedian();
              double min = *std::min_element(scores.begin(), scores.end());
            [[maybe_unused]] double avg = TEMPAgentFitness[arena][a] / STARTPOSITIONS.size();
            //  TEMPAgentFitness[arena][a] = 0.7 * min + 0.3 * avg;
              // TEMPAgentFitness[arena][a] = min;
              TEMPAgentFitness[arena][a] = avg;
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
