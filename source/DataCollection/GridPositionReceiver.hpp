#pragma once

#include <iostream>

#include "DataReceiverTest.hpp"
#include "../core/GridPosition.hpp"

namespace DataCollection {

    class GridPositionReceiver : public DataReceiverTest<cse491::GridPosition> {

    };
}