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
    [[nodiscard]] size_t GetOwnerID() const { return owner_id; }

    /// @brief Identify the entity (item or agent) that now owns this item.
    /// @param owner The new owning entity.
    /// @return A reference to this item.
    ItemBase & SetOwner(const Entity & owner) {
      if (owner.IsItem()) owner_type = OwnerType::ITEM;
      else if (owner.IsAgent()) owner_type = OwnerType::AGENT;
      else owner_type = OwnerType::NONE; // Error?
      owner_id = owner.GetID();
      return *this;
    }

    ItemBase & SetGrid(size_t grid_id=0) {
      owner_type = OwnerType::GRID;
      owner_id = grid_id;
      return *this;
    }
    ItemBase & SetUnowned() { owner_type = OwnerType::NONE; return *this; }
  };

} // End of namespace cse491
