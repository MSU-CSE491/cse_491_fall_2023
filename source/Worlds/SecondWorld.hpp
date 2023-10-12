/**
 * @file SecondWorld.hpp
 * @author Jayson Van Dam
 * @author Kurt LaBlanc
 * @author Satvik Ravipati
 */

#pragma once
#include "../core/WorldBase.hpp"

namespace group4 {
    /**
     * Creates a world with agents and a win flag
     */
    class SecondWorld : public cse491::WorldBase {
    protected:
        enum ActionType { REMAIN_STILL=0, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };

        /// Easy access to floor CellType ID.
        size_t floor_id;

        /// Easy access to flag CellType ID.
        size_t flag_id;

        /// Easy access to wall CellType ID.
        size_t wall_id;

        /// Provide the agent with movement actions.
        void ConfigAgent(cse491::AgentBase & agent) override {
            agent.AddAction("up", MOVE_UP);
            agent.AddAction("down", MOVE_DOWN);
            agent.AddAction("left", MOVE_LEFT);
            agent.AddAction("right", MOVE_RIGHT);
        }

    public:
        /**
         * Constructor with no arguments
         */
        SecondWorld() {
            floor_id = AddCellType("floor", "Floor that you can easily walk over.", ' ');
            flag_id = cse491::WorldBase::AddCellType("flag", "Goal flag for a game end state", 'g');
            wall_id = AddCellType("wall", "Impenetrable wall that you must find a way around.", '#');
            main_grid.Read("../assets/grids/group4_maze.grid", type_options);

    // Adding power sword with id = 1; name = sword of power
    auto powerSword = std::make_unique<cse491::Entity>(1, "Sword of Power");
    powerSword->SetPosition(3, 4);
    powerSword->SetProperty("Damage", 20.0);
    item_set.push_back(std::move(powerSword));

    // Adding fire sword with id = 2; name = Inferno Slicer
    auto infernoSlicer = std::make_unique<cse491::Entity>(2, "Inferno Slicer");
    infernoSlicer->SetPosition(5, 0);
    infernoSlicer->SetProperties("Damage", 12.5, "Speed", 15.0,
                                 "Burning Duration", 2.5);
    item_set.push_back(std::move(infernoSlicer));

    // Adding a piece of armor with id = 10; name = Daedric
    auto daedricArmor = std::make_unique<cse491::Entity>(10, "Daedric Armor");
    daedricArmor->SetPosition(7, 4);
    daedricArmor->SetProperties("Health", 99, "Extra Inv. Space", 5);
    item_set.push_back(std::move(daedricArmor));
  }

  /**
   * Destructor
   */
  ~SecondWorld() = default;

  void UpdateWorld() override {

  }

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
        int DoAction(cse491::AgentBase & agent, size_t action_id) override {
            cse491::GridPosition new_position;
            switch (action_id) {
            case REMAIN_STILL: new_position = agent.GetPosition(); break;
            case MOVE_UP:      new_position = agent.GetPosition().Above(); break;
            case MOVE_DOWN:    new_position = agent.GetPosition().Below(); break;
            case MOVE_LEFT:    new_position = agent.GetPosition().ToLeft(); break;
            case MOVE_RIGHT:   new_position = agent.GetPosition().ToRight(); break;
            }
            if (!main_grid.IsValid(new_position)) { return false; }
            if (main_grid.At(new_position) == wall_id) { return false; }

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