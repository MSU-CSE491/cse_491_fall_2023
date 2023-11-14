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
        Button(std::string t, sf::Vector2f size, sf::Color bgColor, sf::Color textColor);

        /**
         * @brief set the string of the button
         *
         * @param s label of the button
         */
        void setString(std::string s){
            text->setString(s);
        }

        void setMFont();

        /**
         * @brief set the font of the button
         *
         * @param font
         */
        void setFont(sf::Font &font) {
            text->setFont(font);
        }

        /**
         * @brief set the button size
         *
         * @param size
         */
        void setButtonSize(sf::Vector2f size){
            button.setSize(size);
        }

        /**
         * @brief set the background color of the button
         *
         * @param color
         */
        void setBackColor(sf::Color color){
            button.setFillColor(color);
        }

        /**
         * @brief set the text color of the string
         *
         * @param color
         */
        void setTextColor(sf::Color color){
            text->setFillColor(color);
        }

        void setPosition(sf::Vector2f pos);

        /**
         * @brief draws the button onto the window
         *
         * @param window
         */
        void drawTo(sf::RenderWindow &window){
            window.draw(button);
            window.draw(*text);
        }

        bool isMouseOver(sf::RenderWindow &window);
    };
}

#endif //CSE_491_BUTTON_HPP
