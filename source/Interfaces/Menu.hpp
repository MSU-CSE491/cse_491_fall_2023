/**
 * @file Menu.h
 * @author
 *
 *
 */

#ifndef CSE_491_MENU_HPP
#define CSE_491_MENU_HPP
#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include <vector>
#include <memory>
#include "Inventory.hpp"

namespace i_2D {
const int MAX_NUMBER_OF_ITEMS_MENU_ITEMS = 3;

    class Menu {
    private:
        /// Top menubar on the window
        std::vector<std::unique_ptr<Button>> mMenuBar;
        /// Interface Entitiy's inventory
        std::unique_ptr<Inventory> mInventory;
        sf::Font * mFont;
        sf::Vector2f mWorldSize;

    public:
        void initialize(sf::Font &font,sf::Vector2f size);

        void drawto(sf::RenderWindow &window);

        void HandleMouseMove(sf::RenderWindow &window);

        void HandleMouseButtonPressed(sf::RenderWindow &window);
        std::vector<std::unique_ptr<Button>> &GetMenu()
        {
            return mMenuBar;
        }
        void SetWorldSize(sf::Vector2f size){
            mWorldSize = size;
        }

        bool IsInventoryOpen(){
            if(mInventory){
                return true;
            }else return false;
        }

        void ConstructInventory(){
            mInventory = std::make_unique<Inventory>(mWorldSize);
            mInventory->ConstructInventory(*mFont);
        }

        void DeconstructInventory(){
            mInventory.reset();
        }

    };
}
#endif //CSE_491_MENU_HPP
