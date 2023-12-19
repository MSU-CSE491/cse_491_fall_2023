/**
 * @file TextBox.cpp
 * @authors Gaya Kanagaraj, Vincenzo Felici, Mui Pham
 * @date: 11/02/2023
 * TextBox class enables interface entity to send message to the MessageBoard and World
 */

#include "TextBox.hpp"

namespace i_2D {

/**
 * @brief Constructor
 *
 * @param font The font used by TextBox
 * @param size The size of the text
 * @param color The color of the text
 * @param sel Set the activity of TextBox
 */
TextBox::TextBox(const sf::Font &font, int size, sf::Color color, bool sel)
{
  mText = std::make_unique<sf::Text>(font);
  mText->setCharacterSize(size);
  mText->setFillColor(color);
  mText->setPosition({10, 650});
  isSelected = sel;
  if (!isSelected) mText->setString("Press Click!");
}

/**
 * @brief Setter for isSelected, updates the text displayed in the box
 * @param sel whether the TextBox is to be set selected or not
 */
void TextBox::SetSelected(bool sel)
{
  isSelected = sel;
  if (!sel) {
    std::string t = mText.str();
    std::string newT;
    for (char i : t) {
      newT += i;
    }
    mTextBox->setString((newT));
  }
}

/**
 * @brief Set the string of the text
 *
 * @param s The string to be set to
 */
void TextBox::SetString(const std::string &s)
{
  mText->setString(s);
  mStreamText.str("");
  mStreamText << s;
}

/**
 * @brief Setter for isSelected, updates the text displayed in the box
 * @param sel whether the TextBox is to be set selected or not
 */
void TextBox::SetSelected(bool sel)
{
  isSelected = sel;
  if (!sel) {
    std::string t = mStreamText.str();
    std::string newT;
    for (char i : t) {
      newT += i;
    }
    mText->setString((newT));
  }
}
else
{
  InputLogic(charTyped);
}
}
}
}

/**
 * @brief TypedOn event handler
 * @param input the new text that has been typed into the TextBox
 */
void TextBox::TypedOn(sf::Event input)
{
  if (isSelected) {
    int charTyped = static_cast<int>(input.text.unicode);
    if (charTyped < 128) {
      if (hasLimit) {
        if (static_cast<int>(mStreamText.str().length()) < limit) {
          InputLogic(charTyped);
        } else if (static_cast<int>(mStreamText.str().length()) >= limit &&
                   charTyped == DELETE_KEY) {
          DeleteLastChar();
        }
      } else {
        InputLogic(charTyped);
      }
    }
  }
}
}

/**
 * @brief Determines what actions to take with different key presses
 * @param charTyped The character that has just been entered into the TextBox
 */
void TextBox::InputLogic(int charTyped)
{
  if (isSelected) {
    if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
      mStreamText << static_cast<char>(charTyped);
    } else if (charTyped == DELETE_KEY) {
      if (mStreamText.str().length() > 0) {
        DeleteLastChar();
      }
    }
  }

  mText->setString(mStreamText.str() + "_");
}

/**
 * @brief removes the last character from the TextBox's current string
 */
void TextBox::DeleteLastChar()
{
  std::string t = mStreamText.str();
  mStreamText.str("");
  mStreamText << t.substr(0, t.size() - 1);
  mText->setString(mStreamText.str());
}

mBorderRect.setPosition(mTextBox->getPosition() - sf::Vector2f(5, 5));
mBorderRect.setFillColor(sf::Color(200, 200, 200));
mBorderRect.setOutlineThickness(2.0f);  //  thickness of the border
mBorderRect.setOutlineColor(sf::Color::Black);

window.draw(mBorderRect);
window.draw(*mTextBox);
}

mBorderRect.setPosition(mText->getPosition() - sf::Vector2f(5, 5));
mBorderRect.setFillColor(sf::Color(200, 200, 200));
mBorderRect.setOutlineThickness(2.0f);  //  thickness of the border
mBorderRect.setOutlineColor(sf::Color::Black);

window.draw(mBorderRect);
window.draw(*mText);
}
}
