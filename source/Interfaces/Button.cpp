/**
 * @file Button.cpp
 * @author muiph
 */

#include "Button.hpp"

/// Constructor
i_2D::Button::Button(std::string t, sf::Vector2f size, sf::Color bgColor, sf::Color textColor) {
    if(!mfont.loadFromFile("../../assets/font/ArialNarrow7.ttf")){
        std::cout << "Error loading font file" << std::endl;
    }
    text = std::make_unique<sf::Text>(mfont);

    text->setString(t);
    text->setFillColor(textColor);

    button.setSize(size);
    button.setFillColor(bgColor);
}

/**
 * @brief set the font of the button
 */
void i_2D::Button::setMFont() {
    if(!mfont.loadFromFile("../../assets/font/ArialNarrow7.ttf")){
        std::cout << "Error loading font file" << std::endl;
    }
    text = std::make_unique<sf::Text>(mfont);
}

/**
 * @brief set the position of the button
 *
 * @param pos position on the window
 */
void i_2D::Button::setPosition(sf::Vector2f pos) {
    button.setPosition(pos);

    float xPos = (pos.x + button.getGlobalBounds().width / 2) - (text->getLocalBounds().width/2);
    float yPos = (pos.y + button.getGlobalBounds().height / 2) - (text->getLocalBounds().height/2);
    text->setPosition({xPos, yPos});
}

/**
 * @brief checks if mouse position is in a button
 *
 * @param window
 *
 * @return returns turn if mouse position is in the button
 */
bool i_2D::Button::isMouseOver(sf::RenderWindow &window) {
    float mouseX = sf::Mouse::getPosition(window).x;
    float mouseY = sf::Mouse::getPosition(window).y;

    float btnPosX = button.getPosition().x;
    float btnPosY = button.getPosition().y;

    float btnxPosWidth = button.getPosition().x + button.getLocalBounds().width;
    float btnyPosHeight = button.getPosition().y + button.getLocalBounds().height;

    if(mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY){
        return true;
    }
    return false;
}
