/**
 * @file MessageBoard.cpp
 * @authors Gaya Kanagaraj, Vincenzo Felici, Mui Pham
 * @date: 11/03/2023
 * MessageBoard class communicate messages to the interface entity
 */

#include "MessageBoard.hpp"

namespace i_2D {

    /**
     * @brief Construct the message board
     *
     * @param font The font used by message borad
     */
    MessageBoard::MessageBoard(sf::Font &font) {

        mText = std::make_unique<sf::Text>(font);
        mText->setString("Welcome!");
        mText->setCharacterSize(35);
        mText->setFillColor(sf::Color::Blue);
        mText->setPosition({5, 125});
        mStartTime = std::chrono::system_clock::now();
    }

    /**
     * @brief Draws the message board onto the render window
     *
     * @param window The render window to be drawn on
     */
    void MessageBoard::DrawTo(sf::RenderWindow &window) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now() - mStartTime).count() > 10000) {
            mText->setString("");
        } else window.draw(*mText);
    }

}
