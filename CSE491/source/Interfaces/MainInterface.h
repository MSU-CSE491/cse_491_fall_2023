/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A low-end text interface providing a bare-minimum level of interaction.
 * @note Status: PROPOSAL
 **/

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <vector>
#include <sstream>

#include "../core/Data.hpp"
#include "../core/InterfaceBase.hpp"

namespace cse491 {
    namespace i_2D {

        class MainInterface : public InterfaceBase { // Rename to mainInterface

        protected:
            bool wait_for_input = true;
            sf::RenderWindow window;
            sf::Font font;
            sf::Text consoleText;
            sf::Clock clock;
            sf::Vector2i characterPosition; // Add character position variable


        public:
            MainInterface(size_t id, const std::string & name): InterfaceBase(id, name) {
                window.create(sf::VideoMode(800, 600), "Maze Window");
                if (!font.loadFromFile("/System/Library/Fonts/HelveticaNeue.ttc")) {
                    std::cerr << "Failed to load system default font!" << std::endl;
                    // Handle the error or continue without custom font
                }
                consoleText.setFont(font);
                consoleText.setCharacterSize(24);
                consoleText.setPosition(10, 10); // Adjust the position as needed
                characterPosition.x = 0; // X-coordinate
                characterPosition.y = 0; // Y-coordinate

            }

            ~MainInterface() = default;

            void DrawGrid(const WorldGrid &grid, const type_options_t &type_options,
                          const item_set_t &item_set, const agent_set_t &agent_set) {
                // Clear the window
                window.clear();

                // Set the size of each cell in pixels (adjust this to fit your needs)
                float cellSize = 30.0f;

                // Calculate the total size of the grid
                float gridWidth = static_cast<float>(grid.GetWidth()) * cellSize;
                float gridHeight = static_cast<float>(grid.GetHeight()) * cellSize;

                // Calculate the position to center the grid in the window
                float gridX = (window.getSize().x - gridWidth) / 2.0f;
                float gridY = (window.getSize().y - gridHeight) / 2.0f;

                // Draw the grid based on the symbol_grid
                for (size_t y = 0; y < grid.GetHeight(); ++y) {
                    for (size_t x = 0; x < grid.GetWidth(); ++x) {
                        // Calculate the position for this cell
                        float posX = gridX + static_cast<float>(x) * cellSize;
                        float posY = gridY + static_cast<float>(y) * cellSize;

                        // Check the symbol in the symbol_grid
                        char symbol = type_options[grid.At(x, y)].symbol;

                        // Create a rectangle for each cell
                        sf::RectangleShape cellRect(sf::Vector2f(cellSize, cellSize));
                        cellRect.setPosition(posX, posY);

                        // Draw walls
                        if (symbol == '#') {
                            sf::Color brown(139, 69, 19);
                            cellRect.setFillColor(brown);

                        }
                            // Draw agents/entities
                        else if (symbol == '+' || symbol == '*') {
                            cellRect.setFillColor(sf::Color::Red);
                        }
                        // Draw the cell
                      window.draw(cellRect);

                    }

                }
                sf::Text characterText("*", font, 36); // Increase the font size to 36 (adjust as needed)

                // Calculate the position of the character text within the grid cell
                float characterX = gridX + characterPosition.x * cellSize + (cellSize - characterText.getLocalBounds().width) / 2;
                float characterY = gridY + characterPosition.y * cellSize + (cellSize - characterText.getLocalBounds().height) / 2;


                characterText.setPosition(characterX,characterY);

                // Set the color of the character text (e.g., red)
                characterText.setFillColor(sf::Color::Magenta);

                // Draw the character text (on top of the grid)
                window.draw(characterText);

                // Display everything
                window.display();
            }


            bool Initialize() override {
                return true;
            }

            size_t SelectAction(const WorldGrid & grid,
                                const type_options_t & type_options,
                                const item_set_t & item_set,
                                const agent_set_t & agent_set) override {

                while (window.isOpen()) {

                    sf::Event event;
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed)
                            window.close();
                    }
                    DrawGrid(grid, type_options, item_set, agent_set);

                    // See if there are any keys waiting in standard input (wait if needed)
                    char input;
                    do {
                        std::cin >> input;
                    } while (!std::cin && wait_for_input);

//                    // Respond to the user input...
                    size_t action_id = 0;
//                    switch (input) {
//                        case 'w':
//                        case 'W':
//                            characterPosition.y = std::max(0, characterPosition.y - 1); // Move up
//                            break;
//                        case 'a':
//                        case 'A':
//                            characterPosition.x = std::max(0, characterPosition.x - 1); // Move left
//                            break;
//                        case 's':
//                        case 'S':
//                            characterPosition.y = std::min(static_cast<int>(grid.GetHeight()) - 1, characterPosition.y + 1); // Move down
//                            break;
//                        case 'd':
//                        case 'D':
//                            characterPosition.x = std::min(static_cast<int>(grid.GetWidth()) - 1, characterPosition.x + 1); // Move right
//                            break;
//                        case 'q':
//                        case 'Q':
//                            exit(0); // Quit!
//                    }
//
//                    // If we waited for input, but don't understand it, notify the user.
//                    if (wait_for_input && action_id == 0) {
//                        std::cout << "Unknown key '" << input << "'." << std::endl;
//                    }

                    // Do the action!
                    return action_id;
                }
                return 0;
            }

        };

    } // End of namespace 2D
} // End of namespace cse491
