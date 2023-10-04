#include <iostream>

#include "SFML/Graphics.hpp"

int main(){
    
  // Create the window
  sf::RenderWindow  window(sf::VideoMode({800, 600}), "SFML works!");
  window.setFramerateLimit(60);
  // Create our tile and its properties
  const int tile_size = 20;
  sf::RectangleShape rect(sf::Vector2f(tile_size, tile_size));
  rect.setFillColor(sf::Color(200, 50, 0));
  sf::Vector2f rect_pos(window.getSize().x / 2, window.getSize().y / 2);
  sf::Vector2f rect_vel(200,200);

  sf::Clock clock; // Create a clock to help calculate delta time
  while(window.isOpen()){
    auto delta_time = clock.restart().asSeconds();

    // Handle input  
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) break;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)
          || sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
      break;
    }

    // Update rectangle, including collision
    rect_pos += rect_vel * delta_time;
    if(rect_pos.x + rect.getSize().x > window.getSize().x){
      rect_pos.x = window.getSize().x - rect.getSize().x;
      rect_vel.x *= -1;
    }
    else if(rect_pos.x < 0){
      rect_pos.x = 0;
      rect_vel.x *= -1;
    }
    if(rect_pos.y + rect.getSize().y > window.getSize().y){
      rect_pos.y = window.getSize().y - rect.getSize().y;
      rect_vel.y *= -1;
    }
    else if(rect_pos.y < 0){
      rect_pos.y = 0;
      rect_vel.y *= -1;
    }

    // Render!
    window.clear(sf::Color::Black);
    rect.setPosition(rect_pos);
    window.draw(rect);
    window.display();
  }
  return 0;
}
