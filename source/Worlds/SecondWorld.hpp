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

namespace group4 {
/// @brief Filename for the first floor grid file
const std::string FIRST_FLOOR_FILENAME = "../assets/grids/group4_maze.grid";

/// @brief Filename for the second floor grid file
const std::string SECOND_FLOOR_FILENAME = "../assets/grids/second_floor.grid";

/// @brief Filename for the last floor grid file
const std::string FINAL_FLOOR_FILENAME = "../assets/grids/third_floor.grid";

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

  /**
   * Switches to the next grid file
   * and resets player position and item map.
   * @param agent The player agent
   * @param pos The player position
   */
  void SwitchGrid(cse491::AgentBase& agent, cse491::GridPosition& pos) {
    if (world_filename == FIRST_FLOOR_FILENAME) {
      world_filename = SECOND_FLOOR_FILENAME;
      agents_filename = "../assets/second_floor_input.json";
    } else if (world_filename == SECOND_FLOOR_FILENAME) {
      world_filename = FINAL_FLOOR_FILENAME;
      agents_filename = "../assets/third_floor_input.json";
    }

    agent.Notify("Going to " + world_filename, "world_switched");

    // Need to clear item_map so that items don't stay for the next floor.
    item_map.clear();

    // Resetting the current new_position to the top left of the new grid.
    pos = cse491::GridPosition(0, 0);
    main_grid.Read(world_filename, type_options);
    LoadFromFile(agents_filename);
  }

 protected:
  enum ActionType {
    REMAIN_STILL = 0,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    DROP_ITEM,
    WARP_TO_FLOOR_3  // New action for hidden warp tile
  };

  /// Easy access to floor CellType ID.
  size_t floor_id;

  /// Easy access to flag CellType ID.
  size_t flag_id;

  /// Easy access to wall CellType ID.
  size_t wall_id;

  /// Easy access to warp CellType ID.
  size_t hidden_warp_id;

  /// Provide the agent with movement actions.
  void ConfigAgent(cse491::AgentBase& agent) override {
    agent.AddAction("up", MOVE_UP);
    agent.AddAction("down", MOVE_DOWN);
    agent.AddAction("left", MOVE_LEFT);
    agent.AddAction("right", MOVE_RIGHT);
    agent.AddAction("drop", DROP_ITEM);
    // Add the new action for the hidden warp tile
    agent.AddAction("warp", WARP_TO_FLOOR_3);
  }

 public:
  /**
   * Constructor with no arguments
   */
  SecondWorld() : world_filename(FIRST_FLOOR_FILENAME) {
    floor_id =
        AddCellType("floor", "Floor that you can easily walk over.", ' ');
    flag_id = AddCellType("flag", "Goal flag for a game end state", 'g');
    wall_id = AddCellType(
        "wall", "Impenetrable wall that you must find a way around.", '#');
    hidden_warp_id = AddCellType(
        "hidden_warp", "Hidden warp tile that warps to floor 3.", 'u');

    main_grid.Read(FIRST_FLOOR_FILENAME, type_options);
  }

