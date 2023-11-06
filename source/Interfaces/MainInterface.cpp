/**
 * @author : Team - 3
 * @date: 10/03/2023
 * MainInterface class creates a window and displays the default maze grid
 */
#include <map>
#include "MainInterface.hpp"


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
        mMenu.initialize();

        ChooseTexture();
    }

    /**
     * @brief Creates a vector representation of the maze grid.
     *
     * @param grid         The WorldGrid representing the maze.
     * @param type_options The type options for symbols.
     * @param item_map     The map of ids to items in the maze.
     * @param agent_map    The map of ids to agents in the maze.
     *
     * @return A vector of strings representing the maze grid.
     */
    std::vector<std::string> MainInterface::CreateVectorMaze(

      const WorldGrid &grid, const type_options_t &type_options,
      const item_map_t &item_map, const agent_map_t &agent_map)

    {
        std::vector<std::string> symbol_grid(grid.GetHeight());
        // Load the world into the symbol_grid;
        for (size_t y = 0; y < grid.GetHeight(); ++y) {
            symbol_grid[y].resize(grid.GetWidth());
            for (size_t x = 0; x < grid.GetWidth(); ++x) {
                symbol_grid[y][x] = type_options[grid.At(x, y)].symbol;
            }
        }

        // Add in the agents / items
        for (const auto & [id, item_ptr] : item_map) {

          GridPosition pos = item_ptr->GetPosition();
          symbol_grid[pos.CellY()][pos.CellX()] = '+';
        }

        for (const auto & [id, agent_ptr] : agent_map) {
            GridPosition pos = agent_ptr->GetPosition();
            char c = '*';
            if (agent_ptr->HasProperty("symbol")) {
                c = static_cast<char>(agent_ptr->GetProperty<char>("symbol"));
            }
            symbol_grid[pos.CellY()][pos.CellX()] = c;
        }
        return symbol_grid;
    }

    /**
     * @brief Calculates the size of each cell based on the window size and grid dimensions.
     *
     * @param grid The WorldGrid representing the maze.
     * @return sf::Vector2f The size of each cell as a 2D vector.
     */
    sf::Vector2f MainInterface::CalculateCellSize(const WorldGrid &grid) {
        float cellSizeWide = mWindow.getSize().x / grid.GetWidth();
        float cellSizeTall = mWindow.getSize().y / grid.GetHeight();
        float cellSize = std::min(cellSizeWide, cellSizeTall) ;
        return sf::Vector2f(cellSize, cellSize);
    }

    /**
    * @brief Draws the maze grid and entities on the SFML window.
    *
    * @param grid         The WorldGrid representing the maze.
    * @param type_options The type options for symbols.
    * @param item_map     The map of ids to items in the maze.
    * @param agent_map    The map of ids to agents in t
    */
    void MainInterface::DrawGrid(const WorldGrid &grid, const type_options_t &type_options, const item_map_t &item_map, const agent_map_t &agent_map)
    {
        mWindow.clear(sf::Color::White);

        std::vector<std::string> symbol_grid = CreateVectorMaze(grid, type_options, item_map, agent_map);

        sf::Vector2f cellSize = CalculateCellSize(grid);
        float drawSpaceWidth, drawSpaceHeight, drawCenterX, drawCenterY;
        CalculateDrawSpace(grid, cellSize.x, drawSpaceWidth, drawSpaceHeight, drawCenterX, drawCenterY);

        for (size_t iterY = 0; iterY < grid.GetHeight(); ++iterY) {
            for (size_t iterX = 0; iterX < grid.GetWidth(); ++iterX) {
                float cellPosX = drawCenterX + static_cast<float>(iterX) * cellSize.x;
                float cellPosY = drawCenterY + static_cast<float>(iterY) * cellSize.y;

                char symbol = symbol_grid[iterY][iterX];


                sf::RectangleShape cellRect(sf::Vector2f(cellSize.x, cellSize.y));
                sf::RectangleShape cell(sf::Vector2f(cellSize.x, cellSize.y));
                cellRect.setPosition(sf::Vector2f(cellPosX, cellPosY));

                cellRect.setPosition(sf::Vector2f(cellPosX, cellPosY));
                cell.setPosition(sf::Vector2f(cellPosX, cellPosY));

                bool isVerticalWall = (iterY > 0 && symbol_grid[iterY - 1][iterX] == '#') ||
                                      (iterY < grid.GetHeight() - 1 && symbol_grid[iterY + 1][iterX] == '#');
                SwitchCellSelect(cellRect, cell, symbol, isVerticalWall);

            }
        }
        // Display everything
        mMenu.drawto(mWindow);
        mWindow.display();
    }
    /**
     * @brief Calculates the total drawing space based on the grid dimensions and cell size and also the center position of the drawing space.
     *
     * @param grid The WorldGrid representing the maze.
     * @param cellSize The size of each cell in pixels.
     * @param drawSpaceWidth Reference to the variable to store the total width of the drawing space.
     * @param drawSpaceHeight Reference to the variable to store the total height of the drawing space.
     * @param drawCenterX Reference to the variable to store the x-coordinate of the center of the drawing space.
     * @param drawCenterY Reference to the variable to store the y-coordinate of the center of the drawing space.
     */
    void MainInterface::CalculateDrawSpace(const WorldGrid &grid, float cellSize, float &drawSpaceWidth,
                                           float &drawSpaceHeight, float &drawCenterX, float &drawCenterY)
    {
        drawSpaceWidth = static_cast<float>(grid.GetWidth()) * cellSize;
        drawSpaceHeight = static_cast<float>(grid.GetHeight()) * cellSize;
        drawCenterX = (mWindow.getSize().x - drawSpaceWidth) / 2.0f;
        drawCenterY = (mWindow.getSize().y - drawSpaceHeight) / 2.0f;
    }

    /**

    * @brief Handles user input for selecting actions.
    *
    * @param grid         The WorldGrid representing the maze.
    * @param type_options The type options for symbols.
    * @param item_map     The map of ids to items in the maze.
    * @param agent_map    The map of ids to agents in the maze.
    */
    size_t MainInterface::SelectAction(const WorldGrid &grid,
                                       const type_options_t &type_options,
                                       const item_map_t &item_map,
                                       const agent_map_t &agent_map) {
        while (mWindow.isOpen()) {
            sf::Event event;
            while (mWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    mWindow.close();
                    exit(0);

                } else if (event.type == sf::Event::KeyPressed) {
                    return HandleKeyEvent(event);

                } else if(event.type == sf::Event::Resized) {
                    HandleResize(event, grid);

                } else if(event.type == sf::Event::MouseMoved){
                    mMenu.HandleMouseMove(mWindow);

                } else if(event.type == sf::Event::MouseButtonPressed){
                    mMenu.HandleMouseButtonPressed(mWindow);

                }else if(event.type == sf::Event::MouseWheelScrolled)
                {
//                    HandleScroll(event);
                }
            }


            DrawGrid(grid, type_options, item_map, agent_map);

        }

        return 0;
    }

    /**
     * @brief Handles key events and returns the corresponding action ID.
     *
     * This function takes an SFML event as input and processes the key events.
     * It checks for specific key presses such as W, A, S, D, and Q, and returns the
     * action ID associated with the respective key. If the key is not recognized,
     * it notifies the user with a message.
     *
     * @param event The SFML event object containing the key event information.
     * @return size_t The action ID corresponding to the key event.
     */
    size_t MainInterface::HandleKeyEvent(const sf::Event& event) {
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
            case sf::Keyboard::Up:
                action_id = GetActionID("up");
                break;
            case sf::Keyboard::Left:
                action_id = GetActionID("left");
                break;
            case sf::Keyboard::Down:
                action_id = GetActionID("down");
                break;
            case sf::Keyboard::Right:
                action_id = GetActionID("right");
                break;

            case sf::Keyboard::Q:
                exit(0);
            case sf::Keyboard::Escape:
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
    }

    /**
     * @brief Handles the window resize event
     *
     * Restricts window resizing if below a minimum size.
     * Matches the window's view to the new size of the window.
     *
     * @param event The SFML event object containing the resize event information.
     * @param grid The WorldGrid containing information on the world structure.
     */
    void MainInterface::HandleResize(const sf::Event& event, const WorldGrid &grid)
    {
        // Check size limits of window
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
    /**
     * @brief Draw the wall texture based on the provided parameters.
     *
     * @param cellRect The rectangle shape of the cell.
     * @param wallTexture The texture for the wall.
     * @param isVerticalWall for vertical wall
    */
    void MainInterface::DrawWall(sf::RectangleShape& cellRect, sf::Texture& wallTexture, bool isVerticalWall) {
        // TODO below if the wall has to be placed vertical
        if(isVerticalWall){}
//        if (isVerticalWall) {
//            sf::Transform transform;
//            transform.rotate(270.0f, sf::Vector2f(cellPosX + cellSize / 2.0f, cellPosY + cellSize / 2.0f));
//            transform.rotate(270.0f,sf::Vector2f(cellPosX + cellSize / 2.0f, cellPosY + cellSize / 2.0f));
//            mWindow.draw(cellRect, transform);
//            mWindow.draw(cellRect);
//        } else {
//            mWindow.draw(cellRect);
//        }
        cellRect.setTexture(&mTextureHolder.GetTexture("wallTexture"));
        mWindow.draw(cellRect);
    }

    /**
     * @brief Draw the cell with black color.
     *
     * @param cellRect The rectangle shape of the cell.
     */
    void MainInterface::DrawEmptyCell(sf::RectangleShape& cellRect) {
        cellRect.setFillColor(sf::Color::Black);
        mWindow.draw(cellRect);
    }
    
    /**
     * @brief Draw the cell with a bright pink that is hard to miss
     *
     * @param cellRect The rectangle shape of the cell.
     */
    void MainInterface::DrawDefaultCell(sf::RectangleShape& cellRect) {
        cellRect.setFillColor(sf::Color::Magenta);
        mWindow.draw(cellRect);
    }

    /**
     * @brief Draw the cell with Agent texture and specified color.
     * @param cellRect The rectangle shape of the cell.
     * @param cell The rectangle shape for the cell.
     * @param agent The agent texture.
     * @param color The color to be set for the cell.
     */

    void MainInterface::DrawAgentCell(sf::RectangleShape& cellRect, sf::RectangleShape& cell, sf::Texture& agent) {
        cellRect.setTexture(&agent);

        cell.setFillColor(sf::Color::Black);
        mWindow.draw(cell);
        mWindow.draw(cellRect);
    }
    /*
     * This function chooses the world to load the texture for it's images
     * and sets the current texture map for drawing
     */
    void MainInterface::ChooseTexture()
    {
        if(GetName() == "Interface1")
        {
            mTexturesDefault = mTextureHolder.MazeTexture();
            mTexturesCurrent = mTexturesDefault;
        }
        else if(GetName() == "Interface")
        {
            mTexturesSecondWorld = mTextureHolder.SecondWorldTexture();
            mTexturesCurrent = mTexturesSecondWorld;
        }
        else if(GetName() == "Interface3")
        {
            mTexturesManualWorld =mTextureHolder. ManualWorldTexture();
            mTexturesCurrent = mTexturesManualWorld;
        }
        else if(GetName() == "Interface2")
        {
            mTexturesGenerativeWorld =mTextureHolder. GenerativeWorldTexture();
            mTexturesCurrent = mTexturesGenerativeWorld;
        }

    }
    /**
     * This is a helper function for DrawGrid. jsut using the switch statement to draw the grids
     * @param cellRect - cell for texture
     * @param cell  - cell for the solid
     * @param symbol  - symbol of the cell
     * @param isVerticalWall  - to the wall
     */
    void MainInterface::SwitchCellSelect(sf::RectangleShape& cellRect,sf::RectangleShape& cell, char symbol, bool isVerticalWall)
    {
        switch (symbol) {
            case ' ':
                DrawEmptyCell(cellRect);
                break;
            case '#':
                DrawWall(cellRect, mTexturesCurrent[symbol], isVerticalWall);
                break;
            default:
                DrawAgentCell(cellRect, cell, mTexturesCurrent[symbol]);
                break;
        }
    }

}


