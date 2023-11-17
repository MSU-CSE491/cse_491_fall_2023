/**
 * @author : Team - 3
 * @date: 11/15/2023
 * Inventory class creates an array of buttons and uses it as inventory
 */

#ifndef CSE_491_INVENTORY_HPP
#define CSE_491_INVENTORY_HPP

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include <vector>
#include <memory>

namespace i_2D {
    /***
     * @class Inventory
     *
     * @brief Represent the inventory of entities.
     *
     * An interface/visual representation of an inventory.
     */
    class Inventory {
    private:
        /// Projected inventory screen
        std::unique_ptr<sf::RectangleShape> mInventoryWindow;
        /// List of items in an inventory
        std::vector<std::vector<std::unique_ptr<Button>>> mInventoryList;
        sf::Vector2f mWorldSize; ///< Size of the world
        int mCol=0; ///< Number of columns enumerating the inventory list
        int mRow=0; ///< Number of rows enumerating the inventory list

    public:
        explicit Inventory(const sf::Vector2f size) : mWorldSize(size) {}
        void SetSize(const sf::Vector2f size) {mWorldSize = size;}
        void ConstructInventory(sf::Font &font);
        void DrawTo(sf::RenderWindow &window);
        void HandleMouseMove(sf::RenderWindow &window);
    };
}


#endif //CSE_491_INVENTORY_HPP
