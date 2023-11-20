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

  // Add identifier for the hidden warp tile
  size_t hidden_warp_id;

  /// Will pretend this is the agent's inventory
  std::vector<size_t> inventory;

  /// Map of the chests that hold items
  // TODO: No longer need b/c entities can have inventories which include agents
  // and items
  std::map<cse491::ItemBase, std::vector<cse491::ItemBase>> chest_map;

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

  // TODO: we should also add a dropping feature, agent can drop an item at any
  // point in the game,
  //  does not necessarily have to be at a chest location
  /**
   * Drops the item in the agent's inventory
   * @param agent This agent's item we're dropping
   * @param pos The position where the item will be dropped
   */
  void DropItem(cse491::AgentBase& agent, cse491::GridPosition& pos) {}

  /**
   * Checks to see if there is a flag or item at the agent's current position
   * @param agent The agent
   * @param pos The position
   */
  void CheckPosition(cse491::AgentBase& agent, cse491::GridPosition& pos) {
    // First check to see if agent is on win flag
    if ((main_grid.At(pos) == flag_id) && (agent.IsInterface())) {
      // Set win flag to true

      // TODO: Confirm 2nd param
      agent.Notify("Flag found ", "item_alert");

      agent.Notify("Leaving " + world_filename, "world_switched");

      if (world_filename == FIRST_FLOOR_FILENAME) {
        agent.Notify("Going to " + SECOND_FLOOR_FILENAME, "world_switched");

        world_filename = SECOND_FLOOR_FILENAME;
        agents_filename = "../assets/second_floor_input.json";

        // Need to clear item_map so that items don't stay for the next floor.
        item_map.clear();

        // Resetting the current new_position to the top left of the new grid.
        pos = cse491::GridPosition(0, 0);
        main_grid.Read(SECOND_FLOOR_FILENAME, type_options);
        LoadFromFile(agents_filename);

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
          // TODO: Check to see if the chest is empty, if !empty -> move them to
          // agent's inventory
          // TODO: If we are able to set the ownership of the items to the
          // chest,
          //  we should be able to check what item's the chest owns, rather than
          //  checking to see what items are on that GridPosition

          if (!item_found.GetInventory().empty()) {
            auto temp_inventory = item_found.GetInventory();
            std::cout << "This is inside the chest: " << std::endl;
            for (auto x : temp_inventory) {
              std::cout << "Name: " << GetItem(x).GetName() << " -- ID: " << x
                        << std::endl;
            }

            agent.Notify(
                "If you would like all the items, enter '0'; otherwise enter "
                "the number seperated by a comma for which item(s) you would "
                "like!",
                "item_alert");

            // TODO: Need to determine max size of inventory
            if (agent.GetInventory().size() == 10) {
              agent.Notify(
                  "It looks like your inventory is full, please drop items or "
                  "place them in chests!",
                  "item_alert");
            }

          } else {
            agent.Notify(
                "The chest is empty! Do you want to store any items in here?",
                "item_alert");
          }

        } else {
          if (agent.GetInventory().size() == 10) {
            agent.Notify(
                "It looks like your inventory is full, please drop items or "
                "place them in chests!",
                "item_alert");
          } else {
            agent.Notify("You found " + item_found.GetName() + "!",
                         "item_alert");

            // Transfer ownership to agent
            item_found.SetOwner(agent);

            // Removes item from item_map
            RemoveItem(item_found.GetID());

            // Change the grid position to floor_id, so it's not seen on the
            // grid
            main_grid.At(pos) = floor_id;
          }
        }
      }
    }
  }

  // TODO: I think we should break up the DoAction into smaller functions
  // instead of having one really large
  //  function. Would be easier to read and it's not good to mix multiple
  //  functions into one...
  /**
   * Allows agents to perform an action and sets each agent's new position
   * @param agent The agent performing the action
   * @param action_id The id that represents the move
   * @return An int that declares if the move is legal or illegal (true / false)
   */
  int DoAction(cse491::AgentBase& agent, size_t action_id) override {
    cse491::GridPosition new_position;
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
        DropItem(agent, new_position);
        break;
    }

    if (!main_grid.IsValid(new_position)) {
      return false;
    }
    if (main_grid.At(new_position) == wall_id) {
      return false;
    }

    CheckPosition(agent, new_position);

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
