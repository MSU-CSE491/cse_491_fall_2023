/**
 * @file Menu.cpp
 * @author
 */

#include "Menu.h"
#include <SFML/Window.hpp>

namespace i_2D{
    Menu::Menu(float width, float height){

        //menu[0].setFont(font);
        menu[0].setFillColor(sf::Color::Red);
        menu[0].setString("Play");
        menu[0].setPosition(sf::Vector2f(width / 2, height / MAX_NUMBER_OF_ITEMS_MENU_ITEMS +1) * 1.f);

        //menu[1].setFont(font);
        menu[1].setFillColor(sf::Color::White);
        menu[1].setString("Options");
        menu[1].setPosition(sf::Vector2f(width / 2, height / MAX_NUMBER_OF_ITEMS_MENU_ITEMS +1) * 1.f);

        //menu[2].setFont(font);
        menu[2].setFillColor(sf::Color::White);
        menu[2].setString("Exit");
        menu[2].setPosition(sf::Vector2f(width / 2, height / MAX_NUMBER_OF_ITEMS_MENU_ITEMS +1) * 1.f);

        selectedItemIndex = 0;
    }

    Menu::~Menu() {
    }

    void Menu::draw(sf::RenderWindow &window) {
        for (int i = 0; i < MAX_NUMBER_OF_ITEMS_MENU_ITEMS; i++){
            window.draw(menu[i]);
        }
    }

    void Menu::Start(){
        sf::RenderWindow window(sf::VideoMode({600, 600}), "SFML WORK!");

        while (window.isOpen()){
            sf::Event event;

            while (window.pollEvent(event)){
                switch (event.type)
                {
                case sf::Event::Closed:
                    window.close();
                    break;
                }
            }
        }

        window.clear();

        draw(window);

        window.display();
    }
    void Menu::MoveUp(){
        if (selectedItemIndex - 1 >= 0){
            menu[selectedItemIndex].setFillColor(sf::Color::White);
            selectedItemIndex--;
            menu[selectedItemIndex].setFillColor(sf::Color::Red);
        }
    }

    void Menu::MoveDown(){
        if (selectedItemIndex + 1 >= MAX_NUMBER_OF_ITEMS_MENU_ITEMS){
            menu[selectedItemIndex].setFillColor(sf::Color::White);
            selectedItemIndex++;
            menu[selectedItemIndex].setFillColor(sf::Color::Red);
        }
    }
}


///!!! CODE LOGIC FOR MENU MOVE UP AND DOWN
//{
//    sf::RenderWindow window(sf::VideoMode({600, 600}), "SFML WORK!");
//    i_2D::Menu menu();
//
//    while (window.isOpen()){
//        sf::Event event;
//
//        while (window.pollEvent(event)){
//            switch (event.type)
//            {
//            case sf::Event::KeyReleased:
//                switch (event.key.code){
//                case sf::KeyBoard::Up:
//                    menu.MoveUp();
//                    break;
//
//                case sf::KeyBoard::Down:
//                    menu.MoveDOwn();
//                    break;
//
//                case sf::KeyBoard::Return:
//                    switch (menu.GetPressedItem()){
//                    case 0:
//                        std::cout << "Play button has been pressed" << std::endl;
//                    case 1:
//                        std::cout << "Option button has been pressed" << std::endl;
//                    case 2:
//                        window.close();
//                        break;
//                    }
//                    break;
//                }
//            case sf::Event::Closed:
//            window.close();
//            break;
//            }
//        }
//    }
//
//    window.clear();
//
//    draw(window);
//
//    window.display();
//}


