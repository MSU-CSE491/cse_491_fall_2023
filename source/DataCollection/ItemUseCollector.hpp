#pragma once

#include <unordered_map>
#include <string>
#include <algorithm>
#include "JsonBuilder.hpp"

namespace DataCollection {

    /**
     * @brief A data collector class for usage amount of game items.
     *
     * Useful for setting up graphs for analysis of item balancing.
     */
    class ItemUseCollector {
    private:
        std::unordered_map<std::string, int> usageData;  /// Damage storage map of item name to amount of uses.
    public:
        /**
         * Default constructor for ItemUseCollector
         */
        ItemUseCollector() = default;

        /**
         * Getter for item usage data
         * @return Const reference to the usage data storage.
         */
        const std::unordered_map<std::string, int>& GetUsageData() {
            return usageData;
        }

        /**
         * Get the amount of unique items that are collected
         * @return int amount of unique items
         */
        int GetNumberOfItems() {
            return usageData.size();
        }

        /**
         * Increment usage amount for a certain item.
         * @param itemName Item name to record new usage of
         */
        void IncrementItemUsage(const std::string& itemName) {
            usageData[itemName]++;
        }

        /**
         * Get the most frequently used item in the game.
         * @return Name of the most frequent item as a string, empty string if no data
         */
        std::string GetMostFrequent() {
            if (!usageData.empty()) {
                auto maxItr = std::max_element(usageData.begin(), usageData.end(),
                        [](const auto& firstItem, const auto& secondItem) {
                            return firstItem.second < secondItem.second;
                        }
                );

                return maxItr->first;
            }

            return "";
        }

        /**
         * Get the least frequently used item in the game.
         * @return Name of the least item as a string, empty string if no data
         */
        std::string GetLeastFrequent() {
            if (!usageData.empty()) {
                auto maxItr = std::min_element(usageData.begin(), usageData.end(),
                        [](const auto& firstItem, const auto& secondItem) {
                            return firstItem.second < secondItem.second;
                        }
                );

                return maxItr->first;
            }

            return "";
        }

        void WriteToItemUseFile(std::string path)
        {
            JsonBuilder json_builder;
            std::ofstream jsonfilestream(path);
            json_builder.StartArray("items");
            for (auto& usage: usageData) {
                json_builder.AddName(usage.first);
                json_builder.AddInt("amountOfUses", usage.second);
                json_builder.InputToArray("items", json_builder.GetJSON());
                json_builder.ClearJSON();
            }
            json_builder.WriteToFile(jsonfilestream, json_builder.GetJSONArray());
            jsonfilestream.close();
        }
    };
}