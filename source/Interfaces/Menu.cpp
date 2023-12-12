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
    void Menu::Initialize(sf::Font &font, sf::Vector2f size) {
        sf::Color backgroundcolor = sf::Color::Black;
        sf::Color textcolor = sf::Color::White;
        mFont = &font;
        mWorldSize = size;

        mMenuBar.emplace_back(std::make_unique<Button>(
                "Menu", MENU_BUTTON_SIZE, backgroundcolor, textcolor, font));
        mMenuBar[0]->SetPosition({0,0});
        mMenuBar.emplace_back(std::make_unique<Button>(
                "Inventory", MENU_BUTTON_SIZE, backgroundcolor, textcolor, font));
        mMenuBar[1]->SetPosition({200,0});
        mMenuBar.emplace_back(std::make_unique<Button>(
                "Exit", MENU_BUTTON_SIZE, backgroundcolor, textcolor, font));
        mMenuBar[2]->SetPosition({400,0});
        mMenuBar.emplace_back(std::make_unique<Button>(
                "Normal", MENU_BUTTON_SIZE, backgroundcolor, textcolor, font));
        mMenuBar[3]->SetPosition({600,0});
        mMenuBar.emplace_back(std::make_unique<Button>(
                "Enlarge", MENU_BUTTON_SIZE, backgroundcolor, textcolor, font));
        mMenuBar[4]->SetPosition({800,0});

    }

    /**
     * @brief draws each menu button to the window
     *
     * @param window the main window of the graphic interface
     */
    void Menu::DrawTo(sf::RenderWindow &window) {
        for( const auto &button : mMenuBar){
            button->DrawTo(window);
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
    std::string Menu::HandleMouseMove(sf::RenderWindow &window) {
        std::string s1 = "null";
        for (int i = 0; i < mMenuBar.size(); ++i) {
            if (mMenuBar[i]->IsMouseOver(window)) {
                mMenuBar[i]->SetBackColor(sf::Color::Magenta);
            } else {
                mMenuBar[i]->SetBackColor(sf::Color::Black);
            }
        }
        if(mInventory){
            s1 = mInventory->HandleMouseMove(window);
        }
        return s1;
    }
    /**
     * @brief check if the mouse click the exit button
     * closes window accordingly
     *
     * @param window the main window of the graphic interface
     */
    void Menu::HandleMouseButtonPressed(sf::RenderWindow &window,
                                        const std::vector<std::string> &interfaceAgentInventory) {
        if(mMenuBar[2]->IsMouseOver(window)){
            exit(0);
        }else if(mMenuBar[1]->IsMouseOver(window)){
            if(mInventory){
                DeconstructInventory();
            }else {
                ConstructInventory(interfaceAgentInventory);
            }
        }

    }
}


