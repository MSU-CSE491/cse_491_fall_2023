## DataCollection Namespace

The **DataCollection** namespace provides a set of classes and templates for managing and processing data related to agent actions and positions. This library offers flexibility and ease of use for storing, retrieving, and manipulating data within your C++ applications.

### Classes

1. **AgentData**

   The `AgentData` class represents data collected for an agent, including their name, actions, positions, and associated IDs.

   **Methods:**
   - `StorePositions(cse491::GridPosition pos)`: Stores a grid position for the agent.
   - `StoreAction(std::unordered_map<std::string, size_t> action)`: Stores an action for the agent.
   - `StoreAgentId(int id)`: Stores an agent ID associated with an action.
   - `GetActions() -> std::vector<std::unordered_map<std::string, size_t>>&`: Retrieves the stored actions.
   - `SetActions(const std::vector<std::unordered_map<std::string, size_t>>& newActions)`: Sets the stored actions to a new set of actions.
   - `GetPositionSize() const -> int`: Gets the size of the stored grid positions.
   - `GetAgentIds() const -> std::vector<int>`: Gets the agent IDs associated with the stored actions.

2. **DataReceiver\<T\>**

   The `DataReceiver` class template provides a generic way to store and retrieve data objects of a specified type.

   **Methods:**
   - `StoreData(T obj)`: Stores a data object of type T.
   - `getStorage() -> std::vector<T>&`: Retrieves the stored data objects.

3. **AgentReceiver**

   The `AgentReceiver` class extends `DataReceiver<AgentData>` and provides specific functionality for storing `AgentData` objects along with grid positions and last action IDs.

   **Methods:**
   - `StoreData(cse491::GridPosition pos, int last_action)`: Stores a grid position and last action ID associated with an agent.
   - `GetPositionSize() -> int`: Gets the position size of the last stored `AgentData` object.
   - `IsEmpty() -> bool`: Checks if the storage is empty.

### Usage

To use the **DataCollection** namespace in your C++ application, include the appropriate header files and create instances of the classes as needed. Make sure to follow the specific methods and their parameters as outlined in the class descriptions above.

```cpp
#include <DataCollection/AgentData.h>
#include <DataCollection/DataReceiver.h>
#include <DataCollection/AgentReceiver.h>

// Example Usage
DataCollection::AgentData agent;
agent.StorePositions(cse491::GridPosition(1, 2));
agent.StoreAction({{"up", 1}});
agent.StoreAgentId(101);

DataCollection::AgentReceiver agentReceiver;
agentReceiver.StoreData(cse491::GridPosition(3, 4), 102);
int positionSize = agentReceiver.GetPositionSize();
bool isEmpty = agentReceiver.IsEmpty();
```

### Installation

To use this library, simply include the necessary header files in your project. Ensure that you have C++11 or later enabled in your compiler settings.

### Contributing

Feel free to contribute by reporting issues, suggesting features, or submitting pull requests. We welcome any contributions that can improve the functionality, performance, or usability of this library.

### License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

---
