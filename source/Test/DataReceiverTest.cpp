#include <gtest/gtest.h>
#include <vector>
#include <utility>

// Include the class definitions (DataReceiver, Entity, and GridPosition)
#include "DataReceiver.h"
#include "Entity.h"
#include "GridPosition.h"

// Define your test cases
TEST_F(DataReceiverTest, DisplayPosition) {
    // Call the DisplayPosition method
    dataReceiver->DisplayPosition(*entity);

    // Retrieve the storage vector
    const std::vector<std::pair<int, int>>& storage = dataReceiver->GetStorage();

    // Check if the stored position matches the expected values
    ASSERT_EQ(storage.size(), 1);
    EXPECT_EQ(storage[0].first, 10);
    EXPECT_EQ(storage[0].second, 20);
}


