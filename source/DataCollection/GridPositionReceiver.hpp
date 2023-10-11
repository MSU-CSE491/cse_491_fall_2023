#pragma once

#include <iostream>

#include "DataReceiver.hpp"
#include "../core/GridPosition.hpp"

namespace DataCollection {

    class GridPositionReceiver : public DataReceiver<cse491::GridPosition> {

    };
}