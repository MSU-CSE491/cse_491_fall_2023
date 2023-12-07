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
        nlohmann::json json_array;
    public:
        /**
         * @brief Default constructor for JSONBuilder class.
         */
        JsonBuilder() = default;

        /**
         * @brief Destructor for JSONBuilder class.
         */
        ~JsonBuilder() = default;

        void StartArray(std::string title) {
            json_array[title] = nlohmann::json::array();
        }

        void InputToArray(std::string title, nlohmann::json input) {
            json_array[title].push_back(input);
        }

        /**
         * @brief Adds the agent's name to the JSON object.
         * @param name The agent's name.
         */
        void AddName(std::string name) {
            json["name"] = name;
        }

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
         * @brief Retrieves the JSON object.
         * @return The JSON object.
         */
        nlohmann::json GetJSON() {
            return json;
        }

        void ClearJSON() {
            json.clear();
        }

        nlohmann::json GetJSONArray() {
            return json_array;
        }

        void AddInt(std::string title, int usage) {
            json[title] = usage;
        }

        /**
         * @brief Writes the JSON object to a file.
         */
        void WriteToFile(std::ofstream &jsonfilestream, nlohmann::json Json)
        {
            jsonfilestream << Json.dump(4);
        }
    };
} // namespace DataCollection