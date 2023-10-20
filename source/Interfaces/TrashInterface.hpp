/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A low-end text interface providing a bare-minimum level of interaction.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <iostream>
#include <vector>

#include "../core/Data.hpp"
#include "../core/InterfaceBase.hpp"

namespace cse491 {

  class TrashInterface : public InterfaceBase {
  protected:
    bool wait_for_input = true;  ///< Should we pause and wait for the user to hit a key?

    // -- Helper Functions --
    void DrawGrid(const WorldGrid & grid, const type_options_t & type_options,
                  const item_set_t & item_set, const agent_set_t & agent_set)
    {
      std::vector<std::string> symbol_grid(grid.GetHeight());

      // Load the world into the symbol_grid;
      for (size_t y=0; y < grid.GetHeight(); ++y) {
        symbol_grid[y].resize(grid.GetWidth());
        for (size_t x=0; x < grid.GetWidth(); ++x) {
          symbol_grid[y][x] = type_options[ grid.At(x,y) ].symbol;
        }
      }

      // Add in the agents / entities
      for (const auto & entity_ptr : item_set) {
        GridPosition pos = entity_ptr->GetPosition();
        char c = '+';
        if (entity_ptr->HasProperty("symbol")) {
            c = entity_ptr->GetProperty<char>("symbol");
        }
        symbol_grid[pos.CellY()][pos.CellX()] = c;
      }

      for (const auto & agent_ptr : agent_set) {
        GridPosition pos = agent_ptr->GetPosition();
        char c = '*';
        if(agent_ptr->HasProperty("symbol")){
          c = agent_ptr->GetProperty<char>("symbol");
        }
        symbol_grid[pos.CellY()][pos.CellX()] = c;
      }

      // Print out the symbol_grid with a box around it.
      std::cout << '+' << std::string(grid.GetWidth(),'-') << "+\n";
      for (const auto & row : symbol_grid) {
        std::cout << "|";
        for (char cell : row) {
          // std::cout << ' ' << cell;
          std::cout << cell;
        }
        std::cout << "|\n";
      }
      std::cout << '+' << std::string(grid.GetWidth(),'-') << "+\n";
      std::cout << "\nYour move? ";
      std::cout.flush();
    }

  public:
    TrashInterface(size_t id, const std::string & name) : InterfaceBase(id, name) { }
    ~TrashInterface() = default; // Already virtual from Entity

    // -- AgentBase overrides --

    bool Initialize() override {
      return true;
    }

    size_t SelectAction(const WorldGrid & grid,
                        const type_options_t & type_options,
                        const item_set_t & item_set,
                        const agent_set_t & agent_set) override
    {
      // Update the current state of the world.
      DrawGrid(grid, type_options, item_set, agent_set);

      // See if there are any keys waiting in standard input (wait if needed)
      char input;
      do {
        std::cin >> input;
      } while (!std::cin && wait_for_input);

      // Respond to the user input...
      size_t action_id = 0;
      switch (input) {
        case 'w': case 'W': action_id = GetActionID("up");    break;
        case 'a': case 'A': action_id = GetActionID("left");  break;
        case 's': case 'S': action_id = GetActionID("down");  break;
        case 'd': case 'D': action_id = GetActionID("right"); break;
        case 'q': case 'Q': exit(0); // Quit!
      }

      // If we waited for input, but don't understand it, notify the user.
      if (wait_for_input && action_id == 0) {
        std::cout << "Unknown key '" << input << "'." << std::endl;
      }

      // Do the action!
      return action_id;
    }
  };

} // End of namespace cse491
