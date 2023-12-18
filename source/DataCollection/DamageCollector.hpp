#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <numeric>
#include <fstream>
#include "JsonBuilder.hpp"

namespace DataCollection {

    /**
     * @brief A data collector class for damage of game items.
     *
     * Useful for setting up graphs for analysis of item balancing.
     */
     class DamageCollector {
     private:
         std::unordered_map<std::string, std::vector<double>> damageData;  // Damage storage map of item name to damage amounts
     public:
         /**
         * @brief Default constructor for DamageCollector.
         */
         DamageCollector() = default;

         /**
          * Store a damage amount for a certain item.
          * @param itemName Name of the item to store the damage for
          * @param damageAmt Amount of damage this item did
          */
         void RecordDamageResult(const std::string& itemName, double damageAmt) {
             damageData[itemName].push_back(damageAmt);
         }

         /**
          * Get the damage amounts for a certain item.
          * @param itemName Name of the item to get damage amounts for
          * @return Reference to the vector of damage amounts
          */
         std::vector<double>& GetDamageAmounts(std::string itemName) {
             if (damageData.contains(itemName)) {
                 return damageData[itemName];
             } else {
                 // Created only once, subsequent calls will reference this
                 static std::vector<double> empty;
                 return empty;
             }
         }

         /**
          * Calculate average damage for a certain item
          * @param itemName Item name to calculate average for
          * @return The average damage as a double, -1 if the item does not exist
          */
        double CalculateAverageDamage(const std::string& itemName) {
             if (damageData.contains(itemName)) {
                 std::vector<double>& damages = damageData[itemName];
                 return std::accumulate(damages.begin(), damages.end(), 0.0) / damages.size();
             }

             return -1.0;
         }

         void WriteToDamageFile(std::string path) {
             std::ofstream jsonfilestream(path);
             JsonBuilder json_builder;
             for (auto& damage : damageData) {
                 json_builder.AddName(damage.first);
                 for (auto& damageAmt : damage.second) {
                     json_builder.AddDamage(damageAmt);
                 }
             }
         }
     };
}