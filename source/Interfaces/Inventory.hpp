/**
 * @author : Team - 3
 * @date: 11/15/2023
 * Inventory class creates an array of buttons and displays the inventory
 */

#ifndef CSE_491_INVENTORY_HPP
#define CSE_491_INVENTORY_HPP

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include <vector>
#include <memory>

namespace i_2D {
    class Inventory {
    private:
        std::unique_ptr<sf::RectangleShape> mInventoryWindow;
        std::vector<std::vector<std::unique_ptr<Button>>> mInventoryList;
        sf::Vector2f mWorldSize;
        int mCol=0;
        int mRow=0;

    public:
        explicit Inventory(const sf::Vector2f size) : mWorldSize(size) {}

        void SetSize(const sf::Vector2f size) {mWorldSize = size;}
        void ConstructInventory(sf::Font &font){
            mInventoryWindow = std::make_unique<sf::RectangleShape>();
            mInventoryWindow->setSize({mWorldSize.x,mWorldSize.y/2});
            mInventoryWindow->setFillColor(sf::Color::Black);
            mInventoryWindow->setPosition(sf::Vector2f{0.f,50.f});
            if(mWorldSize.x > 1800){
                mCol = 5;
            }else mCol = 3;
            if(mWorldSize.y > 900){
                mRow = 4;
            }else mRow = 3;
            for(int i = 0; i < mRow; i++) {
                std::vector<std::unique_ptr<Button>> v1;
                for (int j = 0; j < mCol; j++) {
                    v1.push_back(std::make_unique<Button>(
                            "text", sf::Vector2f{(mWorldSize.x)/mCol,(mWorldSize.y/2-50)/mRow},
                            sf::Color::Black, sf::Color::White, font));
                    v1[j]->setPosition(sf::Vector2f{i*(mWorldSize.x/mCol),
                                                    mWorldSize.y/2 + 50 + j*(mWorldSize.y/2-50)/mRow});
                }
                mInventoryList.push_back(std::move(v1));
            }
        }
        void DrawTo(sf::RenderWindow &window){
            window.draw(*mInventoryWindow);
            for(const auto &x : mInventoryList){
                for(const auto &y : x){
                    y->drawTo(window);
                }
            }

        }
    };
}


#endif //CSE_491_INVENTORY_HPP
