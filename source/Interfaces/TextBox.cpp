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
    mTextBox->setPosition({10,650});
    isSelected = sel;
    if(!isSelected)
        mTextBox->setString("Press Enter!");

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
                if(static_cast<int>(mText.str().length()) <= MAX_CHAR){
                    InputLogic(charTyped);
                }else if( static_cast<int>(mText.str().length()) > MAX_CHAR && charTyped == DELETE_KEY){
                    DeleteLastChar();
                }
            }else{
                InputLogic(charTyped);
            }
        }
    }
}

void i_2D::TextBox::InputLogic(int charTyped) {
    if(isSelected)
    {
        if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
            if (mText.str().length() < MAX_CHAR) {
                mText << static_cast<char>(charTyped);
            }
            else if (mText.str().length() >= MAX_CHAR) {
                hasLimit = true;
            }
        } else if (charTyped == ENTER_KEY) {
            // Handle line breaks
            if (mText.str().length() < MAX_CHAR) {
                mText << '\n';
                hasLimit = false;
            }
        } else if (charTyped == DELETE_KEY) {
            if (mText.str().length() > 0) {
                DeleteLastChar();
            }
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
/**
* @brief Draws the text to the render window
*
* @param window The render window to be drawn on
*/
void i_2D::TextBox::DrawTo(sf::RenderWindow &window){

    mBorderRect.setSize(sf::Vector2f(800, 50));
    //Subtracts the vector (5, 5) from the position of mTextBox. This creates a new position
    // slightly to the left and up from the original position, effectively creating a margin.

    mBorderRect.setPosition(mTextBox->getPosition() - sf::Vector2f(5, 5));
    mBorderRect.setFillColor(sf::Color(200, 200, 200));
    mBorderRect.setOutlineThickness(2.0f);  //  thickness of the border
    mBorderRect.setOutlineColor(sf::Color::Black);

    window.draw(mBorderRect);
    window.draw(*mTextBox);

}

