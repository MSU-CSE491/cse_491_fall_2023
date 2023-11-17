//
// Created by muiph on 11/15/2023.
//

#include "Inventory.hpp"

void i_2D::Inventory::ConstructInventory(sf::Font &font) {
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
            v1[j]->setPosition(sf::Vector2f{j*(mWorldSize.x/mCol),
                                            mWorldSize.y/2 + 50 + i*(mWorldSize.y/2-50)/mRow});
        }
        mInventoryList.push_back(std::move(v1));
    }
}

void i_2D::Inventory::DrawTo(sf::RenderWindow &window) {
    window.draw(*mInventoryWindow);
    for(const auto &x : mInventoryList){
        for(const auto &y : x){
            y->drawTo(window);
        }
    }
}

void i_2D::Inventory::HandleMouseMove(sf::RenderWindow &window) {
    for(auto &x : mInventoryList){
        for(auto &y : x){
            if(y->isMouseOver(window)){
                y->setBackColor(sf::Color::Magenta);
            }else y->setBackColor(sf::Color::Black);
        }
    }
}
