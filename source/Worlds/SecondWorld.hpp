/**
 * @file SecondWorld.hpp
 * @author Jayson Van Dam
 * @author Kurt LaBlanc
 * @author Satvik Ravipati
 */

#pragma once
#include <algorithm>
#include <nlohmann/json.hpp>

#include "../Agents/PacingAgent.hpp"
#include "../core/WorldBase.hpp"
#include "ProgramExecutor.hpp"  //< for attack, item scripts

using cse491::CellType;

namespace group4 {
/// @brief Filename for the first floor grid file
const std::string FIRST_FLOOR_FILENAME = "../assets/grids/group4_maze.grid";

/// @brief Filename for the second floor grid file
const std::string SECOND_FLOOR_FILENAME = "../assets/grids/second_floor.grid";

/// @brief Filename for the last floor grid file
const std::string FINAL_FLOOR_FILENAME = "../assets/grids/third_floor.grid";

/// @brief Filename for item pickup script
///
/// This script modifies properties of the agent collecting the item.
const std::string ITEM_PICKUP_SCRIPT = "../assets/scripts/g4_item_pickup.ws";

/// @brief Filename for agent combat script
///
/// When an agent attempts to move into another agent, this attack script runs.
const std::string COMBAT_SCRIPT = "../assets/scripts/g4_agent_attack.ws";

/// @brief Filename for world initialization script
const std::string WORLD_LOAD_SCRIPT = "../assets/scripts/g4_world_load.ws";

/// @brief Filename for second floor initialization script
const std::string WORLD_LOAD_SCRIPT_2 = "../assets/scripts/g4_world_2_load.ws";

/// @brief Filename for final floor initialization script
const std::string WORLD_LOAD_SCRIPT_3 = "../assets/scripts/g4_world_3_load.ws";

/// @brief Maximum inventory size for an agent
const size_t MAX_INVENTORY_SIZE = 30;

/// @brief Off the grid position
const cse491::GridPosition OffGrid = {-1, -1};

/**
 * Creates a world with agents and a win flag
 */
class SecondWorld : public cse491::WorldBase {
 private:
  /// @brief File name for this world grid. Switched when the player gets to the
  /// next level.
  std::string world_filename = "";

  /// @brief File name for agent input JSON file.
  std::string agents_filename = "";

  
  void LoadWorldScript(std::string source){
    // Clear items on the current grid, unless held by a player
    for (auto it = item_map.begin(); it != item_map.end();) {
      auto& item = it->second;
      if (item->IsOnGrid()) {
        // clear this item
        it = item_map.erase(it);
      } else if (item->IsOwnedByItem()) {
        // it's owned by a chest: this is currently the only item containing
        // items
        it = item_map.erase(it);
      } else {
        ++it;
      }
    }

    // Clear agents, unless they are the player (an interface)
    std::vector<size_t> agents_to_remove = {};
    for (auto& [agent_id, agent_ptr] : agent_map) {
      if (!agent_ptr->IsInterface()) {
        agents_to_remove.push_back(agent_id);
      }
    }

    for (auto agent_id : agents_to_remove) {
      RemoveAgent(agent_id);
    }
    
    // Initialize level
    pe.runFile(source);
  }
  
  
  /**
   * Switches to the next grid file
   * and resets player position and item map.
   * @param agent The player agent
   * @param pos The player position
   */
  void SwitchGrid(cse491::AgentBase& agent) {

    // This value initialized by each world load script to point to the next level
    auto next_world_script = pe.var<std::string>("next_world");

    agent.Notify("Going to " + world_filename, "world_switched");
    if (next_world_script == "GAME_END") {
      agent.Notify("Congrats, you won the game!", "congrats_msg");
      run_over = true;
      return;
    }

    LoadWorldScript(next_world_script);
  }

 protected:
  enum ActionType {
    REMAIN_STILL = 0,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    DROP_ITEM,
//    WARP_TO_FLOOR_3  // New action for hidden warp tile
  };

  /// Script executor object.
  worldlang::ProgramExecutor pe;

  /// Vector of the items in this world
  std::map<size_t, std::unique_ptr<cse491::ItemBase>> inventory;

