/**
 * @authors Gaya Kanagaraj, Vincenzo Felici, Mui Pham
 * Inventory class creates an array of buttons and uses it as inventory
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "Button.hpp"

namespace i_2D {
/***
 * @class Inventory
 *
 * @brief Represent the inventory of entities.
 *
 * An interface/visual representation of an inventory.
 */
class Inventory {
private:
  /// Projected inventory screen
  std::unique_ptr<sf::RectangleShape> mCanvas;
  /// 2D array of buttons representing items
  std::vector<std::vector<std::unique_ptr<Button>>> mListOfButtons;
  sf::Vector2f mWorldSize;  ///< Size of the world
  size_t mCol = 0;             ///< Number of columns enumerating the inventory list
  size_t mRow = 0;             ///< Number of rows enumerating the inventory list

  std::vector<std::string> mItems;            ///< List of items in an inventory
  std::unique_ptr<sf::Texture> mItemDisplay;  ///< The image of the items currently on display

public:
  /// Set the Displayed item's texture
  void SetItemDisplay(sf::Texture &texture)
  {
    if (mItemDisplay) mItemDisplay.reset();
    mItemDisplay = std::make_unique<sf::Texture>(texture);
  }
  /// Constructor - save the value of the world's dimension
  explicit Inventory(const sf::Vector2f size) : mWorldSize(size) {}
  /// Set the value saved as the world's dimension
  void SetSize(const sf::Vector2f size) { mWorldSize = size; }
  void ConstructInventory(sf::Font &font, const std::vector<std::string> &interfaceAgentInventory);
  void DrawTo(sf::RenderWindow &window);
  std::string HandleMouseMove(sf::RenderWindow &window);
};
}  // namespace i_2D
