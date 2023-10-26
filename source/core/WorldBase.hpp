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

#include "AgentBase.hpp"
#include "Data.hpp"
#include "ItemBase.hpp"
#include "WorldGrid.hpp"
#include "../DataCollection/AgentReciever.hpp"

namespace cse491 {

class DataReceiver;

class WorldBase {
public:
  static constexpr size_t npos = static_cast<size_t>(-1);

protected:
  /// Derived worlds may choose to have more than one grid.
  std::unordered_map<std::string, WorldGrid> grids;

  WorldGrid & main_grid;        ///< Main grid for this world; shortcut to `grids["main"]`
  type_options_t type_options;  ///< Vector of types of cells in grids for this world.

  item_set_t item_set;          ///< Vector of pointers to non-agent entities
  agent_set_t agent_set;        ///< Vector of pointers to agent entities

  bool run_over = false;        ///< Should the run end?
  
  std::string action;           ///< The action that the agent is currently performing
  std::shared_ptr<DataCollection::AgentReceiver> agent_receiver;

  unsigned int seed;            ///< Seed used for generator
  std::mt19937 random_gen;      ///< Random number generator
  std::uniform_real_distribution<> uni_dist; ///< Uniform distribution of doubles, 0 to 1
  std::normal_distribution<> norm_dist;      ///< Normal distribution; mean 0, std 1

  /// Helper function that is run whenever a new agent is created.
  /// @note Override this function to provide agents with actions or other setup.
  virtual void ConfigAgent(AgentBase & /* agent */) {}

  /// @brief  Add a new type of cell to this world.
  /// @param name A unique name for this cell type
  /// @param desc A longer description of the cell type
  /// @param symbol An (optional) unique symbol for text IO (files, command line)
  /// @return A unique ID associated with this cell type (position in type_options vector)
  size_t AddCellType(const std::string &name, const std::string &desc = "",
                     char symbol = '\0') {
    type_options.push_back(CellType{name, desc, symbol});
    return type_options.size() - 1;
  }

public:
  /// Initializes world with cell types and random generator
  /// @param seed Seed used for RNG. Use 0 for a non-deterministic result.
  WorldBase(unsigned int seed=0)
    : grids(), main_grid(grids["main"]), seed(seed)
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

  // -- Accessors --

  /// Get the total number of NON-agent entities
  [[nodiscard]] size_t GetNumItems() const { return item_set.size(); }

  /// Get the total number of AGENT entities
  [[nodiscard]] size_t GetNumAgents() const { return agent_set.size(); }

  /// Return a reference to an agent with a given ID.
  [[nodiscard]] ItemBase & GetItem(size_t id) {
    assert(id < item_set.size());
    return *item_set[id];
  }

  /// Return a reference to an agent with a given ID.
  [[nodiscard]] AgentBase &GetAgent(size_t id) {
    assert(id < agent_set.size());
    return *agent_set[id];
  }

  /// Return the ID of an item with a given name.
  [[nodiscard]] size_t GetItemID(const std::string & name) {
    for (size_t i = 0; i < item_set.size(); ++i) {
      if (item_set[i] && item_set[i]->GetName() == name) return i;      
    }
    return npos;
  }

  /// Return the ID of an agent with a given name.
  [[nodiscard]] size_t GetAgentID(const std::string & name) {
    for (size_t i = 0; i < agent_set.size(); ++i) {
      if (agent_set[i] && agent_set[i]->GetName() == name) return i;      
    }
    return npos;
  }
  

  /// Return an editable version of the current grid for this world (main_grid by default)
  virtual WorldGrid & GetGrid() { return main_grid; }
  virtual WorldGrid & GetGrid(const std::string & name) { return grids[name]; }

  /// Return a const grid for this world (main_grid by default)
  virtual const WorldGrid & GetGrid() const { return main_grid; }
  virtual const WorldGrid & GetGrid(const std::string & name) const { return grids.at(name); }

  /// Determine if the run has ended.
  virtual bool GetRunOver() const { return run_over; }

  // -- Random Number Generation --
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
  double GetRandomNormal(double mean, double sd=1.0) {
    assert(sd > 0);
    return mean + norm_dist(random_gen) * sd;
  }

 
  // -- Agent Management --

  /// @brief Build a new agent of the specified type
  /// @tparam AGENT_T The type of agent to build
  /// @tparam PROPERTY_Ts Types for any properties to set at creation (automatic)
  /// @param agent_name The name of this agent
  /// @param properties Name/value pairs for any properties set at creation
  /// @return A reference to the newly created agent
  template<typename AGENT_T, typename... PROPERTY_Ts>
  AgentBase & AddAgent(std::string agent_name = "None", PROPERTY_Ts... properties) {
    auto agent_ptr = std::make_unique<AGENT_T>(agent_set.size(), agent_name);
    agent_ptr->SetWorld(*this);
    agent_ptr->SetProperties(std::forward<PROPERTY_Ts>(properties)...);
    ConfigAgent(*agent_ptr);
    if (agent_ptr->Initialize() == false) {
      std::cerr << "Failed to initialize agent '" << agent_name << "'." << std::endl;
    }
    agent_set.emplace_back(std::move(agent_ptr));
    return *agent_set.back();
  }