  /// Provide the agent with movement actions.
  void ConfigAgent(cse491::AgentBase& agent) override {
    agent.AddAction("up", MOVE_UP);
    agent.AddAction("down", MOVE_DOWN);
    agent.AddAction("left", MOVE_LEFT);
    agent.AddAction("right", MOVE_RIGHT);
    agent.AddAction("drop", DROP_ITEM);
//    agent.AddAction("warp", WARP_TO_FLOOR_3);
  }

 public:
  /**
   * Constructor with no arguments
   */
  SecondWorld() : world_filename(FIRST_FLOOR_FILENAME), pe{*this} {
    pe.registerFunction("loadAgents", [this](worldlang::ProgramExecutor& pe){
      auto args = pe.popArgs();
      if (args.size() != 1) { pe.error("Wrong number of arguments!"); return; }
      auto path = pe.as<std::string>(args[0]);
      
      this->LoadFromFile(path);
    });
    
    LoadWorldScript(WORLD_LOAD_SCRIPT);
  }

  /**
   * Constructor with grid and agent file names
   * @param grid_filename Relative path to grid file
   * @param agent_filename Relative path to agent input.json file
   */
  SecondWorld(std::string grid_filename, std::string agent_filename)
      : world_filename(grid_filename),
        agents_filename(agent_filename),
        pe{*this} {
    AddCellType("floor", "Floor that you can easily walk over.", ' ');
    AddCellType("flag", "Goal flag for a game end state", 'g');
    AddCellType("wall", "Impenetrable wall that you must find a way around.", '#');
    AddCellType("hidden_warp", "Hidden warp tile that warps to floor 3.", 'u');
    AddCellType("water", "Water that distinguishes fire.", 'w');

    main_grid.Read(grid_filename, type_options);
    LoadFromFile(agent_filename);
  }

  /**
   * Destructor
   */
  ~SecondWorld() = default;

  /**
   * Loads data from a JSON file
   * and adds agents with specified properties into the world.
   * @param input_filename Relative path to input.json file
   */
  void LoadFromFile(const std::string& input_filename) {
    std::ifstream input_file(input_filename);

    if (!input_file.is_open()) {
      std::cerr << "Error: could not open file " << input_filename << std::endl;
      return;
    }

    nlohmann::json data;
    try {
      input_file >> data;
    } catch (const nlohmann::json::parse_error& err) {
      std::cerr << "JSON parsing error: " << err.what() << std::endl;
      return;
    }

    /// The base max health for an agent.
    /// Can be added to by giving the agent specific items,
    /// such as "chocolate_bar", that are specified below.
//    const int BASE_MAX_HEALTH = 100;
    for (const auto& agent : data) {
      // May get a json.exception.type_error here if you assign to the wrong C++
      // type, so make sure to nail down what types things are in JSON first! My
      // intuition is that each agent object will have: name: string (C++
      // std::string) x: number (C++ int) y: number (C++ int) entities:
      // array<string> (C++ std::vector<std::string>)
      std::string agent_name = agent.at("name");
      int x_pos = agent.at("x");
      int y_pos = agent.at("y");

//      int additional_max_health = 0;
//      std::vector<std::string> entities = agent.at("entities");


//      for (const auto& entity : entities) {
        // TODO: How should we set the entity properties here?
        // Just adding to MaxHealth now, but this doesn't seem very scalable.
/*        if (entity == "chocolate_bar") {
          additional_max_health = 10;
        }*/
//      }

      auto& a = AddAgent<cse491::PacingAgent>(agent_name)
          .SetPosition(x_pos, y_pos);
//          .SetProperty("MaxHealth", BASE_MAX_HEALTH + additional_max_health);

      auto properties = agent.at("properties");
      for (const auto& p : properties.items()){
        std::cout << p.value().is_number() << std::endl;
        a.SetProperty(p.key(), p.value().get<double>());
      }

    }
  }

  /**
   * This function gives us an output.json file
   */
  virtual void Run() override {
    while (!run_over) {
      RunAgents();
    }

    SaveToFile();
  }

