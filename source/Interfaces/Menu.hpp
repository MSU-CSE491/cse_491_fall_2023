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
        std::vector<std::shared_ptr<Button>> menuBar;

    public:
        void initialize();

        void drawto(sf::RenderWindow &window);

        void HandleMouseMove(sf::RenderWindow &window);

        void HandleMouseButtonPressed(sf::RenderWindow &window);
    };
}
#endif //CSE_491_MENU_HPP
