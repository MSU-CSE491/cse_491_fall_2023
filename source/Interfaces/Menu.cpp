/**
 * @file Menu.cpp
 * @author
 */

#include "Menu.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

const sf::Vector2f MENU_BUTTON_SIZE({200.f,50.f});
namespace i_2D {

    /**
     * @brief initialize the buttons at the top of the window
     */
    void Menu::initialize(const sf::Font &font) {
        sf::Color backgroundcolor = sf::Color::Black;
        sf::Color textcolor = sf::Color::White;

        menuBar.emplace_back(std::make_unique<Button>(
                "Menu", MENU_BUTTON_SIZE, backgroundcolor, textcolor, font));
        menuBar[0]->setPosition({0,0});
        menuBar.emplace_back(std::make_unique<Button>(
                "Inventory", MENU_BUTTON_SIZE, backgroundcolor, textcolor, font));
        menuBar[1]->setPosition({200,0});
        menuBar.emplace_back(std::make_unique<Button>(
                "Exit", MENU_BUTTON_SIZE, backgroundcolor, textcolor, font));
        menuBar[2]->setPosition({400,0});

    }

    /**
     * @brief draws each menu button to the window
     *
     * @param window the main window of the graphic interface
     */
    void Menu::drawto(sf::RenderWindow &window) {
        for( const auto &button : menuBar){
            button->drawTo(window);
        }
    }

    /**
     * @brief check for mouse hoovering over the menu buttons
     * change button color accordingly
     *
     * @param window the main window of the graphic interface
     */
    void Menu::HandleMouseMove(sf::RenderWindow &window) {
        if (menuBar[0]->isMouseOver(window)){
            menuBar[0]->setBackColor(sf::Color::Magenta);
            menuBar[1]->setBackColor(sf::Color::Black);
            menuBar[2]->setBackColor(sf::Color::Black);
        }else if (menuBar[1]->isMouseOver(window)){
            menuBar[0]->setBackColor(sf::Color::Black);
            menuBar[1]->setBackColor(sf::Color::Magenta);
            menuBar[2]->setBackColor(sf::Color::Black);
        }else if (menuBar[2]->isMouseOver(window)){
            menuBar[0]->setBackColor(sf::Color::Black);
            menuBar[1]->setBackColor(sf::Color::Black);
            menuBar[2]->setBackColor(sf::Color::Magenta);
        }else{
            menuBar[0]->setBackColor(sf::Color::Black);
            menuBar[1]->setBackColor(sf::Color::Black);
            menuBar[2]->setBackColor(sf::Color::Black);
        }
    }

    /**
     * @brief check if the mouse click the exit button
     * closes window accordingly
     *
     * @param window the main window of the graphic interface
     */
    void Menu::HandleMouseButtonPressed(sf::RenderWindow &window) {
        if(menuBar[2]->isMouseOver(window)){
            exit(0);
        }
    }
}


