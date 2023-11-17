/**
 * @author : Team - 3
 * @date: 11/02/2023
 * TextBox class enables interface entity to send message to the MessageBoard and World
 */

#include "TextBox.hpp"

/**
 * @brief Constructor
 *
 * @param font The font used by TextBox
 * @param size The size of the text
 * @param color The color of the text
 * @param sel Set the activity of TextBox
 */
i_2D::TextBox::TextBox(const sf::Font &font, int size, sf::Color color, bool sel) {
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
/**
 * @brief Set the string of the text
 *
 * @param s The string to be set to
 */
void i_2D::TextBox::SetString(const std::string &s) {
    mTextBox->setString(s);
    mText.str("");
    mText << s;
}

void i_2D::TextBox::SetSelected(bool sel) {
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

void i_2D::TextBox::TypedOn(sf::Event input) {
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

void i_2D::TextBox::InputLogic(int charTyped) {
    if(charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY){
        mText << static_cast<char>(charTyped);
    }else if (charTyped == DELETE_KEY){
        if(mText.str().length() > 0){
            DeleteLastChar();
        }
    }
    mTextBox->setString(mText.str() + "_");
}

void i_2D::TextBox::DeleteLastChar() {
    std::string t = mText.str();
    std::string newT;
    for(unsigned int i = 0; i < t.length() - 1; i++){
        newT += t[i];
    }
    mText.str("");
    mText << newT;
    mTextBox->setString(mText.str());
}
