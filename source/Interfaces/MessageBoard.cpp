//
// Created by muiph on 11/3/2023.
//

#include "MessageBoard.h"

i_2D::MessageBoard::MessageBoard(sf::Font &font) {
    mText = std::make_unique<sf::Text>(font);
    mText->setString("Hello World!");
    mText->setCharacterSize(35);
    mText->setFillColor(sf::Color::Blue);
    mText->setPosition({5,165});
    mStartTime = std::chrono::system_clock::now();
}

void i_2D::MessageBoard::DrawTo(sf::RenderWindow &window) {
    if(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - mStartTime).count() > 10000){
        mText->setString("");
    }else window.draw(*mText);
}


