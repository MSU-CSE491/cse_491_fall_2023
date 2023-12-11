/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A base class for all World modules.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <algorithm>
#include <cassert>
#include <memory>
#include <queue>
#include <random>
#include <string>
#include <vector>
#include <sstream>

#include "../DataCollection/AgentReciever.hpp"
#include "AgentBase.hpp"
#include "Data.hpp"
#include "ItemBase.hpp"
#include "WorldGrid.hpp"
#include "../DataCollection/AgentReciever.hpp"
#include "DataCollection/DataManager.hpp"
#include "Interfaces/NetWorth/server/ServerManager.hpp"
#include "Interfaces/NetWorth/client/ClientManager.hpp"
#include "Interfaces/NetWorth/client/ControlledAgent.hpp"
#include "Interfaces/NetWorth/client/ClientInterface.hpp"

// Forward declaration
namespace worldlang{
	class ProgramExecutor;
}

namespace cse491 {
class DataReceiver;

class WorldBase {
public:
  static constexpr size_t npos = static_cast<size_t>(-1);
  netWorth::ServerManager *server_manager = nullptr;        /// Server manager for world if used
  netWorth::ClientManager *client_manager = nullptr;        /// Client manager for world if used

  /// Helper function that is run whenever a new agent is created.
  /// @note Override this function to provide agents with actions or other
  /// setup.
  virtual void ConfigAgent(AgentBase & /* agent */) const {}

protected:
  /// Derived worlds may choose to have more than one grid.
  std::unordered_map<size_t, WorldGrid> grids;

  WorldGrid &main_grid; ///< Main grid for this world; shortcut to `grids["main"]`
  type_options_t type_options; ///< Vector of types of cells in grids for this world.

  item_map_t item_map;          ///< Map of IDs to pointers to non-agent entities
  agent_map_t agent_map;        ///< Map of IDs to pointers to agent entities
  size_t last_entity_id = 0;     ///< The last Entity ID used; increment at each creation

  bool run_over = false;        ///< Should the run end?

  bool world_running = true; ///< Is the world currently running?

  std::string action;           ///< The action that the agent is currently performing
  std::shared_ptr<DataCollection::AgentReceiver> agent_receiver;

  unsigned int seed;            ///< Seed used for generator
  std::mt19937 random_gen;      ///< Random number generator
  std::uniform_real_distribution<> uni_dist; ///< Uniform distribution of doubles, 0 to 1
  std::normal_distribution<> norm_dist;      ///< Normal distribution; mean 0, std 1

  /// Helper function to set the next entity id.
  size_t NextEntityID() { return ++last_entity_id; }

  /// Helper function that is run whenever a new agent is created.
  /// @note Override this function to provide agents with actions or other setup.
  virtual void ConfigAgent(AgentBase & /* agent */) {}

  /// @brief  Add a new type of cell to this world.
  /// @param name A unique name for this cell type
  /// @param desc A longer description of the cell type
  /// @param symbol An (optional) unique symbol for text IO (files, command
  /// line)
  /// @return A unique ID associated with this cell type (position in
  /// type_options vector)
  size_t AddCellType(const std::string &name, const std::string &desc = "",
                     char symbol = '\0') {
    type_options.push_back(CellType{name, desc, symbol});
    return type_options.size() - 1;
  }

public:
  /// Initializes world with cell types and random generator
  /// @param seed Seed used for RNG. Use 0 for a non-deterministic result.
  WorldBase(unsigned int seed=0)
    : grids(), main_grid(grids[0]), seed(seed)
  {
    // The first cell type (ID 0) should be reserved for errors or empty spots in a grid.
    AddCellType("Unknown", "This is an invalid cell type and should not be reachable.");

    // Initialize the random number generator.
    if (seed == 0) {
      std::random_device rd; // An expensive "true" random number generator.
      seed = rd();           // Change the seed to a random value.
    }
    random_gen.seed(seed);
  }

  virtual ~WorldBase() = default;

  virtual void Reset() {
    item_map.clear();
    agent_map.clear();
    last_entity_id = 0;
    run_over = false;
  }

  // -- Accessors --

  /// Get the total number of NON-agent entities
  [[nodiscard]] size_t GetNumItems() const { return item_map.size(); }

  /// Get the total number of AGENT entities
  [[nodiscard]] size_t GetNumAgents() const { return agent_map.size(); }

  /// Does an item with the provided ID exist?
  [[nodiscard]] bool HasItem(size_t id) const { return item_map.count(id); }

  /// Does an agent with the provided ID exist?
  [[nodiscard]] bool HasAgent(size_t id) const { return agent_map.count(id); }

