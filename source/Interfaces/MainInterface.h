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
            WorldGrid wGrid;

        public:
            MainInterface(size_t id, const std::string &name) : InterfaceBase(id, name) {
                window.create(sf::VideoMode(1000, 800), "Maze Window");
                if (!font.loadFromFile("/System/Library/Fonts/HelveticaNeue.ttc")) {
                    std::cerr << "Failed to load system default font!" << std::endl;
                    // Handle the error or continue without custom font
                }
                consoleText.setFont(font);
//                window.display();

            }

            ~MainInterface() = default;

            void SetGrid(WorldGrid wg){
                wGrid= wg;
            }

            void DrawGrid(const WorldGrid &grid, const type_options_t &type_options,
                          const item_set_t &item_set, const agent_set_t &agent_set) {
                // Clear the window
                window.clear();
                std::vector<std::string> symbol_grid(grid.GetHeight());
                // Load the world into the symbol_grid;
                for (size_t y = 0; y < grid.GetHeight(); ++y) {
                    symbol_grid[y].resize(grid.GetWidth());
                    for (size_t x = 0; x < grid.GetWidth(); ++x) {
                        symbol_grid[y][x] = type_options[grid.At(x, y)].symbol;
                    }
                }

                // Add in the agents / entities
                for (const auto &entity_ptr: item_set) {
                    GridPosition pos = entity_ptr->GetPosition();
                    symbol_grid[pos.CellY()][pos.CellX()] = '+';
                }

                for (const auto &agent_ptr: agent_set) {
                    GridPosition pos = agent_ptr->GetPosition();
                    char c = '*';
                    if (agent_ptr->HasProperty("char")) {
                        c = static_cast<char>(agent_ptr->GetProperty("char"));
                    }
                    symbol_grid[pos.CellY()][pos.CellX()] = c;
                }
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
                        char symbol = symbol_grid[y][x];



                        // Create a rectangle for each cell
                        sf::RectangleShape cellRect(sf::Vector2f(cellSize, cellSize));
                        cellRect.setPosition(posX, posY);

                        // Draw walls
                        if (symbol == '#') {
                            sf::Color brown(139, 69, 19);
                            cellRect.setFillColor(brown);
                            window.draw(cellRect);

                        } else if (symbol == ' ') {
                            sf::Color pink(205, 204, 207);
                            cellRect.setFillColor(pink);
                            window.draw(cellRect);

                        }
                            // Draw agents/entities
                        else if (symbol == '*') {
//                            // Set the fill color of the rectangle to magenta.
                            cellRect.setFillColor(sf::Color(205, 204, 207));

                            // Create a text object for the `*` character.
                            sf::Text characterText("*", font, 36); // Increase the font size to 36 (adjust as needed)

                            // Calculate the position for the text object.
                            float characterX = posX + cellSize / 2 - characterText.getLocalBounds().width / 2;
                            float characterY = posY + cellSize / 2 - characterText.getLocalBounds().height / 2;

                            // Set the position of the text object.
                            characterText.setPosition(characterX, characterY);

                            // Set the color of the text object to green.
                            characterText.setFillColor(sf::Color::Blue);

                            // Draw the rectangle and the text object.
                            window.draw(cellRect);
                            window.draw(characterText);
                        } else {
                            cellRect.setFillColor(sf::Color(205, 204, 207));
                            // Create a text object for the `*` character.
                            sf::Text characterText("@", font, 26); // Increase the font size to 36 (adjust as needed)

                            // Calculate the position for the text object.
                            float characterX = posX + cellSize / 2 - characterText.getLocalBounds().width / 2;
                            float characterY = posY + cellSize / 2 - characterText.getLocalBounds().height / 2;

                            // Set the position of the text object.
                            characterText.setPosition(characterX, characterY);

                            // Set the color of the text object to green.
                            characterText.setFillColor(sf::Color::Black);
                            window.draw(cellRect);
                            window.draw(characterText);
                        }
                    }

                }

                // Display everything
                window.display();
            }
            void updateGrid(const WorldGrid &grid,
                            const type_options_t &type_options,
                            const item_set_t &item_set,
                            const agent_set_t &agent_set)
            {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed)
                        window.close();
                }

                    DrawGrid(grid, type_options, item_set, agent_set);
            }
            bool Initialize() override {
                return true;
            }

            size_t SelectAction(const WorldGrid &grid,
                                const type_options_t &type_options,
                                const item_set_t &item_set,
                                const agent_set_t &agent_set) override {

                while (window.isOpen()) {
                    sf::Event event;
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                            window.close();
                        } else if (event.type == sf::Event::KeyPressed) {
                            // Handle keypress events here
                            size_t action_id = 0;
                            switch (event.key.code) {
                                case sf::Keyboard::W:
                                    action_id = GetActionID("up");
                                    break;
                                case sf::Keyboard::A:
                                    action_id = GetActionID("left");
                                    break;
                                case sf::Keyboard::S:
                                    action_id = GetActionID("down");
                                    break;
                                case sf::Keyboard::D:
                                    action_id = GetActionID("right");
                                    break;
                                case sf::Keyboard::Q:
                                    exit(0);
                                default:
                                    // The user pressed an unknown key.
                                    break;
                            }

                            // If we waited for input, but don't understand it, notify the user.
                            if (action_id == 0) {
                                std::cout << "Unknown key." << std::endl;
                            }

                            // Do the action!
                            return action_id;
                        }
                    }

                    updateGrid(grid, type_options, item_set, agent_set);
                }

                return 0;
            }


        };

    } // End of namespace 2D
} // End of namespace cse491
