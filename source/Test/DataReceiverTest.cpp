#include <gtest/gtest.h>
#include <vector>
#include <utility>

// Include the class definitions (DataReceiver, Entity, and GridPosition)
#include "DataReceiver.h"
#include "Entity.h"
#include "GridPosition.h"

// Define a fixture for your tests
class DataReceiverTest : public ::testing::Test {
protected:
    // Create common setup logic if needed
    DataReceiverTest() {
        // Initialize common test data here
    }

    // Perform cleanup, if necessary
    virtual ~DataReceiverTest() {
        // Cleanup common test data here
    }

    // Set up the test fixture
    virtual void SetUp() {
        // Create instances of Entity, GridPosition, and DataReceiver for testing
        gridPosition = new GridPosition(10, 20);
        entity = new Entity(*gridPosition);
        dataReceiver = new DataReceiver(*entity);
    }

    // Tear down the test fixture
    virtual void TearDown() {
        delete dataReceiver;
        delete entity;
        delete gridPosition;
    }

    // Declare your test objects here
    GridPosition* gridPosition;
    Entity* entity;
    DataReceiver* dataReceiver;
};

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

// Define more test cases as needed

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
