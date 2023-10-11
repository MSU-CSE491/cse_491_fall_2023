//
// Created by Terra Byte on 10/9/23.
//

#include "MainInterface.h"

namespace i_2D {
    /**
     * @brief Constructs a `MainInterface` object.
     *
     * @param id   The identifier for this interface.
     * @param name The name of this interface.
     */
    MainInterface::MainInterface(size_t id, const std::string &name) : InterfaceBase(id, name),
                                                                       mWindow(sf::VideoMode({1000, 800}),
                                                                               "Maze Window") {
    }

    /**
     * @brief Creates a vector representation of the maze grid.
     *
     * @param grid         The WorldGrid representing the maze.
     * @param type_options The type options for symbols.
     * @param item_set     The set of items in the maze.
     * @param agent_set    The set of agents in the maze.
     *
     * @return A vector of strings representing the maze grid.
     */
    std::vector<std::string> MainInterface::CreateVectorMaze(const WorldGrid &grid, const type_options_t &type_options,
                                                             const item_set_t &item_set, const agent_set_t &agent_set) {
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
        return symbol_grid;
    }

    /**
    * @brief Draws the maze grid and entities on the SFML window.
    *
    * @param grid         The WorldGrid representing the maze.
    * @param type_options The type options for symbols.
    * @param item_set     The set of items in the maze.
    * @param agent_set    The set of agents in the maze.
    */
    void MainInterface::DrawGrid(const WorldGrid &grid, const type_options_t &type_options,
                                 const item_set_t &item_set, const agent_set_t &agent_set) {
        // Clear the window
        mWindow.clear(sf::Color::White);

        std::vector<std::string> symbol_grid = CreateVectorMaze(grid, type_options, item_set, agent_set);

        // Set the size of each cell in pixels (adjust this to fit your needs)
        // Fit cells to shorter window side
        float cellSizeWide = mWindow.getSize().x / grid.GetWidth();
        float cellSizeTall = mWindow.getSize().y / grid.GetHeight();
        float cellSize = std::min(cellSizeWide, cellSizeTall);

        // Calculate the total size of the draw space
        float drawSpaceWidth = static_cast<float>(grid.GetWidth()) * cellSize;
        float drawSpaceHeight = static_cast<float>(grid.GetHeight()) * cellSize;

        // Calculate the position to center the draw space in the mWindow
        float drawCenterX = (mWindow.getSize().x - drawSpaceWidth) / 2.0f;
        float drawCenterY = (mWindow.getSize().y - drawSpaceHeight) / 2.0f;

        // Reference texture files
        sf::Texture wallTexture;
        if (!wallTexture.loadFromFile("../assets/walls/wall.png")) {
            std::cerr << "Failed to load wall texture!" << std::endl;
        }
        sf::Texture troll;
        if (!troll.loadFromFile("../assets/agents/troll.png")) {
            std::cerr << "Failed to load wall texture!" << std::endl;
        }

        // Draw the cells based on the symbol_grid
        for (size_t iterY = 0; iterY < grid.GetHeight(); ++iterY) {
            for (size_t iterX = 0; iterX < grid.GetWidth(); ++iterX) {
                // Calculate the position for this cell
                float cellPosX = drawCenterX + static_cast<float>(iterX) * cellSize;
                float cellPosY = drawCenterY + static_cast<float>(iterY) * cellSize;

                // Check the symbol in the symbol_grid
                char symbol = symbol_grid[iterY][iterX];

                // Create a rectangle for each cell
                sf::RectangleShape cellRect(sf::Vector2f(cellSize, cellSize));
                sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
                cellRect.setPosition(sf::Vector2f(cellPosX, cellPosY));
                cell.setPosition(sf::Vector2f(cellPosX, cellPosY));

                bool isVerticalWall = (iterY > 0 && symbol_grid[iterY - 1][iterX] == '#') ||
                                      (iterY < grid.GetHeight() - 1 && symbol_grid[iterY + 1][iterX] == '#');
                switch (symbol) {
                    case '#':
                        cellRect.setTexture(&wallTexture);
                        if (isVerticalWall) {
//                                sf::Transform transform;
////                                transform.rotate(270.0f, sf::Vector2f(cellPosX + cellSize / 2.0f, cellPosY + cellSize / 2.0f));
//
//                                transform.rotate(270.0f,sf::Vector2f(cellPosX + cellSize / 2.0f, cellPosY + cellSize / 2.0f));
//                                mWindow.draw(cellRect, transform);
                            mWindow.draw(cellRect);

                        } else {
                            mWindow.draw(cellRect);
                        }
                        break;

                    case ' ':
                        cellRect.setFillColor(sf::Color::Black);
                        mWindow.draw(cellRect);
                        break;

                    case '*':
                        cellRect.setTexture(&troll);
                        cellRect.setFillColor(sf::Color::Cyan);
                        cell.setFillColor(sf::Color::Black);
                        mWindow.draw(cell);
                        mWindow.draw(cellRect);
                        break;

                    case '@':
                        cellRect.setTexture(&troll);
                        cellRect.setFillColor(sf::Color::Red);
                        cell.setFillColor(sf::Color::Black);
                        mWindow.draw(cell);
                        mWindow.draw(cellRect);
                        break;

                }
            }

        }
        // Display everything
        mWindow.display();
    }

