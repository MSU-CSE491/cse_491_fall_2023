//
// Created by muiph on 11/2/2023.
//

#ifndef CSE_491_TEXTBOX_H
#define CSE_491_TEXTBOX_H

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

        explicit TextBox(const sf::Font &font ,int size = 35, sf::Color color = sf::Color::Green, bool sel = true){
            mTextBox = std::make_unique<sf::Text>(font);
            mTextBox->setCharacterSize(size);
            mTextBox->setFillColor(color);
            mTextBox->setPosition({5,600});
            isSelected = sel;
            if(sel){
                mTextBox->setString("_");
            }else{
                mTextBox->setString("");
            }
        }

        void SetString(const std::string &s){
            mTextBox->setString(s);
            mText.str("");
            mText << s;
        }

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

        void SetSelected(bool sel){
            isSelected = sel;
            if(!sel){
                std::string t = mText.str();
                std::string newT;
                for(char i : t){
                    newT += i;
                }
                mTextBox->setString((newT));
            }
        }

        std::string GetText(){
            return mText.str();
        }

        void DrawTo(sf::RenderWindow &window){
            window.draw(*mTextBox);
        }

        void TypedOn(sf::Event input){
            if(isSelected){
                int charTyped = static_cast<int>(input.text.unicode);
                if(charTyped < 128){
                    if(hasLimit){
                        if(static_cast<int>(mText.str().length()) <= limit){
                            InputLogic(charTyped);
                        }else if( static_cast<int>(mText.str().length()) > limit && charTyped == DELETE_KEY){
                            DeleteLastChar();
                        }
                    }else{
                        InputLogic(charTyped);
                    }
                }
            }
        }

        bool IsSelected() const{
            return isSelected;
        }
    private:
        void InputLogic(int charTyped){
            if(charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY){
                mText << static_cast<char>(charTyped);
            }else if (charTyped == DELETE_KEY){
                if(mText.str().length() > 0){
                    DeleteLastChar();
                }
            }
            mTextBox->setString(mText.str() + "_");
        }

        void DeleteLastChar() {
            std::string t = mText.str();
            std::string newT;
            for(unsigned int i = 0; i < t.length() - 1; i++){
                newT += t[i];
            }
            mText.str("");
            mText << newT;
            mTextBox->setString(mText.str());
        }
    };
}

#endif //CSE_491_TEXTBOX_H
