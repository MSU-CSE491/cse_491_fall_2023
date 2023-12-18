/**
 * @file TextBox.hpp
 * @authors Gaya Kanagaraj, Vincenzo Felici, Mui Pham
 * @date: 11/02/2023
 * TextBox class enables interface entity to send message to the MessageBoard and World
 */

#pragma once

#include<iostream>
#include<SFML/Graphics.hpp>
#include<sstream>

namespace i_2D {

#define DELETE_KEY 8 ///< value for delete key
#define ENTER_KEY 13 ///< value for the enter key
#define ESCAPE_KEY 27 ///< value for the escape key

    /**
     * @class TextBox The TextBox is where users write to interface and the world
     */
    class TextBox {

    private:
        /// The string of the text
        std::unique_ptr<sf::Text> mText;
        std::ostringstream mStreamText; ///< Use to take in string
        bool isSelected = false; /// Flag for checking text mode
        bool hasLimit = true; /// Flag for checking limit mode
        int limit = 60; /// The limit of characters allowed
        // Draw the border around the TextBox
        sf::RectangleShape mBorderRect;

        void InputLogic(int charTyped);

        void DeleteLastChar();

    public:
        TextBox() = default;

        explicit TextBox(const sf::Font &font, int size = 25, sf::Color color = sf::Color::Red, bool sel = false);

        void SetString(const std::string &s);

        /**
         * @brief Set the font of the TextBox
         *
         * @param font The font to be set to
         */
        void SetFont(const sf::Font &font) {
            mText->setFont(font);
        }

        /**
         * @brief The position of the TextBox
         *
         * @param pos The position to be set to
         */
        void SetPosition(sf::Vector2f pos) {
            mText->setPosition(pos);
        }

        /**
         * @brief Set the limit of the TextBox
         *
         * @param ToF True of False
         */
        void SetLimit(bool ToF) {
            hasLimit = ToF;
        }

        /**
         * @brief Set the limit of the TextBox
         *
         * @param ToF True or False
         * @param lim The limit of the TextBox
         */
        void SetLimit(bool ToF, int lim) {
            hasLimit = ToF;
            limit = lim;
        }

        void SetSelected(bool sel);

        /**
         * @brief Get the string that was input by user
         *
         * @return Return the string
         */
        std::string GetText() {
            return mStreamText.str();
        }

        void DrawTo(sf::RenderWindow &window);

        void TypedOn(sf::Event input);

        /**
         * @brief Checks if TextBox is active
         *
         * @return True if active, else False
         */
        bool IsSelected() const {
            return isSelected;
        }

        /**
         * @brief Checks if a point is within the TextBox
         * @param point an xy point to check the location of
         * @return True if the point is in bounds, False otherwise
         */
        bool Contains(sf::Vector2f point) const {
            return mBorderRect.getGlobalBounds().contains(point);
        }

    };
}