  /// Return a reference to an agent with a given ID.
  [[nodiscard]] ItemBase & GetItem(size_t id) {
    assert(HasItem(id));
    return *item_map[id];
  }

  /// Return a reference to an agent with a given ID.
  [[nodiscard]] AgentBase & GetAgent(size_t id) {
    assert(HasAgent(id));
    return *agent_map[id];
  }

  /// Return the ID of an item with a given name.
  [[nodiscard]] size_t GetItemID(const std::string & name) {
    for (auto & [id, ptr] : item_map) {
      if (ptr->GetName() == name) return id;
    }
    return npos;
  }

  /// Return the ID of an agent with a given name.
  [[nodiscard]] size_t GetAgentID(const std::string & name) {
    for (auto & [id, ptr] : agent_map) {
      if (ptr->GetName() == name) return id;
    }
    return npos;
  }

  /// Return an editable version of the current grid for this world (main_grid by default)
  virtual WorldGrid & GetGrid() { return main_grid; }
  virtual WorldGrid & GetGrid(size_t grid_id) { return grids[grid_id]; }

  /// Return a const grid for this world (main_grid by default)
  virtual const WorldGrid & GetGrid() const { return main_grid; }
  virtual const WorldGrid & GetGrid(size_t grid_id) const { return grids.at(grid_id); }

  /// Determine if the run has ended.
  virtual bool GetRunOver() const { return run_over; }

  // -- Random Number Generation --

  /// @brief  Get the seed used to initialize this RNG
  unsigned int GetSeed() const { return seed; }

  /// @brief  Return a uniform random value between 0.0 and 1.0
  double GetRandom() { return uni_dist(random_gen); }

  /// @brief  Return a uniform random value between 0.0 and max
  double GetRandom(double max) { return GetRandom() * max; }

  /// @brief  Return a uniform random value between min and max
  double GetRandom(double min, double max) {
    assert(max > min);
    return min + GetRandom(max - min);
  }

  /// @brief  Return a gaussian random value with mean 0.0 and sd 1.0
  double GetRandomNormal() { return norm_dist(random_gen); }

  /// @brief  Return a gaussian random value with provided mean and sd.
  double GetRandomNormal(double mean, double sd = 1.0) {
    assert(sd > 0);
    return mean + norm_dist(random_gen) * sd;
  }

  // -- Agent Management --

  /// @brief Build a new agent of the specified type
  /// @tparam AGENT_T The type of agent to build
  /// @tparam PROPERTY_Ts Types for any properties to set at creation
  /// (automatic)
  /// @param agent_name The name of this agent
  /// @param properties Name/value pairs for any properties set at creation
  /// @return A reference to the newly created agent
  template<typename AGENT_T, typename... PROPERTY_Ts>
  AgentBase & AddAgent(std::string agent_name = "None", PROPERTY_Ts... properties) {
    std::mutex agent_map_lock;
    agent_map_lock.lock();
    const size_t agent_id = NextEntityID();
    auto agent_ptr = std::make_unique<AGENT_T>(agent_id, agent_name);
    agent_ptr->SetWorld(*this);
    agent_ptr->SetProperties(std::forward<PROPERTY_Ts>(properties)...);
    ConfigAgent(*agent_ptr);
    if (agent_ptr->Initialize() == false) {
      std::cerr << "Failed to initialize agent '" << agent_name << "'."
                << std::endl;
    }
    agent_map[agent_id] = std::move(agent_ptr);
      AgentBase & agentReturn = *agent_map[agent_id];
      agent_map_lock.unlock();
    return agentReturn;
  }

  /// @brief Add a new, already-built item
  /// @return A reference to the newly created item
  ItemBase & AddItem(std::unique_ptr<ItemBase> item_ptr) {
    assert(item_ptr);                // item_ptr must not be null.
    assert(item_ptr->GetID() != 0);  // item_ptr must have had a non-zero ID assigned.
    item_ptr->SetWorld(*this);
    size_t item_id = item_ptr->GetID();
    item_map[item_id] = std::move(item_ptr);
    return *item_map[item_id];
  }

  /// @brief Build a new item and add it
  /// @tparam PROPERTY_Ts Types for any properties to set at creation (automatic)
  /// @param item_name The name of this item
  /// @param properties Name/value pairs for any properties set at creation
  /// @return A reference to the newly created item
  template <typename ITEM_T=ItemBase, typename... PROPERTY_Ts>
  ItemBase & AddItem(std::string item_name="None", PROPERTY_Ts... properties) {
    auto item_ptr = std::make_unique<ITEM_T>(NextEntityID(), item_name);
    item_ptr->SetProperties(std::forward<PROPERTY_Ts>(properties)...);
    return AddItem(std::move(item_ptr));
  }

