/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Set of types used throughout the codebase
 * @note Status: PROPOSAL
 **/

#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <set>

namespace cse491 {
  
  /// @brief Simple data structure to hold info about a TYPE of cell in the world.
  struct CellType {
    std::string name;  ///< Unique name for this type of cell (e.g., "wall", "tree", "moon")
    std::string desc;  ///< Full description of what this type of cell is
    char symbol;       ///< Symbol for text representations (files or interface)
    std::set<std::string> properties{}; ///< Set of properties for this cell type.
    
    /// Adds the specifed property to this CellType.
    CellType& SetProperty(const std::string& property){
      properties.insert(property);
      return *this;
    }
    
    /// Removes the specifed property from this CellType.
    CellType& RemoveProperty(const std::string& property){
      properties.erase(property);
      return *this;
    }
    
    /// Checks if the given property is set on this CellType.
    bool HasProperty(const std::string& property) const {
      return properties.count(property);
    }
    
    constexpr static std::string CELL_WALL = "wall";
  };

  /// @brief Available CellTypes will be passed around as a vector of options.
  using type_options_t = std::vector<CellType>;

  class ItemBase;
  /// @brief Maps of item IDs to item pointers
  using item_map_t = std::map<size_t, std::unique_ptr<ItemBase>>;

  class AgentBase;
  /// @brief Maps of agent IDs to agent pointers
  using agent_map_t = std::map<size_t, std::unique_ptr<AgentBase>>;

}