  /**
   * Constructor with grid and agent file names
   * @param grid_filename Relative path to grid file
   * @param agent_filename Relative path to agent input.json file
   */
  SecondWorld(std::string grid_filename, std::string agent_filename)
      : world_filename(grid_filename), agents_filename(agent_filename) {
    floor_id =
        AddCellType("floor", "Floor that you can easily walk over.", ' ');
    flag_id = AddCellType("flag", "Goal flag for a game end state", 'g');
    wall_id = AddCellType(
        "wall", "Impenetrable wall that you must find a way around.", '#');
    hidden_warp_id = AddCellType(
        "hidden_warp", "Hidden warp tile that warps to floor 3.", 'u');

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
    const int BASE_MAX_HEALTH = 100;
    for (const auto& agent : data) {
      // May get a json.exception.type_error here if you assign to the wrong C++
      // type, so make sure to nail down what types things are in JSON first! My
      // intuition is that each agent object will have: name: string (C++
      // std::string) x: number (C++ int) y: number (C++ int) entities:
      // array<string> (C++ std::vector<std::string>)
      std::string agent_name = agent.at("name");
      int x_pos = agent.at("x");
      int y_pos = agent.at("y");

      int additional_max_health = 0;
      std::vector<std::string> entities = agent.at("entities");

      for (const auto& entity : entities) {
        // TODO: How should we set the entity properties here?
        // Just adding to MaxHealth now, but this doesn't seem very scalable.
        if (entity == "chocolate_bar") {
          additional_max_health = 10;
        }
      }

      AddAgent<cse491::PacingAgent>(agent_name)
          .SetPosition(x_pos, y_pos)
          .SetProperty("MaxHealth", BASE_MAX_HEALTH + additional_max_health);
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
  void DropItem(cse491::AgentBase& agent, cse491::GridPosition& pos, size_t item_id=0) {
    // Cannot drop
    if (agent.GetInventory().empty()) {
      agent.Notify("Cannot drop any items, inventory is empty.", "item_alert");
      return;
    }

    auto items_found = FindItemsAt(pos, 0);
    auto &item_drop = GetItem(agent.GetInventory().at(0));

    // Transfer ownership to chest
    if (!items_found.empty()) {
      auto &target_item = GetItem(items_found.at(item_id));
      if (target_item.HasProperty("Chest")) {

        item_drop.SetPosition(OffGrid);
        target_item.AddItem(item_drop);

        // Set the position and remove item from agent's inventory
        agent.RemoveItem(item_drop.GetID());

        // Must set the grid back because RemoveItem() doesn't account for that
        item_drop.SetGrid();
        agent.Notify("Dropping " + item_drop.GetName() + " into the chest!", "item_alert");
        return;

      // Item already on player's position
      } else {
        agent.Notify("Cannot drop the item, there is already an item on this cell.", "item_alert");
        return;
      }
    }

    // Transfer ownership to grid
    agent.RemoveItem(item_drop.GetID());
    item_drop.SetGrid();
    item_drop.SetPosition(pos);
    main_grid.At(pos) = GetCellTypeSymbol(item_drop.GetID());

    agent.Notify("Dropping " + item_drop.GetName() + " onto the ground!", "item_alert");
  }

  /**
   * Checks to see if there is a flag or item at the agent's current position
   * @param agent The agent
   * @param pos The position
   */
  void CheckPosition(cse491::AgentBase& agent, cse491::GridPosition& pos) {
    // First check to see if agent is on win flag
    if ((main_grid.At(pos) == flag_id) && (agent.IsInterface())) {
      // Set win flag to true

      agent.Notify("Flag found ", "item_alert");

      agent.Notify("Leaving " + world_filename, "world_switched");

      if (world_filename == FIRST_FLOOR_FILENAME ||
          world_filename == SECOND_FLOOR_FILENAME) {
        // We are currently on floor 1 or 2,
        // so switch to the next world
        // and reset the item_map and player position.
        SwitchGrid(agent, pos);
      } else if (world_filename == FINAL_FLOOR_FILENAME) {
        agent.Notify("Congrats, you won the game!", "congrats_msg");
        run_over = true;
      }

      // then checks if agent is on any items
    } else if ((main_grid.At(pos) == hidden_warp_id) && (agent.IsInterface())) {
      // Agent used the hidden warp tile action
      std::cout << "Hidden warp tile activated! Warping to floor 3."
                << std::endl;

      agent.Notify("Leaving " + world_filename, "world_switched");

      // Update the world to floor 3
      world_filename = FINAL_FLOOR_FILENAME;
      agents_filename = "../assets/third_floor_input.json";

      // Clear item_map for the next floor
      item_map.clear();

      // Reset the current new_position to the top left of the new grid.
      pos = cse491::GridPosition(0, 0);
      main_grid.Read(FINAL_FLOOR_FILENAME, type_options);
      LoadFromFile(agents_filename);

      // Update the agent's position on the new floor
      agent.SetPosition(pos);

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
              agent.Notify("You found the " + GetItem(x).GetName() + " in the " + item_found.GetName());
              agent.AddItem(GetItem(x));
              item_found.RemoveItem(GetItem(x));
            }

            // Check agent's inventory size
            if (agent.GetInventory().size() == MAX_INVENTORY_SIZE) {
              agent.Notify(
                  "It looks like your inventory is full, please drop items or place them in chests!", "item_alert");
            }
          } else {
            agent.Notify("The chest is empty! You can store items with 't'!", "item_alert");
          }

        // Item is not a chest
        } else {
          if (agent.GetInventory().size() == MAX_INVENTORY_SIZE) {
            agent.Notify("It looks like your inventory is full, please drop items or place them in chests!",
                "item_alert");

          } else {
            agent.Notify("You found " + item_found.GetName() + "!", "item_alert");

            // Add item to the agent's inventory
            agent.AddItem(item_found.GetID());

            // Set the position off the grid, so it doesn't render
            item_found.SetPosition(OffGrid);
            main_grid.At(pos) = floor_id;
          }
        }
      }
    }
  }

  /**
   * Allows agents to perform an action and sets each agent's new position
   * @param agent The agent performing the action
   * @param action_id The id that represents the move
   * @return An int that declares if the move is legal or illegal (true / false)
   */
  int DoAction(cse491::AgentBase& agent, size_t action_id) override {
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
        IsDropped = true;
        break;
    }

    if (!main_grid.IsValid(new_position)) {
      return false;
    }
    if (main_grid.At(new_position) == wall_id) {
      return false;
    }

    if (!IsDropped) {
        CheckPosition(agent, new_position);
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
};
}  // namespace group4