  /// @brief Remove an agent from the agent map
  /// @param agent_id The unique ID this agent
  /// @return A reference to this world.
  WorldBase & RemoveAgent(size_t agent_id) {
    agent_map.erase(agent_id);
    return *this;
  }

  /// @brief Remove an item from the item map
  /// @param item_id The unique ID this item
  /// @return A reference to this world.
  WorldBase & RemoveItem(size_t item_id) {
    item_map.erase(item_id);
    return *this;
  }
  
  /// @brief Remove an agent from the agent map by name
  /// @param agent_name The name of this agent
  /// @return This world
  WorldBase &RemoveAgent(std::string agent_name = "None") {
    assert(agent_name != "Interface"); // We are not allowed to remove interfaces.
    return RemoveAgent(GetAgentID(agent_name));
  }

  /// @brief Remove an item from the item map by name
  /// @param item_id The ID of this item
  /// @return This world
  WorldBase &RemoveItem(std::string item_name) {
    return RemoveItem(GetItemID(item_name));
  }
  
  WorldBase & AddItemToGrid(size_t item_id, GridPosition pos, size_t grid_id=0) {
    item_map[item_id]->SetPosition(pos, grid_id);
    return *this;
  }

  // -- Action Management --

  /// @brief Central function for an agent to take any action
  /// @param agent The specific agent taking the action
  /// @param action The id of the action to take
  /// @return The result of this action (usually 0/1 to indicate success)
  /// @note Thus function must be overridden in any derived world.
  virtual int DoAction(AgentBase &agent, size_t action_id) = 0;

  /// @brief Step through each agent giving them a chance to take an action.
  /// @note Override this function if you want to control which grid the agents
  /// receive.
  virtual void RunAgents() {
    for (auto & [id, agent_ptr] : agent_map) {
      size_t action_id = agent_ptr->SelectAction(main_grid, type_options, item_map, agent_map);
      agent_ptr->storeActionMap(agent_ptr->GetName());
      int result = DoAction(*agent_ptr, action_id);
      agent_ptr->SetActionResult(result);
    }
  }

  /// @brief RunAgents, but with extra features for client-side
  /// @note Override this function if you want to control which grid the agents receive.
  virtual void RunClientAgents() {
    for (auto & [id, agent_ptr] : agent_map) {
      size_t action_id = agent_ptr->SelectAction(main_grid, type_options, item_map, agent_map);
      agent_ptr->storeActionMap(agent_ptr->GetName());
      int result = DoAction(*agent_ptr, action_id);
      agent_ptr->SetActionResult(result);
    }

    // Deserialize agents
    std::string data = client_manager->getSerializedAgents();
    if (data.substr(0, 18) == ":::START agent_set") {
      std::istringstream is(data);
      DeserializeAgentSet(is, client_manager);
    }
  }

  /// @brief RunAgents, but with extra features for server-side
  /// @note Override this function if you want to control which grid the agents receive.
  virtual void RunServerAgents() {
    std::set<size_t> to_delete;

    for (auto & [id, agent_ptr] : agent_map) {
      // wait until clients have connected to run
      while (!server_manager->hasAgentsPresent()|| !world_running) {}

      // select action and send to client
      size_t action_id = agent_ptr->SelectAction(main_grid, type_options, item_map, agent_map);
		server_manager->writeToActionMap(id, action_id);
      agent_ptr->storeActionMap(agent_ptr->GetName());
      int result = DoAction(*agent_ptr, action_id);
      agent_ptr->SetActionResult(result);

      // mark agent for deletion if client disconnects
      if (action_id == 9999) to_delete.insert(id);
    }

    // delete agents
    for (size_t id : to_delete) {
        RemoveAgent(id);
    }

    // send updates to client for deleted agents
    if (!to_delete.empty()) {
        std::ostringstream os;
        SerializeAgentSet(os);
        std::string data = os.str();
		server_manager->setSerializedAgents(data);
		server_manager->setNewAgent(true);
		server_manager->sendGameUpdates();
    }
  }

  void CollectData() {
      for (const auto & [id, agent_ptr] : agent_map) {
          DataCollection::DataManager::GetInstance().GetAgentReceiver().StoreData(
          agent_ptr->GetName(), 
          agent_ptr->GetPosition(), agent_ptr->GetActionResult()
        );
      }
  }

  /// @brief UpdateWorld() is run after every agent has a turn.
  /// Override this function to manage background events for a world.
  virtual void UpdateWorld() {}

