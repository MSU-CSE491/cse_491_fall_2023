/**
 * @author : Team - 3
 * @date: 11/03/2023
 * MessageBoard class communicate messages to the interface entity
 */

#include "MessageBoard.h"

/**
 * @brief Construct the message board
 *
 * @param font The font used by message borad
 */
i_2D::MessageBoard::MessageBoard(sf::Font &font) {
    mText = std::make_unique<sf::Text>(font);
    mText->setString("Hello World!");
    mText->setCharacterSize(35);
    mText->setFillColor(sf::Color::Blue);
    mText->setPosition({5,165});
    mStartTime = std::chrono::system_clock::now();
}

/**
 * @brief Draws the message board onto the render window
 *
 * @param window The render window to be drawn on
 */
void i_2D::MessageBoard::DrawTo(sf::RenderWindow &window) {
    if(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - mStartTime).count() > 10000){
        mText->setString("");
    }else window.draw(*mText);
}


