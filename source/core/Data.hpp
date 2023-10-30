/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Set of types used throughout the codebase
 * @note Status: PROPOSAL
 **/

#pragma once

#include <memory>
#include <string>
#include <vector>

namespace cse491 {
  
  /// @brief Simple data structure to hold info about a TYPE of cell in the world.
  struct CellType {
    std::string name;  ///< Unique name for this type of cell (e.g., "wall", "tree", "moon")
    std::string desc;  ///< Full description of what this type of cell is
    char symbol;       ///< Symbol for text representations (files or interface)
  };

  /// @brief Available CellTypes will be passed around as a vector of options.
  using type_options_t = std::vector<CellType>;

  class ItemBase;
  /// @brief Sets of items will be represented as vectors of pointers to the base class
  using item_set_t = std::vector<std::unique_ptr<ItemBase>>;

  class AgentBase;
  /// @brief Sets of agents will be represented as vectors of pointers to the base class
  using agent_set_t = std::vector<std::unique_ptr<AgentBase>>;

}