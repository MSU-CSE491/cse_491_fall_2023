/**
 * @author : Team - 3
 * @date: 11/02/2023
 * Button class creates a SF::Rectangle and SF::Text on top
 */

#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>

namespace i_2D {
    /***
     * @class Button
     *
     * @brief Simple RectangleShape and a Text inside the shape
     * Responsive rectangles on the Renderwindow that handles mouse's hover and click events
     */
    class Button {
    private:
        /// The Rectangle that the button resides in
        sf::RectangleShape button;
        /// The String printed onto the button
        std::unique_ptr<sf::Text> text;


    public:
        Button(const std::string &t, sf::Vector2f size, sf::Color bgColor,
               sf::Color textColor, const sf::Font &font);

        /**
         * @brief set the string of the button
         * @param s label of the button
         */
        void SetString(const std::string &s){
            text->setString(s);
        }

        void SetMFont(const sf::Font &font);

        /**
         * @brief set the font of the button
         * @param font
         */
        void SetFont(const sf::Font &font) {
            text->setFont(font);
        }

        /**
         * @brief set the button size
         * @param size
         */
        void SetButtonSize(sf::Vector2f size){
            button.setSize(size);
        }

        /**
         * @brief set the background color of the button
         * @param color
         */
        void SetBackColor(sf::Color color){
            button.setFillColor(color);
        }

        /**
         * @brief set the text color of the string
         * @param color
         */
        void SetTextColor(sf::Color color){
            text->setFillColor(color);
        }

        void SetPosition(sf::Vector2f pos);

        /**
         * @brief draws the button onto the window
         * @param window
         */
        void DrawTo(sf::RenderWindow &window){
            window.draw(button);
            window.draw(*text);
        }

        bool IsMouseOver(sf::RenderWindow &window);
    };
}

