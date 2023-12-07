/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A base class interface for all non-agent item types.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <cassert>
#include <string>
#include <unordered_map>

#include "Data.hpp"
#include "Entity.hpp"

namespace cse491 {

  class ItemBase : public Entity {
  protected:
    enum OwnerType { NONE=0, GRID, ITEM, AGENT };
    OwnerType owner_type = OwnerType::NONE;
    size_t owner_id = 0;

  public:
    ItemBase(size_t id, const std::string & name) : Entity(id, name) { }
    ~ItemBase() = default; // Already virtual from Entity

    // -- Entity Overrides --

    bool IsItem() const override { return true; }

    // -- Ownership Info --
    [[nodiscard]] bool IsOnGrid() const { return owner_type == OwnerType::GRID; }
    [[nodiscard]] bool IsOwnedByItem() const { return owner_type == OwnerType::ITEM; }
    [[nodiscard]] bool IsOwnedByAgent() const { return owner_type == OwnerType::AGENT; }
    [[nodiscard]] bool IsOwned() const { return IsOwnedByItem() || IsOwnedByAgent(); }
    [[nodiscard]] bool IsOnGrid(size_t grid_id) const {
      return IsOnGrid() && owner_id == grid_id;
    }
    [[nodiscard]] bool IsOwnedByItem(size_t item_id) const {
      return IsOwnedByItem() && owner_id == item_id;
    }
    [[nodiscard]] bool IsOwnedByAgent(size_t agent_id) const {
      return IsOwnedByAgent() && owner_id == agent_id;
    }
    [[nodiscard]] bool IsOwnedBy(size_t entity_id) const {
      return IsOwned() && owner_id == entity_id;
    }
    [[nodiscard]] size_t GetOwnerID() const { return owner_id; }

    /// @brief Identify the entity (item or agent) that now owns this item.
    /// @param owner The new owning entity.
    /// @return A reference to this item.
    ItemBase & SetOwner(const Entity & owner) {
      if (owner.IsItem()) owner_type = OwnerType::ITEM;
      else if (owner.IsAgent()) owner_type = OwnerType::AGENT;
      else owner_type = OwnerType::NONE; // Error?
      owner_id = owner.GetID();
      position.MakeInvalid();
      return *this;
    }

    ItemBase & SetGrid(size_t grid_id=0) {
      owner_type = OwnerType::GRID;
      owner_id = grid_id;
      return *this;
    }
    ItemBase & SetUnowned() { owner_type = OwnerType::NONE; return *this; }

    /// @brief Serialize item
    /// @param ostream
    void Serialize(std::ostream &os) override {
      os << name << '\n';
      os << position.GetX() << '\n';
      os << position.GetY() << '\n';
      os << property_map.size() << '\n';
      for (const auto & property : property_map) {
        os << property.first << '\n';

        // Get property type
        PropertyType type = GetPropertyType(property.first);
        os << static_cast<int>(type) << '\n';

        // serialize property value
        if (type == PropertyType::t_double) {
          os << AsProperty<double>(property.first).value << '\n';
        } else if (type == PropertyType::t_int) {
          os << AsProperty<int>(property.first).value << '\n';
        } else if (type == PropertyType::t_char) {
          os << AsProperty<char>(property.first).value << '\n';
        } else if (type == PropertyType::t_string) {
          os << AsProperty<std::string>(property.first).value << '\n';
        } else {
          // unknown type, do nothing
          os << '\n';
        }
      }
    }

    /// @brief Deserialize item
    /// @param istream
    void Deserialize(std::istream &is) {
      std::string x_str, y_str;
      std::getline(is, name, '\n');
      std::getline(is, x_str, '\n');
      std::getline(is, y_str, '\n');
      position.Set(stoi(x_str), stoi(y_str));

      std::string property, type_str, value_str;
      std::getline(is, property, '\n');
      int num_properties = stoi(property);
      for (int i = 0; i < num_properties; i++) {
        std::getline(is, property, '\n');
        std::getline(is, type_str, '\n');
        std::getline(is, value_str, '\n');

        // Set property based on type
        auto type = static_cast<PropertyType>(stoi(type_str));
        if (type == PropertyType::t_double) {
          SetProperty(property, stod(value_str));
        } else if (type == PropertyType::t_int) {
          SetProperty(property, stoi(value_str));
        } else if (type == PropertyType::t_char) {
          SetProperty(property, value_str[0]);
        } else if (type == PropertyType::t_string) {
          SetProperty(property, value_str);
        }
      }
    }
  };

} // End of namespace cse491