  /**
   * This function gives us an output.json file using nlohmann::json library
   */
  void SaveToFile() {
    nlohmann::json output_data;  // json to store the data being outputted

    for (const auto& [agent_id, agent_ptr] : agent_map) {
      auto new_position = agent_ptr->GetPosition();
      std::string agent_name = agent_ptr->GetName();
      double x_pos = new_position.GetX();
      double y_pos = new_position.GetY();

      nlohmann::json agent_data;  // json for each agent
      agent_data["name"] = agent_name;
      agent_data["x"] = x_pos;
      agent_data["y"] = y_pos;

      output_data.push_back(agent_data);  // add it to the json array
    }

    std::ofstream ofs("output.json");  // save it to a file called output.json
    ofs << output_data.dump(2);        // indentation
  }

  /**
   * Drops the item in the agent's inventory
   * @param agent This agent's item we're dropping
   * @param pos The position where the item will be dropped
   */
  void DropItem(cse491::AgentBase& agent, cse491::GridPosition& pos) {
    // Cannot drop
    if (agent.GetInventory().empty()) {
      agent.Notify("Cannot drop any items, inventory is empty.", "item_alert");
      return;
    }

    auto items_found = FindItemsAt(pos, 0);
    auto& item_drop = GetItem(agent.GetInventory().at(0));

    // Transfer ownership to chest
    if (!items_found.empty()) {
      auto& target_item = GetItem(items_found.at(0));
      if (target_item.HasProperty("Chest")) {
        item_drop.SetPosition(OffGrid);
        target_item.AddItem(item_drop);

        // Set the position and remove item from agent's inventory
        agent.RemoveItem(item_drop.GetID());

        // Must set the grid back because RemoveItem() doesn't account for that
        item_drop.SetGrid();
        agent.Notify("Dropping " + item_drop.GetName() + " into the chest!",
                     "item_alert");
        return;

        // Item already on player's position
      } else {
        agent.Notify(
            "Cannot drop the item, there is already an item on this cell.",
            "item_alert");
        return;
      }
    }
    // Transfer ownership to grid
    agent.RemoveItem(item_drop.GetID());
    item_drop.SetGrid();
    item_drop.SetPosition(pos);

    agent.Notify("Dropping " + item_drop.GetName() + " onto the ground!",
                 "item_alert");
  }

  /**
   * Checks to see if there is a flag or item at the agent's current position
   * @param agent The agent
   * @param pos The position
   * @return true if agent should update position on return
   */
  bool CheckPosition(cse491::AgentBase& agent, cse491::GridPosition& pos) {
    // First check to see if agent is on win flag
    if ((type_options[main_grid.At(pos)].HasProperty("Goal")) &&
        (agent.IsInterface())) {
      agent.Notify("Flag found ", "item_alert");

      agent.Notify("Leaving " + world_filename, "world_switched");

      SwitchGrid(agent);
      
      return false;

      // then checks if agent is on any items
    } else if ((type_options[main_grid.At(pos)].HasProperty("Warp")) &&
               (agent.IsInterface())) {
      // Agent used the hidden warp tile action
      agent.Notify("Hidden warp tile activated! Warping to floor 3.",
                   "hidden_warp");

      agent.Notify("Leaving " + world_filename, "world_switched");

      LoadWorldScript(WORLD_LOAD_SCRIPT_3); // skip right to the end
      return false;
    } else {
      auto items_found = FindItemsAt(pos, 0);
      // If there are items at this position
      if (!items_found.empty() && agent.IsInterface()) {
        auto& item_found = GetItem(items_found.at(0));

        // Item is a chest
        if (item_found.HasProperty("Chest")) {
          // Check to see if the chest owns any items
          if (!item_found.GetInventory().empty()) {
            auto temp_inventory = item_found.GetInventory();
            agent.Notify("This is inside the chest: ", "item_alert");

            // Display the items found
            for (auto x : temp_inventory) {
              agent.Notify("You found the " + GetItem(x).GetName() +
                           " in the " + item_found.GetName());
              agent.AddItem(GetItem(x));
              item_found.RemoveItem(GetItem(x));
            }

            // Check agent's inventory size
            if (agent.GetInventory().size() == MAX_INVENTORY_SIZE) {
              agent.Notify(
                  "It looks like your inventory is full, please drop items or "
                  "place them in chests!",
                  "item_alert");
            }
          } else {
            agent.Notify("The chest is empty! You can store items with 't'!",
                         "item_alert");
          }

          // Item is not a chest
        } else {
          if (agent.GetInventory().size() == MAX_INVENTORY_SIZE) {
            agent.Notify(
                "It looks like your inventory is full, please drop items or "
                "place them in chests!",
                "item_alert");

          } else {
            agent.Notify("You found " + item_found.GetName() + "!",
                         "item_alert");

            // Set the position off the grid, so it doesn't render
            // Note: Setting the position with a GridPosition SETS THE OWNER
            item_found.SetPosition(OffGrid);
            // Add item to the agent's inventory
            agent.AddItem(item_found.GetID());

          }
        }
      }
    }
    return true;
  }

