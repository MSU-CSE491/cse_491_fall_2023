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

  class WorldBase;

  class Entity {
  private:
    WorldBase * world_ptr; ///< Track the world this entity is in (private to protect pointer)

  protected:
    const size_t id;        ///< Unique ID for this entity.
    std::string name;       ///< Name for this entity (E.g., "Player 1" or "+2 Sword")
    GridPosition position;  ///< Where on the grid is this entity?

    struct PropertyBase {
      virtual ~PropertyBase() { }
    };

    template <typename T>
    struct Property : public PropertyBase {
      T value;
      Property(const T & in) : value(in) { }
      Property(T && in) : value(in) { }
    };

    /// Every entity can have a simple set of properties (with values) associated with it.
    std::unordered_map<std::string, std::unique_ptr<PropertyBase>> property_map;

    // -- Helper Functions --

    template <typename T>
    Property<T> & AsProperty(const std::string & name) const {
      assert( HasProperty(name) );
      PropertyBase * raw_ptr = property_map.at(name).get();
      assert( dynamic_cast<Property<T> *>(raw_ptr) );
      auto property_ptr = static_cast<Property<T> *>(raw_ptr);
      return *property_ptr;
    }

  public:
    Entity(size_t id, const std::string & name) : id(id), name(name) { }
    Entity(const Entity &) = delete; // Entities must be unique and shouldn't be copied.
    Entity(Entity &&) = default;
    virtual ~Entity() = default;

    Entity & operator=(const Entity &) = delete; // Entities must be unique and shouldn't be copied.
    Entity & operator=(Entity &&) = delete;      // Entities should never have IDs change.

    // -- Accessors --
    [[nodiscard]] size_t GetID() const { return id; }
    [[nodiscard]] const std::string & GetName() const { return name; }
    [[nodiscard]] GridPosition GetPosition() const { return position; }
    [[nodiscard]] WorldBase & GetWorld() const { assert(world_ptr); return *world_ptr; }

    Entity & SetName(const std::string in_name) { name = in_name; return *this; }
    Entity & SetPosition(GridPosition in_pos) { position = in_pos; return *this; }
    Entity & SetPosition(double x, double y) { position = GridPosition{x,y}; return *this; }
    Entity & SetWorld(WorldBase & in_world) { world_ptr = &in_world; return *this; }

    virtual bool IsAgent() const { return false; }     ///< Is Entity an autonomous agent?
    virtual bool IsItem() const { return false; }      ///< Is Entity an item?
    virtual bool IsInterface() const { return false; } ///< Is Entity an interface for a human?


    // -- Property Management --

    /// Does this agent have a property with the specified name?
    [[nodiscard]] bool HasProperty(const std::string & name) const {
      return property_map.count(name);
    }

    /// Return the current value of the specified property.
    template <typename T=double>
    [[nodiscard]] const T & GetProperty(const std::string & name) const {
      assert(HasProperty(name));   // Break if property does not already exist.
      return AsProperty<T>(name).value;
    }

    /// Change the value of the specified property (will create if needed)
    template <typename T>
    Entity & SetProperty(const std::string & name, const T & value) {
      if (HasProperty(name)) {
        AsProperty<T>(name).value = value;
      } else {
        property_map[name] = std::make_unique<Property<T>>(value);
      }
      return *this;
    }

    /// Allow for setting multiple properties at once.
    Entity & SetProperties() { return *this; }

    template <typename VALUE_T, typename... EXTRA_Ts>
    Entity & SetProperties(const std::string & name, VALUE_T && value, EXTRA_Ts &&... extras) {
      SetProperty(name, std::forward<VALUE_T>(value));        // Set the first property...
      return SetProperties(std::forward<EXTRA_Ts>(extras)...); // And any additional properties...
    }

    /// Completely remove a property from an Entity.
    Entity & RemoveProperty(const std::string & name) {
      property_map.erase(name);
      return *this;
    }    
  };

} // End of namespace cse491
