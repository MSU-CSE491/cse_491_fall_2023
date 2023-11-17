/**
 * @author : Team - 3
 * @date: 11/03/2023
 * MessageBoard class communicate messages to the interface entity
 */
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
        explicit MessageBoard(sf::Font &font);
        void DrawTo(sf::RenderWindow &window);

        void Send(const std::string &message){
            mText->setString(message);
            mStartTime = std::chrono::system_clock::now();
        }
    };
}


#endif //CSE_491_MESSAGEBOARD_H
