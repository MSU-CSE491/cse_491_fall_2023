/**
* @author : Team - 3
* @date: 10/3/2023
* Menu class creates a menu and displays it in the render window
*/

#include "Menu.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

const sf::Vector2f MENU_BUTTON_SIZE({200.f,50.f});
namespace i_2D {

    /**
     * @brief initialize the buttons at the top of the window
     */
    void Menu::initialize(sf::Font &font, sf::Vector2f size) {
        sf::Color backgroundcolor = sf::Color::Black;
        sf::Color textcolor = sf::Color::White;
        mFont = &font;
        mWorldSize = size;

        mMenuBar.emplace_back(std::make_unique<Button>(
                "Menu", MENU_BUTTON_SIZE, backgroundcolor, textcolor, font));
        mMenuBar[0]->setPosition({0,0});
        mMenuBar.emplace_back(std::make_unique<Button>(
                "Inventory", MENU_BUTTON_SIZE, backgroundcolor, textcolor, font));
        mMenuBar[1]->setPosition({200,0});
        mMenuBar.emplace_back(std::make_unique<Button>(
                "Exit", MENU_BUTTON_SIZE, backgroundcolor, textcolor, font));
        mMenuBar[2]->setPosition({400,0});
        mMenuBar.emplace_back(std::make_unique<Button>(
                "Normal", MENU_BUTTON_SIZE, backgroundcolor, textcolor, font));
        mMenuBar[3]->setPosition({600,0});
        mMenuBar.emplace_back(std::make_unique<Button>(
                "Enlarge", MENU_BUTTON_SIZE, backgroundcolor, textcolor, font));
        mMenuBar[4]->setPosition({800,0});

    }

    /**
     * @brief draws each menu button to the window
     *
     * @param window the main window of the graphic interface
     */
    void Menu::drawto(sf::RenderWindow &window) {
        for( const auto &button : mMenuBar){
            button->drawTo(window);
        }
        if(mInventory) {
            mInventory->DrawTo(window);
        }
    }

    /**
     * @brief check for mouse hoovering over the menu buttons
     * change button color accordingly
     *
     * @param window the main window of the graphic interface
     */
    void Menu::HandleMouseMove(sf::RenderWindow &window) {
        for (int i = 0; i < mMenuBar.size(); ++i) {
            if (mMenuBar[i]->isMouseOver(window)) {
                mMenuBar[i]->setBackColor(sf::Color::Magenta);
            } else {
                mMenuBar[i]->setBackColor(sf::Color::Black);
            }
        }
        if(mInventory){
            mInventory->HandleMouseMove(window);
        }
    }
    /**
     * @brief check if the mouse click the exit button
     * closes window accordingly
     *
     * @param window the main window of the graphic interface
     */
    void Menu::HandleMouseButtonPressed(sf::RenderWindow &window) {
        if(mMenuBar[2]->isMouseOver(window)){
            exit(0);
        }else if(mMenuBar[1]->isMouseOver(window)){
            if(mInventory){
                DeconstructInventory();
            }else {
                ConstructInventory();
            }
        }

    }

}


