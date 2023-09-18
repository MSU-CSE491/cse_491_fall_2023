/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A base class for all items or agents that can exist on the grid.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <cassert>
#include <string>
#include <unordered_map>

#include "GridPosition.hpp"

namespace cse491 {

  class Entity {
  protected:
    const size_t id;        ///< Unique ID for this entity.
    std::string name;       ///< Name for this entity (E.g., "Player 1" or "+2 Sword")
    GridPosition position;  ///< Where on the grid is this entity?

    /// Every entity can have a simple set of properties (with values) associated with it.
    std::unordered_map<std::string, double> property_map;

  public:
    Entity(size_t id, const std::string & name) : id(id), name(name) { }
    Entity(const Entity &) = default;
    Entity(Entity &&) = default;
    virtual ~Entity() = default;

    Entity & operator=(const Entity &) = default;
    Entity & operator=(Entity &&) = default;

    // -- Accessors --
    [[nodiscard]] size_t GetID() const { return id; }
    [[nodiscard]] const std::string & GetName() const { return name; }
    [[nodiscard]] GridPosition GetPosition() const { return position; }

    Entity & SetName(const std::string in_name) { name = in_name; return *this; }
    Entity & SetPosition(GridPosition in_pos) { position = in_pos; return *this; }
    Entity & SetPosition(double x, double y) { position = GridPosition{x,y}; return *this; }

    /// Is this Entity actually an autonomous agent? (Overridden in AgentBase to return true)
    virtual bool IsAgent() const { return false; }

    /// Is this Entity actually a specialty Agent that's an Interface for a human player?
    /// (Overridden in InterfaceBase to return true)
    virtual bool IsInterface() const { return false; }


    // -- Property Management --

    /// Does this agent have a property with the specified name?
    [[nodiscard]] bool HasProperty(const std::string & name) const {
      return property_map.count(name);
    }

    /// Return the current value of the specified property.
    [[nodiscard]] double GetProperty(const std::string & name) const {
      assert(HasProperty(name));   // Break if property does not already exist.
      return property_map.at(name);
    }

    /// Change the value of the specified property (will create if needed)
    Entity & SetProperty(const std::string & name, double value=0.0) {
      property_map[name] = value;
      return *this;
    }

    /// Allow for setting multiple properties at once.
    Entity & SetProperties() { return *this; }

    template <typename... EXTRA_Ts>
    Entity & SetProperties(const std::string & name, double value, EXTRA_Ts... extras) {
      SetProperty(name, value);        // Set the first property...
      return SetProperties(std::forward<EXTRA_Ts>(extras)...); // And any additional properties...
    }
  };

} // End of namespace cse491
