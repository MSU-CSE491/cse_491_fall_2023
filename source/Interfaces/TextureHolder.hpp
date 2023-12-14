/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A class maintaining a single texture.
 * @author Vincenzo Felici
 * @note Status: ALPHA
 **/

#pragma once

#include <map>

#include "../core/Data.hpp"
#include "../core/InterfaceBase.hpp"
#include "SFML/Graphics.hpp"

namespace i_2D {
using namespace cse491;
class TextureHolder {
private:
  std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures_;

public:
  TextureHolder() = default;
  ~TextureHolder() = default;
  void LoadTexture(std::string id, std::string file_name);
  const sf::Texture& GetTexture(std::string id);
  std::map<char, sf::Texture> MazeTexture();
  std::map<char, sf::Texture> SecondWorldTexture();
  std::map<char, sf::Texture> ManualWorldTexture();
  std::map<char, sf::Texture> GenerativeWorldTexture();
};
}  // namespace i_2D
