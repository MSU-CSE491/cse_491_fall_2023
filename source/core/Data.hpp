/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Set of types used throughout the codebase
 * @note Status: PROPOSAL
 * 
 * @CAO: It may be that Interfaces aren't much more than a special type of Agent.
 *       we should decide if we even need a special category, but even just listing
 *       the base class is informative.
 **/

#pragma once

#include <memory>
#include <string>
#include <vector>

namespace cse491 {
  
  /// @brief Simple structure to hold information about a type of cell in the world.
  struct CellType {
    std::string name;
    std::string desc;
    size_t id;           ///< Unique identifier; should match position in type_options
    char symbol;         ///< Symbol for text representations (files or interface)
  };

  using type_options_t = std::vector<CellType>;

  class Entity;
  using item_set_t = std::vector<std::unique_ptr<Entity>>;

  class AgentBase;
  using agent_set_t = std::vector<std::unique_ptr<AgentBase>>;

}