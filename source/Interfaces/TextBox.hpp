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

    class TextBox {
    private:
        std::unique_ptr<sf::Text> mTextBox;
        std::ostringstream mText;
        bool isSelected = false;
        bool hasLimit = false;
        int limit = 10000;

    public:
        TextBox()=default;

        explicit TextBox(const sf::Font &font ,int size = 35, sf::Color color = sf::Color::Green, bool sel = true);
        void SetString(const std::string &s);

        void SetFont(const sf::Font &font){
            mTextBox->setFont(font);
        }

        void SetPosition(sf::Vector2f pos){
            mTextBox->setPosition(pos);
        }

        void SetLimit(bool ToF){
            hasLimit = ToF;
        }

        void SetLimit(bool ToF, int lim){
            hasLimit = ToF;
            limit = lim - 1;
        }

        void SetSelected(bool sel);

        std::string GetText(){
            return mText.str();
        }

        void DrawTo(sf::RenderWindow &window){
            window.draw(*mTextBox);
        }

        void TypedOn(sf::Event input);

        bool IsSelected() const{
            return isSelected;
        }
    private:
        void InputLogic(int charTyped);

        void DeleteLastChar();
    };
}

#endif //CSE_491_TEXTBOX_HPP