  void SetAgentReceiver(DataCollection::AgentReceiver r) {
    agent_receiver = std::make_shared<DataCollection::AgentReceiver>(r);
  }

  /// @brief Add a new, previously-built item
  /// @return A reference to the newly created item
  ItemBase & AddItem(std::unique_ptr<ItemBase> item_ptr) {
    item_ptr->SetWorld(*this);
    item_set.emplace_back(std::move(item_ptr));
    return *item_set.back();
  }

  /// @brief Build a new item
  /// @tparam PROPERTY_Ts Types for any properties to set at creation (automatic)
  /// @param item_name The name of this item
  /// @param properties Name/value pairs for any properties set at creation
  /// @return A reference to the newly created item
  template <typename ITEM_T=ItemBase, typename... PROPERTY_Ts>
  ItemBase & AddItem(std::string item_name="None", PROPERTY_Ts... properties) {
    auto item_ptr = std::make_unique<ITEM_T>(item_set.size(), item_name);
    item_ptr->SetProperties(std::forward<PROPERTY_Ts>(properties)...);
    return AddItem(std::move(item_ptr));
  }

  /// @brief Remove an agent from the agent set
  /// @param agent_id The unique ID this agent
  /// @return A reference to this world.
  WorldBase & RemoveAgent(size_t agent_id) {
    agent_set[agent_id] = nullptr;
    return *this;
  }

  /// @brief Remove an item from the item set
  /// @param item_id The unique ID this item
  /// @return A reference to this world.
  WorldBase & RemoveItem(size_t item_id) {
    item_set[item_id] = nullptr;
    return *this;
  }
  
  /// @brief Remove an agent from the agent set by name
  /// @param agent_name The name of this agent
  /// @return This world
  WorldBase & RemoveAgent(std::string agent_name="None") {
    assert(agent_name != "Interface"); // We are not allowed to remove interfaces.
    return RemoveAgent( GetAgentID(agent_name) );
  }

  /// @brief Remove an item from the item set by name
  /// @param item_id The ID of this item
  /// @return This world
  WorldBase & RemoveItem(std::string item_name) {
    return RemoveItem( GetItemID(item_name) );
  }

  // -- Action Management --

  /// @brief Central function for an agent to take any action
  /// @param agent The specific agent taking the action
  /// @param action The id of the action to take
  /// @return The result of this action (usually 0/1 to indicate success)
  /// @note Thus function must be overridden in any derived world.
  virtual int DoAction(AgentBase & agent, size_t action_id) = 0;

  /// @brief Step through each agent giving them a chance to take an action.
  /// @note Override this function if you want to control which grid the agents receive.
  virtual void RunAgents() {
    for (size_t i=0; i < agent_set.size(); ++i) {
      if (!agent_set[i]) continue; // Skip over deleted agents.
      size_t action_id =
        agent_set[i]->SelectAction(main_grid, type_options, item_set, agent_set);
      agent_set[i]->storeActionMap(agent_set[i]->GetName());
      int result = DoAction(*agent_set[i], action_id);
      agent_set[i]->SetActionResult(result);
    }
  }

  void CollectData() {
    if(agent_receiver != nullptr){
      for (const auto & agent_ptr : agent_set) {
          agent_receiver->StoreData(agent_ptr->GetName(), 
              agent_ptr->GetPosition(), agent_ptr->GetActionResult());
      }
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

  // CellType management.

  // Return a const vector of all of the possible cell types.
  [[nodiscard]] const type_options_t &GetCellTypes() const { return type_options; }

  /// @brief Return the ID associated with the cell type name.
  /// @param name The unique name of the cell type
  /// @return The unique ID of the cell type (or 0 if it doesn't exist.)
  [[nodiscard]] size_t GetCellTypeID(const std::string &name) const {
    for (size_t i = 1; i < type_options.size(); ++i) {
      if (type_options[i].name == name) return i;
    }
    return 0;
  }

  [[nodiscard]] const std::string &GetCellTypeName(size_t id) const {
    if (id >= type_options.size()) return type_options[0].name;
    return type_options[id].name;
  }

  [[nodiscard]] char GetCellTypeSymbol(size_t id) const {
    if (id >= type_options.size()) return type_options[0].symbol;
    return type_options[id].symbol;
  }

  /// @brief Determine if this tile can be walked on, defaults to every tile is walkable
  /// @author @mdkdoc15
  /// @param pos The grid position we are checking
  /// @return If an agent should be allowed on this square
  virtual bool IsTraversable(const AgentBase & /*agent*/, cse491::GridPosition /*pos*/) const {
    return true;
  }

};

} // End of namespace cse491
