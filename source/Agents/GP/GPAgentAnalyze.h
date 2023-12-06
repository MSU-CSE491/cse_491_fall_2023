//
// A class that analyzes the data of the best agent in the GP algorithm
// and saves it to a csv file
//

#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <fstream>

namespace cowboys {

    class GPAgentAnalyzer {
    private:
        /// The average fitness of the best agent
        std::vector<double> average_fitness;

        /// The max fitness of the best agent
        std::vector<double> max_fitness;

        /// The weighted score of the best agent
        std::vector<double> elite_score;

        /// The average score of the best agent
        std::vector<double> average_score;

        /// The number of agents with the max fitness
        std::vector<double> max_agents;

    public:
        /**
         * @brief Construct a new GP Agent Analyzer object
         */
        GPAgentAnalyzer() = default;

        /**
         * @brief Destroy the GP Agent Analyzer object
         */
        ~GPAgentAnalyzer() = default;

        /**
         * @brief Adds the average fitness of the best agent
         * @param fitness
         */
        void addAverageFitness(double fitness) {
            average_fitness.push_back(fitness);
        }

        /**
         * @brief Adds the max fitness of the best agent
         * @param fitness
         */
        void addMaxFitness(double fitness) {
            max_fitness.push_back(fitness);
        }

        /**
         * @brief Adds the weighted score of the best agent
         * @param score
         */
        void addEliteScore(double score) {
            elite_score.push_back(score);
        }

        /**
         * @brief Adds the average score of the best agent
         * @param score
         */
        void addAverageScore(double score) {
            average_score.push_back(score);
        }

        /**
         * @brief Adds the number of agents with the max fitness
         * @param num_agents
         */
        void addNumAgentsWithMaxFitness(double num_agents) {
            max_agents.push_back(num_agents);
        }

        /**
         * @brief Saves the data to a csv file
         */
        void saveToFile() {
            // create a new file
            std::ofstream file("gp_agent_analyzer.csv");

            // write the data to the file
            file << "average_fitness,max_fitness,average_elite_score,best_agent_weighted_score,agents_with_max_fitness\n";

            for (int i = 0; i < average_fitness.size(); i++) {
                file << average_fitness[i] << "," << max_fitness[i] << "," << elite_score[i] << "," << average_score[i] << "," << max_agents[i] << "\n";
            }
            std::cout << "Saved GP Agent Analyzer data to gp_agent_analyzer.csv" << std::endl;

            // close the file
            file.close();
        }
    };

}  // namespace cowboys

