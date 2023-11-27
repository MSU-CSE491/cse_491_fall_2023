#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include "../core/GridPosition.hpp"
#include <nlohmann/json.hpp>

namespace DataCollection{
    /**
     * @brief Builds a JSON object from the data collected for an agent.
     */
    class JsonBuilder {
    private:
        nlohmann::json json;  ///< The JSON object to be built.

    public:
        /**
         * @brief Default constructor for JSONBuilder class.
         */
        JsonBuilder() = default;

        /**
         * @brief Destructor for JSONBuilder class.
         */
        ~JsonBuilder() = default;

        void Addagentname(std::string name) {
            json["agentname"] = name;
        }

        /**
         * @brief Adds a grid position to the JSON object.
         * @param pos The grid position to be added.
         */
        void AddPosition(cse491::GridPosition pos) {
            json["positions"].push_back({{"x", pos.GetX()}, {"y", pos.GetY()}});
        }

        void AddDamage(double damage) {
            json["damage"].push_back(damage);
        }

        /**
         * @brief Adds an action to the JSON object.
         * @param action The action to be added.
         */
        void AddAction(std::unordered_map<std::string, size_t> action) {
            json["actions"].push_back(action);
        }

        /**
         * @brief Adds an agent ID to the JSON object.
         * @param id The agent ID to be added.
         */
        void AddAgentId(int id) {
            json["agentIds"].push_back(id);
        }

        /**
         * @brief Adds the agent's name to the JSON object.
         * @param name The agent's name.
         */
        void AddName(std::string name) {
            json["name"] = name;
        }

        /**
         * @brief Retrieves the JSON object.
         * @return The JSON object.
         */
        nlohmann::json GetJSON() {
            return json;
        }

        /**
         * @brief Writes the JSON object to a file.
         */
        void WriteToFile(std::ofstream &jsonfilestream)
        {
            jsonfilestream << json.dump(4);
            jsonfilestream.close();
        }
    };
} // namespace DataCollection