/**
 * @author : Team - 3
 * @date: 11/15/2023
 * Inventory class creates an array of buttons and displays the inventory
 */

#ifndef CSE_491_INVENTORY_HPP
#define CSE_491_INVENTORY_HPP

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include <vector>
#include <memory>

namespace i_2D {
    class Inventory {
    private:
        std::unique_ptr<sf::RectangleShape> mInventoryWindow;
        std::vector<std::vector<std::unique_ptr<Button>>> mInventoryList;
        sf::Vector2f mWorldSize;
        int mCol=0;
        int mRow=0;

    public:
        explicit Inventory(const sf::Vector2f size) : mWorldSize(size) {}

        void SetSize(const sf::Vector2f size) {mWorldSize = size;}
        void ConstructInventory(sf::Font &font);
        void DrawTo(sf::RenderWindow &window);
        void HandleMouseMove(sf::RenderWindow &window);
    };
}


#endif //CSE_491_INVENTORY_HPP