    /**
    * @brief Updates the graphical representation of the maze grid.
    *
    * @param grid         The WorldGrid representing the maze.
    * @param type_options The type options for symbols.
    * @param item_set     The set of items in the maze.
    * @param agent_set    The set of agents in the maze.
    */
    void MainInterface::UpdateGrid(const WorldGrid &grid,
                                   const type_options_t &type_options,
                                   const item_set_t &item_set,
                                   const agent_set_t &agent_set) {
        DrawGrid(grid, type_options, item_set, agent_set);
    }

    /**
    * @brief Handles user input for selecting actions.
    *
    * @param grid         The WorldGrid representing the maze.
    * @param type_options The type options for symbols.
    * @param item_set     The set of items in the maze.
    * @param agent_set    The set of agents in the maze.
    */
    size_t MainInterface::SelectAction(const WorldGrid &grid,
                                       const type_options_t &type_options,
                                       const item_set_t &item_set,
                                       const agent_set_t &agent_set) {

        while (mWindow.isOpen()) {
            sf::Event event;
            while (mWindow.pollEvent(event)) {
                // TODO convert to nested switch case statements?
                if (event.type == sf::Event::Closed) {
                    mWindow.close();
                    exit(0);

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
                            break; // The user pressed an unknown key.
                    }
                    // If we waited for input, but don't understand it, notify the user.
                    if (action_id == 0) {
                        std::cout << "Unknown key." << std::endl;
                    }
                    // Do the action!
                    return action_id;

                } else if(event.type == sf::Event::Resized) {
                    // Check size limits of window
                    // TODO replace min calculations with a member var on first grid read?
                    //  Min cell size may be bad for large worlds if min window size > monitor size
                    float widthWindow = event.size.width;
                    float heightWindow = event.size.height;
                    float widthMin = grid.GetWidth() * MIN_SIZE_CELL;
                    float heightMin = grid.GetHeight() * MIN_SIZE_CELL;
                    widthWindow = std::max(widthWindow, widthMin);
                    heightWindow = std::max(heightWindow, heightMin);

                    // Restrict window size if necessary
                    if(widthWindow <= widthMin || heightWindow <= heightMin) {
                        mWindow.setSize(sf::Vector2u(widthWindow, heightWindow));
                    }

                    // Resize the view to match window size to prevent deformation
                    sf::FloatRect viewArea(sf::Vector2f(0, 0), sf::Vector2f(widthWindow, heightWindow));
                    mWindow.setView(sf::View(viewArea));
                }
            }

            UpdateGrid(grid, type_options, item_set, agent_set);
        }

        return 0;
    }


}