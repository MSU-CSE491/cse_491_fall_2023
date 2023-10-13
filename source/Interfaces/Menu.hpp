/**
 * @file Menu.h
 * @author
 *
 *
 */

#ifndef CSE_491_MENU_HPP
#define CSE_491_MENU_HPP
#include <SFML/Graphics.hpp>

namespace i_2D {
const int MAX_NUMBER_OF_ITEMS_MENU_ITEMS = 3;

    class Menu {
    private:
        int selectedItemIndex;
        sf::Font font;
        sf::Text menu[MAX_NUMBER_OF_ITEMS_MENU_ITEMS];

    public:
        Menu(float width=600.f, float height=600.f);

        ~Menu();

        void draw(sf::RenderWindow &window);

        void MoveUp();

        void MoveDown();

        void Start();

        int GetPressedItem(){ return selectedItemIndex;}
    };
}
#endif //CSE_491_MENU_HPP