  /**
   * Allows agents to perform an action and sets each agent's new position
   * @param agent The agent performing the action
   * @param action_id The id that represents the move
   * @return An int that declares if the move is legal or illegal (true / false)
   */
  int DoAction(cse491::AgentBase& agent, size_t action_id) override {
    if (agent.HasProperty("Dead")){
      return false; // Can't move when dead!
    }
    
    cse491::GridPosition new_position;
    bool IsDropped = false;
    switch (action_id) {
      case REMAIN_STILL:
        new_position = agent.GetPosition();
        break;
      case MOVE_UP:
        new_position = agent.GetPosition().Above();
        break;
      case MOVE_DOWN:
        new_position = agent.GetPosition().Below();
        break;
      case MOVE_LEFT:
        new_position = agent.GetPosition().ToLeft();
        break;
      case MOVE_RIGHT:
        new_position = agent.GetPosition().ToRight();
        break;
      case DROP_ITEM:
        new_position = agent.GetPosition();
        DropItem(agent, new_position);
//        IsDropped = true;
        return true;
        break;
    }

    if (!main_grid.IsValid(new_position)) {
      return false;
    }
    if (!IsTraversable(agent, new_position)) {
      return false;
    }

    // At this point, new_position is valid and not going into a wall.
    // Check if there are any agents on this tile:
    auto res = this->FindAgentsAt(new_position);
    if (res.size() && res[0] != agent.GetID()) {
      // At least one agent was found (and isn't the player)
      // Take the first agent and attack it.
      pe.setVariable("agent", agent.GetID());
      pe.setVariable("opponent", res[0]);
      pe.runFile(COMBAT_SCRIPT);

      auto& opponent = GetAgent(res[0]);
      if (opponent.HasProperty("Dead") && opponent.IsInterface()){
        // game over!
        opponent.Notify("You have died.", "game_over");
        run_over = true;
      } else if (opponent.HasProperty("Dead") && opponent.HasProperty("Boss")){
        // level up: i.e. you can go on water now
        agent.Notify("You have defeated the boss! You now can walk on water.", "message");
        agent.SetProperty("Swimmer", true);
      }

      // The movement was not legal, so we return false.
      // TODO: Should this return a status indicating that an attack occured,
      // to distinguish moves that do nothing from attacks?
      return false;
    }

    if (!IsDropped) {
      if (!CheckPosition(agent, new_position))
        //does not set position if grid changed, but move is considered successful
        return true; 
    }

    IsDropped = false;
    agent.SetPosition(new_position);
    return true;
  }

  /**
   * Prints the entities in item_set (testing)
   */
  void PrintEntities() {
    for (const auto& [id, item_ptr] : item_map) {
      if (!item_ptr) {
        continue;
      }
      std::cout << item_ptr->GetName() << "\n";
    }
    std::cout << std::endl;
  }

  /// Can walk on all tiles except for walls and water (unless agent has
  /// property set)
  bool IsTraversable(const AgentBase& agent,
                     cse491::GridPosition pos) const override {
    if (GetCellTypes().at(main_grid.At(pos)).HasProperty(CellType::CELL_WALL))
      return false;
    else if (GetCellTypes()
                 .at(main_grid.At(pos))
                 .HasProperty(CellType::CELL_WATER))
      return agent.HasProperty("Swimmer");
    else
      return true;
  }
};
}  // namespace group4