  /// @brief Run all agents repeatedly until an end condition is met.
  virtual void Run() {
    run_over = false;
    while (!run_over) {
      RunAgents();
      CollectData();
      UpdateWorld();
    }
  }

  /// @brief Run, but for client-side
  virtual void RunClient(netWorth::ClientManager *manager) {
    run_over = false;
    client_manager = manager;
    while (!run_over) {
      if (world_running){
        RunClientAgents();
        CollectData();
        UpdateWorld();
      }
    }
  }

  /// @brief Run, but for server-side
  virtual void RunServer(netWorth::ServerManager *manager) {
    run_over = false;
    server_manager = manager;
    while (!run_over) {
      if (world_running){
        RunServerAgents();
        CollectData();
        UpdateWorld();
      }
    }
  }

  /// @brief Set if world is running or not for concurrency purposes
  virtual void SetWorldRunning(bool running){
	  world_running = running;
  }

  // CellType management.

  // Return a const vector of all of the possible cell types.
  [[nodiscard]] const type_options_t &GetCellTypes() const {
    return type_options;
  }

  /// @brief Return the ID associated with the cell type name.
  /// @param name The unique name of the cell type
  /// @return The unique ID of the cell type (or 0 if it doesn't exist.)
  [[nodiscard]] size_t GetCellTypeID(const std::string &name) const {
    for (size_t i = 1; i < type_options.size(); ++i) {
      if (type_options[i].name == name)
        return i;
    }
    return 0;
  }

  [[nodiscard]] const std::string &GetCellTypeName(size_t id) const {
    if (id >= type_options.size())
      return type_options[0].name;
    return type_options[id].name;
  }

  [[nodiscard]] char GetCellTypeSymbol(size_t id) const {
    if (id >= type_options.size())
      return type_options[0].symbol;
    return type_options[id].symbol;
  }


  // -- Grid Analysis Helpers --

  /// @brief Lookup IDs for all items at a given grid position.
  /// @param pos Grid position to look up.
  /// @param grid_id ID of grid we are examining (default: main grid)
  /// @return A vector of item IDs at the target position.
  [[nodiscard]] virtual std::vector<size_t> FindItemsAt(GridPosition pos, size_t grid_id=0) const {
    std::vector<size_t> item_ids;
    for (const auto & [id, item_ptr] : item_map) {
      if (item_ptr->IsOnGrid(grid_id) && item_ptr->GetPosition() == pos) item_ids.push_back(id);
    }
    return item_ids;
  }

  /// @brief Lookup IDs for all agents at a given grid position.
  /// @param pos Grid position to look up.
  /// @return A vector of agent IDs at the target position.
  [[nodiscard]] virtual std::vector<size_t> FindAgentsAt(GridPosition pos, size_t grid_id=0) const {
    std::vector<size_t> agent_ids;
    for (const auto & [id, agent_ptr] : agent_map) {
      if (agent_ptr->IsOnGrid(grid_id) && agent_ptr->GetPosition() == pos) agent_ids.push_back(id);
    }
    return agent_ids;
  }

  /// @brief Lookup IDs for all items near a given grid position.
  /// @param pos Grid position to look up.
  /// @param dist Maximum distance away from pos for an item to be included.
  /// @return A vector of item IDs within dist of the target position.
  [[nodiscard]] virtual std::vector<size_t> FindItemsNear(GridPosition pos, double dist=1.0, size_t grid_id=0) const {
    std::vector<size_t> item_ids;
    for (const auto & [id, item_ptr] : item_map) {
      if (item_ptr->IsOnGrid(grid_id) && item_ptr->GetPosition().IsNear(pos, dist)) {
        item_ids.push_back(id);
      }
    }
    return item_ids;
  }

  /// @brief Lookup IDs for all agents near a given grid position.
  /// @param pos Grid position to look up.
  /// @param dist Maximum distance away from pos for an agent to be included.
  /// @return A vector of agent IDs within dist of the target position.
  [[nodiscard]] virtual std::vector<size_t> FindAgentsNear(GridPosition pos, double dist=1.0, size_t grid_id=0) const {
    std::vector<size_t> agent_ids;
    for (const auto & [id, agent_ptr] : agent_map) {
      if (agent_ptr->IsOnGrid(grid_id) && agent_ptr->GetPosition().IsNear(pos, dist)) {
        agent_ids.push_back(id);
      }
    }
    return agent_ids;
  }

  /// @brief Determine if this tile can be walked on, defaults to every tile is walkable
  /// @author @mdkdoc15
  /// @param pos The grid position we are checking
  /// @return If an agent should be allowed on this square
  [[nodiscard]] virtual bool IsTraversable(const AgentBase & /*agent*/, cse491::GridPosition /*pos*/) const {
    return true;
  }

