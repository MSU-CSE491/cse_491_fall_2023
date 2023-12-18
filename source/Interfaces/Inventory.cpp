/**
 * @author : Team - 3
 * @date: 11/15/2023
 * Inventory class creates an array of buttons and uses it as inventory
 */

#include <algorithm>
#include "Inventory.hpp"

namespace i_2D {
/**
 * @brief Build an inventory window and an inventory list
 * @param font The font used by the inventory list
 */
    void Inventory::ConstructInventory(sf::Font &font, const std::vector<std::string> &interfaceAgentInventory) {
        mCanvas = std::make_unique<sf::RectangleShape>();
        mCanvas->setSize({mWorldSize.x, mWorldSize.y / 2});
        mCanvas->setFillColor(sf::Color::Black);
        mCanvas->setPosition(sf::Vector2f{0.f, 50.f});

        mItems.clear();
        mItems.resize(interfaceAgentInventory.size());
        std::copy(interfaceAgentInventory.begin(), interfaceAgentInventory.end(), 
            mItems.begin());

        // Set row & col sizes
        if (mWorldSize.x > 1800) {
            mCol = 5;
        } else mCol = 3;
        if (mWorldSize.y > 900) {
            mRow = 4;
        } else mRow = 3;

        // Create 2d vector of buttons
        size_t Index = 0;
        for (int i = 0; i < mRow; i++) {
            std::vector<std::unique_ptr<Button>> v1;
            for (int j = 0; j < mCol; j++) {
                v1.push_back(std::make_unique<Button>(
                        "", sf::Vector2f{(mWorldSize.x) / mCol, (mWorldSize.y / 2 - 50) / mRow},
                        sf::Color::Black, sf::Color::White, font));
                if (Index < mItems.size()) {
                    v1[j]->SetString(mItems[Index]);
                    ++Index;
                } else {
                    v1[j]->SetString("empty");
                }
                v1[j]->SetPosition(sf::Vector2f{j * (mWorldSize.x / mCol),
                                                mWorldSize.y / 2 + 50 + i * (mWorldSize.y / 2 - 50) / mRow});
            }
            mListOfButtons.push_back(std::move(v1));
        }
    }

/**
 * @brief Draws the inventory onto the render window
 * @param window The render window of the world
 */
    void Inventory::DrawTo(sf::RenderWindow &window) {
        window.draw(*mCanvas);
        for (const auto &x: mListOfButtons) {
            for (const auto &y: x) {
                y->DrawTo(window);
            }
        }
        if(mItemDisplay) {
            sf::RectangleShape rect(
                    sf::Vector2f(window.getSize().x/2-10,(window.getSize().y-50)/2-10));
            rect.setPosition({5,55});
            rect.setTexture(&*mItemDisplay);
            window.draw(rect);
        }
    }

/**
 * @brief Handle the mouse move event
 * @param window The render window of the world
 */
    std::string Inventory::HandleMouseMove(sf::RenderWindow &window) {
        std::string s1 = "null";
        for(int i = 0; i < mListOfButtons.size(); ++i){
            for(int j = 0; j < mListOfButtons.size(); ++j){
                if(mListOfButtons[i][j]->IsMouseOver(window)){
                    mListOfButtons[i][j]->SetBackColor(sf::Color::Magenta);
                    if(i*mCol+j < mItems.size()){
                        s1 = (mItems[i*mCol+j]);
                        if(s1 == "Boots"){
                            s1 = s1.substr(0,s1.size()-1);
                        }
                        s1[0] = tolower(s1[0]);
                        s1 += "Texture";
                    }
                }else{
                    mListOfButtons[i][j]->SetBackColor(sf::Color::Black);
                }
            }
        }
        return s1;
    }
}
