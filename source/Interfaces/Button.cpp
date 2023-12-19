/**
 * @file Button.cpp
 * @author muiph
 */

#include "Button.hpp"

namespace i_2D {
/**
 * Constructs the button
 *
 * @brief Construct the text with font. Then set text and shape properties with params.
 * @param t the string to be displayed by button
 * @param size the size of the rectangle shape
 * @param bgColor the background color of the shape
 * @param textColor the color of the text
 * @param font the font style
 */
Button::Button(const std::string &t, sf::Vector2f size, sf::Color bgColor, sf::Color textColor,
               const sf::Font &font)
{
  text = std::make_unique<sf::Text>(font);

  text->setString(t);
  text->setFillColor(textColor);

  button.setSize(size);
  button.setFillColor(bgColor);
}

/**
 * @brief set the font of the button
 */
void Button::SetMFont(const sf::Font &font) { text = std::make_unique<sf::Text>(font); }

/**
 * @brief set the position of the button
 * @param pos position on the window
 */
void Button::SetPosition(sf::Vector2f pos)
{
  button.setPosition(pos);
  float xPos = (pos.x + button.getGlobalBounds().width / 2) - (text->getLocalBounds().width / 2);
  float yPos = (pos.y + button.getGlobalBounds().height / 2) - (text->getLocalBounds().height / 2);
  text->setPosition({xPos, yPos});
}

/**
 * @brief checks if mouse position is in a button
 * @param window
 * @return returns turn if mouse position is in the button
 */
bool Button::IsMouseOver(sf::RenderWindow &window)
{
  float mouseX = sf::Mouse::getPosition(window).x;
  float mouseY = sf::Mouse::getPosition(window).y;

  float btnPosX = button.getPosition().x;
  float btnPosY = button.getPosition().y;

  float btnXPosWidth = button.getPosition().x + button.getLocalBounds().width;
  float btnYPosHeight = button.getPosition().y + button.getLocalBounds().height;

  if (mouseX < btnXPosWidth && mouseX > btnPosX && mouseY < btnYPosHeight && mouseY > btnPosY) {
    return true;
  }
  return false;
}
}  // namespace i_2D