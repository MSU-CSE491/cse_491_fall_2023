/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Set of types used throughout the codebase
 * @note Status: ALPHA
 **/

#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace cse491 {

/// @brief Simple data structure to hold info about a TYPE of cell in the world.
struct CellType {
  std::string name;  ///< Unique name for this type of cell (e.g., "wall", "tree", "moon")
  std::string desc;  ///< Full description of what this type of cell is
  char symbol;       ///< Symbol for text representations (files or interface)
  std::set<std::string> properties{};  ///< Set of properties for this cell type.

  /// Adds the specifed property to this CellType.
  CellType& SetProperty(const std::string& property) {
    properties.insert(property);
    return *this;
  }

  /// Removes the specifed property from this CellType.
  CellType& RemoveProperty(const std::string& property) {
    properties.erase(property);
    return *this;
  }

  /// Checks if the given property is set on this CellType.
  bool HasProperty(const std::string& property) const { return properties.count(property); }

  constexpr static char CELL_WALL[] = "wall";
  constexpr static char CELL_WATER[] = "water";
};

/// @brief Available CellTypes will be passed around as a vector of options.
using type_options_t = std::vector<CellType>;

class ItemBase;
/// @brief Maps of item IDs to item pointers
using item_map_t = std::map<size_t, std::unique_ptr<ItemBase>>;

class AgentBase;
/// @brief Maps of agent IDs to agent pointers
using agent_map_t = std::map<size_t, std::unique_ptr<AgentBase>>;

/// @brief Common types of properties in network serialization
enum class PropertyType { t_double, t_int, t_char, t_string, t_position, t_bool, t_other };

/// @brief Enum for World types in network serialization
enum class WorldType { w_maze, w_second, w_generative, w_manual };

}  // namespace cse491
