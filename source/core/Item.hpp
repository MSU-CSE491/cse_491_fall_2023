/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A base class for all item types.
 * @note Status: PROPOSAL
 * @author Monika Kanphade
 **/

#pragma once

#include <string>

namespace walle
{
    class Item {
    protected:
        std::string name;
        int level = 0;
        int damage = 0;
        int durability = 0;

    public:
        // Constructor
        Item(const std::string &name, int level, int damage, int durability)
                : name(name), level(level), damage(damage), durability(durability) {}

        // Destructor
        ~Item() = default;

        // Setters and Getters
        const std::string &GetName() { return name; }
        void SetLevel(int new_level) { level = new_level; }
        void SetDamage(int new_damage) { damage = new_damage; }
        void SetDurability(int new_durability) { durability = new_durability; }

        int GetLevel() const { return level; }
        int GetDamage() const { return damage; }
        int GetDurability() const { return durability; }

        // Item functions
        virtual void UseItem() { durability--; } // virtual for Armor and Food item classes

    };
}