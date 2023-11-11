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

namespace i_2D {
const int MAX_NUMBER_OF_ITEMS_MENU_ITEMS = 3;

    class Menu {
    private:
        /// Top menubar on the window
        std::vector<std::unique_ptr<Button>> mMenuBar;

    public:
        void initialize(const sf::Font &font);

        void drawto(sf::RenderWindow &window);

        void HandleMouseMove(sf::RenderWindow &window);

        void HandleMouseButtonPressed(sf::RenderWindow &window);
        std::vector<std::unique_ptr<Button>> &GetMenu()
        {
            return mMenuBar;
        }

    };
}
#endif //CSE_491_MENU_HPP
