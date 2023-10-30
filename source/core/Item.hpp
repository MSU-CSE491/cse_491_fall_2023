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

    class Inventory;

    class Item {
    protected:
        std::string name;
        int level = 0;
        int damage = 0;
        int durability = 0;
        int value = 0;
        double weight = 0.0;

        Inventory* inventory = nullptr;

    public:
        // Constructor
        Item(const std::string &name, int level, int damage, int durability, int value, double weight)
                : name(name), level(level), damage(damage), durability(durability), value(value), weight(weight) {}

        // Destructor
        ~Item() = default;

        // Setters and Getters
        const std::string &GetName() { return name; }

        void SetName(std::string newName) { name = newName; }
        void SetLevel(int new_level) { level = new_level; }
        void SetDamage(int new_damage) { damage = new_damage; }
        void SetDurability(int new_durability) { durability = new_durability; }
        void SetValue(int new_value) { value = new_value; }
        void SetWeight(double new_weight) { weight = new_weight; }
        void SetInventory(walle::Inventory* inv) { inventory = inv; }

        int GetLevel() const { return level; }
        int GetDamage() const { return damage; }
        int GetDurability() const { return durability; }
        int GetValue() const { return value; }
        double GetWeight() const { return weight; }
        Inventory* GetInventory() const { return inventory; }

        // Item functions
        virtual void UseItem() { durability--; } // virtual for Armor and Food item classes

    };
}