  // -- Network Serialization and Deserialization --

  /// @brief Serialize agent data into an ostream
  /// @param os ostream
  void SerializeAgentSet(std::ostream &os) {
      os << ":::START agent_set\n";
      os << last_entity_id << '\n';

      for (const auto &agent: agent_map) {
        agent.second->Serialize(os);
      }
      os << ":::END agent_set\n";
  }

  /// @brief Add deserialized agents to world with a manager
  /// @param is istream
  /// @param manager ClientManager which manages ControlledAgents
  void DeserializeAgentSet(std::istream &is, netWorth::ClientManager *manager) {
    // find beginning of agent_set serialization
    std::string read;
    std::getline(is, read, '\n');
    if (read != ":::START agent_set") {
      std::cerr << "Could not find start of agent_set serialization" << std::endl;
      return;
    }

    std::string name, x, y, id_string, symbol_string;
    size_t size, id;
    size_t client_id = manager->getClientID();

    // remove all agents that are NOT the interface
    std::set<size_t> to_delete;
    for (auto &pair : agent_map) {
      if (pair.first != client_id) to_delete.insert(pair.first);
    }

    for (size_t agent_id : to_delete) {
      RemoveAgent(agent_id);
    }

    // reset last_entity_id; start from the beginning
    last_entity_id = 0;

    // last entity id in server agent map
    std::getline(is, read, '\n');
    size = stoi(read);

    // client id NOT in agent map yet if ID = 0
    // append to end of set
    if (client_id == 0) client_id = size;

    // read each agent (only deserialize name, id, x, y for now)
    for (size_t i = 0; i < size; i++) {
      // read name (may incidentally reach end of set)
      std::getline(is, name, '\n');
      if (name == ":::END agent_set") {
        last_entity_id = client_id;
        return;
      }

      // read id, x, y, and symbol
      std::getline(is, id_string, '\n');
      std::getline(is, x, '\n');
      std::getline(is, y, '\n');
      std::getline(is, symbol_string, '\n');

      // may be gaps between IDs
      id = stoi(id_string);
      while (last_entity_id + 1 != id){
        last_entity_id++;
      }

      // is this new agent NOT the client interface
      if (last_entity_id + 1 != client_id) {
        AddAgent<netWorth::ControlledAgent>(name, "manager", manager).SetPosition(stoi(x), stoi(y)).SetProperty("symbol", symbol_string[0]);
      } else {
        // client interface still exists; do nothing
        last_entity_id++;
      }
    }

    // find end of agent_set deserialization
    std::getline(is, read, '\n');
    if (read != ":::END agent_set") {
      std::cerr << "Could not find end of agent_set serialization" << std::endl;
      return;
    }
  }

  /// @brief Serialize item data into an ostream
  /// @param os ostream
  void SerializeItemSet(std::ostream &os) {
    os << ":::START item_set\n";
    os << item_map.size() << '\n';

    for (const auto &item: item_map) {
        item.second->Serialize(os);
    }
    os << ":::END item_set\n";
  }

  /// @brief Add deserialized items to world
  /// @param istream
  void DeserializeItemSet(std::istream &is) {
    // find beginning of item_set serialization
    std::string read;
    std::getline(is, read, '\n');
    if (read != ":::START item_set") {
      std::cerr << "Could not find start of item_set serialization" << std::endl;
      return;
    }

    // how many items?
    size_t size;
    std::getline(is, read, '\n');
    size = stoi(read);

    // read each item
    for (size_t i = 0; i < size; i++) {
      auto item = std::make_unique<ItemBase>(agent_map.size() + i, "");
      item->Deserialize(is);
      AddItem(std::move(item));
    }

    // find end of item_set serialization
    std::getline(is, read, '\n');
    if (read != ":::END item_set") {
      std::cerr << "Could not find end of item_set serialization" << std::endl;
      return;
    }
  }

  /// @brief Serialize world, agents, and items into ostream
  /// @param os ostream
  void Serialize(std::ostream &os) {
    main_grid.Serialize(os);
    SerializeAgentSet(os);
    SerializeItemSet(os);
  }

  /// @brief Deserialize world, agents, and items from istream
  /// @param manager ClientManager for ControlledAgents
  void Deserialize(std::istream &is, netWorth::ClientManager *manager) {
    main_grid.Deserialize(is);
    DeserializeAgentSet(is, manager);
    DeserializeItemSet(is);
  }

  // Needs access to most things here so this is easiest way to do so
  friend worldlang::ProgramExecutor;
};

} // End of namespace cse491
