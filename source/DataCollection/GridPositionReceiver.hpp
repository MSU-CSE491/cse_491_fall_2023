#pragma once

#include <iostream>

#include "DataReceiver.hpp"
#include "../core/GridPosition.hpp"

namespace DataCollection {

    class GridPositionReceiver : public DataReceiver<cse491::GridPosition> {
        public:
        void DebugPrint() override {
            // auto pos = storage.back();
            // std::cout << "Adding: (" << pos.GetX() << ", " << pos.GetY() << ")" << std::endl;
        }
    };
}