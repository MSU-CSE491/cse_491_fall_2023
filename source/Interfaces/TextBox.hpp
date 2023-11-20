/**
 * @author : Team - 3
 * @date: 11/02/2023
 * TextBox class enables interface entity to send message to the MessageBoard and World
 */

#pragma once

#include<iostream>
#include<SFML/Graphics.hpp>
#include<sstream>

namespace i_2D {
#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

    /**
     * @class The TextBox is where users write to interface and the world
     */
    class TextBox {
    private:
        /// The string of the text
        std::unique_ptr<sf::Text> mTextBox;
        std::ostringstream mText; ///< Use to take in string
        bool isSelected = false; /// Flag for checking text mode
        bool hasLimit = false; /// Flag for checking limit mode
        int limit = 10000; /// The limit of characters allowed
        const int MAX_CHAR = 60; ///< max character per line in the textbox
        // Draw the border around the TextBox
        sf::RectangleShape mBorderRect;

    public:
        TextBox()=default;
        explicit TextBox(const sf::Font &font ,int size = 25, sf::Color color = sf::Color::Red, bool sel = false);
        void SetString(const std::string &s);
        /**
         * @brief Set the font of the TextBox
         *
         * @param font The font to be set to
         */
        void SetFont(const sf::Font &font){
            mTextBox->setFont(font);
        }
        /**
         * @brief The position of the TextBox
         *
         * @param pos The position to be set to
         */
        void SetPosition(sf::Vector2f pos){
            mTextBox->setPosition(pos);
        }
        /**
         * @brief Set the limit of the TextBox
         *
         * @param ToF True of False
         */
        void SetLimit(bool ToF){
            hasLimit = ToF;
        }
        /**
         * @brief Set the limit of the TextBox
         *
         * @param ToF True or False
         * @param lim The limit of the TextBox
         */
        void SetLimit(bool ToF, int lim){
            hasLimit = ToF;
            limit = lim - 1;
        }
        void SetSelected(bool sel);
        /**
         * @brief Get the string that was input by user
         *
         * @return Return the string
         */
        std::string GetText(){
            return mText.str();
        }
        void DrawTo(sf::RenderWindow &window);
        void TypedOn(sf::Event input);
        /**
         * @brief Checks if TextBox is active
         *
         * @return True if active, else False
         */
        bool IsSelected() const{
            return isSelected;
        }

        bool Contains(sf::Vector2f point) const {
            return mBorderRect.getGlobalBounds().contains(point);
        }

    private:
        void InputLogic(int charTyped);
        void DeleteLastChar();
    };
}

