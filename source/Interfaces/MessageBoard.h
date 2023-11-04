//
// Created by muiph on 11/3/2023.
//

#ifndef CSE_491_MESSAGEBOARD_H
#define CSE_491_MESSAGEBOARD_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <memory>
#include <chrono>

namespace i_2D {
    class MessageBoard {
    private:
        std::unique_ptr<sf::Text> mText;
        std::chrono::time_point<std::chrono::system_clock> mStartTime;

    public:
        explicit MessageBoard(sf::Font &font){
            mText = std::make_unique<sf::Text>(font);
            mText->setString("Hello World!");
            mText->setCharacterSize(35);
            mText->setFillColor(sf::Color::Blue);
            mText->setPosition({5,165});
            mStartTime = std::chrono::system_clock::now();
        }

        void drawTo(sf::RenderWindow &window){
            if(std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now() - mStartTime).count() > 2000){
                mText->setString("");
            }else
            window.draw(*mText);
        }

        void Send(const std::string &message){
            mText->setString(message);
            mStartTime = std::chrono::system_clock::now();
        }
    };
}


#endif //CSE_491_MESSAGEBOARD_H
