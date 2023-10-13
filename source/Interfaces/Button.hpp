/**
 * @file Button.h
 * @author
 *
 *
 */

#ifndef CSE_491_BUTTON_HPP
#define CSE_491_BUTTON_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>

namespace i_2D {
    class Button {
    private:
        sf::RectangleShape button;
        sf::Font mfont;
        std::unique_ptr<sf::Text> text;


    public:
        Button() { };
        Button(std::string t, sf::Vector2f size, int charSize, sf::Color bgColor, sf::Color textColor) {
            if(!mfont.loadFromFile("C:\\Users\\muiph\\OneDrive\\Desktop\\gaya\\assets\\font\\ArialNarrow7.ttf")){
                std::cout << "Error loading font file" << std::endl;
            }
            text = std::make_unique<sf::Text>(mfont);

            text->setString(t);
            text->setFillColor(textColor);
            text->setCharacterSize(charSize);

            button.setSize(size);
            button.setFillColor(bgColor);

        }

        void setString(std::string s){
            text->setString(s);
        }
        void setMFont(){
            if(!mfont.loadFromFile("..\\..\\assets\\font\\ArialNarrow7.ttf")){
                std::cout << "Error loading font file" << std::endl;
            }
            text = std::make_unique<sf::Text>(mfont);
        }
        void setFont(sf::Font &font) {
            text->setFont(font);
        }

        void setButtonSize(sf::Vector2f size){
            button.setSize(size);
        }

        void setBackColor(sf::Color color){
            button.setFillColor(color);
        }

        void setTextColor(sf::Color color){
            text->setFillColor(color);
        }

        void setPosition(sf::Vector2f pos){
            button.setPosition(pos);


            float xPos = (pos.x + button.getGlobalBounds().width / 2) - (text->getLocalBounds().width/2);
            float yPos = (pos.y + button.getGlobalBounds().height / 2) - (text->getLocalBounds().height/2);
            text->setPosition({xPos, yPos});
        }

        void drawTo(sf::RenderWindow &window){
            window.draw(button);
            window.draw(*text);
        }

        bool isMouseOver(sf::RenderWindow &window){
            float mouseX = sf::Mouse::getPosition(window).x;
            float mouseY = sf::Mouse::getPosition(window).y;

            float btnPosX = button.getPosition().x;
            float btnPosY = button.getPosition().y;

            float btnxPosWidth = button.getPosition().x + button.getLocalBounds().width;
            float btnyPosHeight = button.getPosition().y + button.getLocalBounds().height;

            if(mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY){
                return true;
            }
            return false;
        }

    };
}

#endif //CSE_491_BUTTON_HPP
