//
// Created by rajme on 12/4/2023.
//

#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <fstream>

namespace cowboys {

    class GPAgentAnalyzer {
    private:
        std::vector<double> average_fitness;
        std::vector<double> max_fitness;
        std::vector<double> elite_score;
        std::vector<double> average_score;
        std::vector<double> max_agents;

    public:
        GPAgentAnalyzer() = default;
        ~GPAgentAnalyzer() = default;

        void addAverageFitness(double fitness) {
            average_fitness.push_back(fitness);
        }

        void addMaxFitness(double fitness) {
            max_fitness.push_back(fitness);
        }

        void addEliteScore(double score) {
            elite_score.push_back(score);
        }

        void addAverageScore(double score) {
            average_score.push_back(score);
        }
        void addNumAgentsWithMaxFitness(double num_agents) {
            max_agents.push_back(num_agents);
        }

        void saveToFile() {
            // create a new file
            std::ofstream file("gp_agent_analyzer.csv");

            // write the data to the file
            file << "average_fitness,max_fitness,average_elite_score,best_agent_weighted_score,agents_with_max_fitness\n";

            for (int i = 0; i < average_fitness.size(); i++) {
                file << average_fitness[i] << "," << max_fitness[i] << "," << elite_score[i] << "," << average_score[i] << "," << max_agents[i] << "\n";
            }

            // close the file
            file.close();


        }
    };

}  // namespace cowboys

