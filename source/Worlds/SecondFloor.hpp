/**
 * @file SecondFloor.hpp
 * @author Jayson Van Dam
 */

#pragma once
#include "../core/WorldBase.hpp"
#include "../Agents/PacingAgent.hpp"
#include <nlohmann/json.hpp>

namespace group4 {
/**
 * Creates a world with agents and a win flag
 * TODO: Integrate this class into SecondWorld by
 * adding a constructor to SecondWorld
 * that takes a path to a grid file.
 */
class SecondFloor : public cse491::WorldBase {
 protected:
  enum ActionType {
    REMAIN_STILL = 0,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT
  };

  /// Easy access to floor CellType ID.
  size_t floor_id;

  /// Easy access to flag CellType ID.
  size_t flag_id;

  /// Easy access to wall CellType ID.
  size_t wall_id;

  /// Provide the agent with movement actions.
  void ConfigAgent(cse491::AgentBase& agent) override {
    agent.AddAction("up", MOVE_UP);
    agent.AddAction("down", MOVE_DOWN);
    agent.AddAction("left", MOVE_LEFT);
    agent.AddAction("right", MOVE_RIGHT);
  }

 public:
  /**
   * Constructor with no arguments
   */
  SecondFloor() {
    floor_id =
        AddCellType("floor", "Floor that you can easily walk over.", ' ');
    flag_id = cse491::WorldBase::AddCellType(
        "flag", "Goal flag for a game end state", 'g');
    wall_id = AddCellType(
        "wall", "Impenetrable wall that you must find a way around.", '#');
    main_grid.Read("../assets/grids/second_floor.grid", type_options);

    const std::string input_filename = "../assets/second_floor_input.json";
    LoadFromFile(input_filename);
  }

  /**
   * Destructor
   */
  ~SecondFloor() = default;

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

    for (const auto& agent : data) {
      // May get a json.exception.type_error here if you assign to the wrong C++ type,
      // so make sure to nail down what types things are in JSON first!
      std::string agent_name = agent.at("name");
      int x_pos = agent.at("x");
      int y_pos = agent.at("y");
      AddAgent<cse491::PacingAgent>(agent_name).SetPosition(x_pos, y_pos);
    }
  }

  void UpdateWorld() override {}

  virtual void Run() override {
    while (!run_over) {
      RunAgents();
      UpdateWorld();
    }

    std::string filename = "output.json";

    std::ofstream ofs(filename);

    ofs << "[" << std::endl;
    size_t index = 0;

    for (const auto& other_agent : agent_set) {
      auto new_position = other_agent->GetPosition();

      std::string agent_name = other_agent->GetName();

      double x_pos = new_position.GetX();
      double y_pos = new_position.GetY();

      ofs << "  {" << std::endl;
      ofs << "    \"name\": "
          << "\"" << agent_name << "\""
          << ",\n";

      ofs << "    \"x\": " << x_pos << "," << std::endl;
      ofs << "    \"y\": " << y_pos << std::endl;

      if (index == agent_set.size() - 1) {
        ofs << "  }" << std::endl;
      } else {
        ofs << "  }," << std::endl;
      }

      index++;
    }
    ofs << "]" << std::endl;
  }

  /**
   * Allows agents to move around the maze.
   * Also checks if the next agent position will be the win flag.
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
    }
    if (!main_grid.IsValid(new_position)) {
      return false;
    }
    if (main_grid.At(new_position) == wall_id) {
      return false;
    }

    if (main_grid.At(new_position) == flag_id) {
      // Set win flag to true
      std::cout << "flag found" << std::endl;
      run_over = true;
    }

    agent.SetPosition(new_position);

    return true;
  }
};
}  // namespace group4