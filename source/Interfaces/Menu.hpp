/**
* @author : Team - 3
* @date: 10/3/2023
* Menu class creates a menu and displays it in the render window
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
        std::unique_ptr<Inventory> mInventory; ///< The interface to the inventory
        sf::Font * mFont; ///< The Font used for the menu
        sf::Vector2f mWorldSize; ///< The world size

    public:
        void initialize(sf::Font &font,sf::Vector2f size);
        void drawto(sf::RenderWindow &window);
        void HandleMouseMove(sf::RenderWindow &window);
        void HandleMouseButtonPressed(sf::RenderWindow &window);
        /**
         * @brief Getter for MenuBar
         * @return MenuBar
         */
        std::vector<std::unique_ptr<Button>> &GetMenu()
        {
            return mMenuBar;
        }
        /**
         * @brief Keep track of the world's size
         *
         * @param size Size of the world
         */
        void SetWorldSize(sf::Vector2f size){
            mWorldSize = size;
        }

        /**
         * @brief Check if the inventory interface is open
         *
         * @return True if inventory interface is on, otherwise false
         */
        bool IsInventoryOpen(){
            if(mInventory){
                return true;
            }else return false;
        }

        /**
         * @brief Builds an inventory
         */
        void ConstructInventory(){
            mInventory = std::make_unique<Inventory>(mWorldSize);
            mInventory->ConstructInventory(*mFont);
        }

        /**
         * @brief Destroy the inventory object
         */
        void DeconstructInventory(){
            mInventory.reset();
        }

    };
}
#endif //CSE_491_MENU_HPP
