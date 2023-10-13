#pragma once

#include <iostream>
#include "GameData.hpp"
#include "DataReceiver.hpp"
#include "../core/GridPosition.hpp"
#include <unordered_map>
namespace DataCollection {

    class GameReceiver : public DataReceiver<GameData> {
    public:
        GameReceiver() = default;
        ~GameReceiver() override = default;

        // You can override or add methods specific to GameReceiver here.

         void DebugPrint() override {
            // Implement the DebugPrint function for GameReceiver
            for (const auto& data : this->storage) {
                std::cout << "Number of collisions: " << data.numOfCol << std::endl;
                // You can add more details for printing actions and positions.
            }
        }
        void store_GameData(const GameData& gameData)  {

            this->storage.push_back(gameData);
        }
    };
}