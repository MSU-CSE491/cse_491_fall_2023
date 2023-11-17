/**
 * @author : Team - 3
 * @date: 11/02/2023
 * TextBox class enables interface entity to send message to the MessageBoard and World
 */

#ifndef CSE_491_TEXTBOX_HPP
#define CSE_491_TEXTBOX_HPP

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

    public:
        TextBox()=default;
        explicit TextBox(const sf::Font &font ,int size = 35, sf::Color color = sf::Color::Green, bool sel = true);
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
        /**
         * @brief Draws the text to the render window
         *
         * @param window The render window to be drawn on
         */
        void DrawTo(sf::RenderWindow &window){
            window.draw(*mTextBox);
        }
        void TypedOn(sf::Event input);
        /**
         * @brief Checks if TextBox is active
         *
         * @return True if active, else False
         */
        bool IsSelected() const{
            return isSelected;
        }
    private:
        void InputLogic(int charTyped);
        void DeleteLastChar();
    };
}

#endif //CSE_491_TEXTBOX_HPP
