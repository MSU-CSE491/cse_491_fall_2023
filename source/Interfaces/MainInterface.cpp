/**
 * @file MainInterface.cpp
 * @authors Gaya Kanagaraj, Vincenzo Felici, Mui Pham
 * @date 10/03/2023
 * @brief MainInterface class manages the game's user interface, including the menu, textbox,
 *        message box, inventory, and texture holder. It serves as the main class responsible
 *        for creating and managing the game window, drawing the grid, handling player movements,
 *        and displaying menu and inventory details.
 *
 */

#include <map>
#include "MainInterface.hpp"

namespace i_2D {

    sf::Clock timer; // To drawTimer function
    float elapsedTime = 0.0f; // Sets elapsed time  = 0.0

    /**
     * @brief Constructs a `MainInterface` object.
     *
     * @param id   The identifier for this interface.
     * @param name The name of this interface.
     */
    MainInterface::MainInterface(size_t id, const std::string &name) : InterfaceBase(id, name),
                                                                       mWindow(sf::VideoMode({1000, 800}),
                                                                               "Maze Window") {
        ///Font for all objects
        if (!mFont.loadFromFile("../../assets/font/ArialNarrow7.ttf")) {
            std::cout << "Error loading font file: ../assets/font/ArialNarrow7.ttf" << std::endl;
        }
        mMessageBoard = std::make_unique<MessageBoard>(mFont);
        mTextBox = std::make_unique<TextBox>(mFont);
        auto a = mWindow.getSize().x;
        auto b = mWindow.getSize().y;
        mMenu.initialize(mFont, sf::Vector2f{static_cast<float>(a), static_cast<float>(b)});
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
            const item_map_t &item_map, const agent_map_t &agent_map) {

        std::vector<std::string> symbol_grid(grid.GetHeight());

        mGridHeight = grid.GetHeight();
        mGridWidth = grid.GetWidth();

        // Load the world into the symbol_grid;
        for (size_t y = 0; y < grid.GetHeight(); ++y) {
            symbol_grid[y].resize(grid.GetWidth());
            for (size_t x = 0; x < grid.GetWidth(); ++x) {
                symbol_grid[y][x] = type_options[grid.At(x, y)].symbol;
            }
        }

        // Add in the entities
        for (const auto &[id, item_ptr]: item_map) {
            GridPosition pos = item_ptr->GetPosition();
            char c = '+';
            if (item_ptr->HasProperty("symbol")) {
                c = item_ptr->GetProperty<char>("symbol");
            }
            if(pos.CellX() >= 0 && pos.CellY() >= 0 && 
                pos.CellX() < grid.GetWidth() && pos.CellY() < grid.GetHeight() && 
                !item_ptr->IsOwned()){
              symbol_grid[pos.CellY()][pos.CellX()] = c;
            }
        }

        // Add in the agents
        for (const auto &[id, agent_ptr]: agent_map) {
            GridPosition pos = agent_ptr->GetPosition();
            char c = '*';
            if (agent_ptr->HasProperty("symbol")) {
                c = agent_ptr->GetProperty<char>("symbol");
            }
            if (!agent_ptr->HasProperty("deleted")){
                symbol_grid[pos.CellY()][pos.CellX()] = c;
            }
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

        float cellSizeWide, cellSizeTall;
        if (mGridSizeLarge) {
            cellSizeWide = mWindow.getSize().x / COL;
            cellSizeTall = mWindow.getSize().y / ROW;
        } else {
            cellSizeWide = mWindow.getSize().x / grid.GetWidth();
            cellSizeTall = mWindow.getSize().y / grid.GetHeight();
        }

        float cellSize = std::min(cellSizeWide, cellSizeTall);
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
    void MainInterface::DrawGrid(const WorldGrid &grid, const type_options_t &type_options, const item_map_t &item_map,
                                 const agent_map_t &agent_map) {
        // Clear old drawing
        mWindow.clear(sf::Color::White);

        // Check player's position
        mPlayerPosition = sf::Vector2i(this->position.GetX(), this->position.GetY());

        // Create grid of symbols representing the world
        std::vector<std::string> symbol_grid;
        std::vector<std::string> default_grid = CreateVectorMaze(grid, type_options, item_map, agent_map);

        // Determine cell size
        sf::Vector2f cellSize = CalculateCellSize(grid);
        float drawSpaceWidth, drawSpaceHeight, drawCenterX, drawCenterY;
        CalculateDrawSpace(grid, cellSize.x, drawSpaceWidth,
                           drawSpaceHeight, drawCenterX, drawCenterY);

        if (mGridSizeLarge) {
            symbol_grid = LargeDisplayGrid(default_grid);
        } else {
            symbol_grid = default_grid;
        }
        //CheckLargerGrid();
        // Create a render texture to draw the grid
        sf::RenderTexture renderTexture;
        renderTexture.create({static_cast<unsigned int>(drawSpaceWidth), static_cast<unsigned int>(drawSpaceHeight)});
        renderTexture.clear(sf::Color::White);

        for (size_t iterY = 0; iterY < symbol_grid.size(); ++iterY) {
            for (size_t iterX = 0; iterX < symbol_grid[0].size(); ++iterX) {
                float cellPosX = static_cast<float>(iterX) * cellSize.x;
                float cellPosY = static_cast<float>(iterY) * cellSize.y;
                char symbol = symbol_grid[iterY][iterX];

                sf::RectangleShape cellRect(sf::Vector2f(cellSize.x, cellSize.y));
                cellRect.setPosition(sf::Vector2f(cellPosX, cellPosY));

                sf::RectangleShape cell(sf::Vector2f(cellSize.x, cellSize.y));
                cell.setPosition(sf::Vector2f(cellPosX, cellPosY));

                SwitchCellSelect(renderTexture, cellRect, cell, symbol);
            }
        }
        renderTexture.display();
        DrawTimer();
        DrawHealthInfo();

        // Draw the texture to the window
        sf::Sprite sprite(renderTexture.getTexture());
        sprite.setPosition({drawCenterX, drawCenterY});
        mWindow.draw(sprite);

        // Display everything
        mTextBox->DrawTo(mWindow);
        mMessageBoard->DrawTo(mWindow);
        mMenu.drawto(mWindow);
        mWindow.display();
    }

    /**
    * @brief this function draws timer and checks the elapsed time
    * if the timer exceed above 0.5 seconds, it returns 0 to the netwrok interface
    * and restart the timer every move player makes
    */
    void MainInterface::DrawTimer() {
        // Get elapsed time in seconds
        elapsedTime = timer.getElapsedTime().asSeconds();

        // Set up font and location
        sf::Text timerText(mFont);
        timerText.setCharacterSize(24);
        timerText.setPosition({750.0f, 75.0f}); // Adjust position as needed

        // Format the elapsed time with 2 decimal points
        std::ostringstream stream;
        stream << "Time: " << std::fixed << std::setprecision(2) << elapsedTime << " S";
        std::string formattedTime = stream.str();

        // Create text for current value
        timerText.setString(formattedTime);
        timerText.setFillColor(sf::Color::Blue);
        mWindow.draw(timerText);
    }

    void MainInterface::DrawHealthInfo() {
        if(!HasProperty("Health") || !PropertyIsType<int>("Health"))
        {
            return;
        }

        int health = GetProperty<int>("Health");

        // Set text properties and draw
        sf::Text healthText(mFont);
        healthText.setCharacterSize(24);
        healthText.setPosition({20.0f, 75.0f});
        healthText.setFillColor(sf::Color::Green);
        healthText.setString("Hp: " + std::to_string(health));
        mWindow.draw(healthText);
    }

    /**
     * @brief Creates a 9x23 window of the symbol grid centered around the player's position.
     *
     * @param symbol_grid   The original symbol grid.
     * @return std::vector<std::string> A new symbol grid representing the 9x23 window.
     */
    std::vector<std::string> MainInterface::LargeDisplayGrid(const std::vector<std::string> &symbol_grid) {
        // Determine the top-left corner of the 9x23 window
        int topLeftX = std::max(0,
                                std::min(mPlayerPosition.x - COL / 2, static_cast<int>(symbol_grid[0].size()) - COL));
        int topLeftY = std::max(0, std::min(mPlayerPosition.y - ROW / 2, static_cast<int>(symbol_grid.size()) - ROW));

        // Create a new symbol grid for the 9x23 display window
        std::vector<std::string> display_grid;
        for (size_t iterY = 0; iterY < ROW; ++iterY) {
            std::string row;
            for (size_t iterX = 0; iterX < COL; ++iterX) {
                int posX = topLeftX + iterX;
                int posY = topLeftY + iterY;

                // Copy the symbol from symbol_grid to display_grid
                row.push_back(symbol_grid[posY][posX]);
            }
            display_grid.push_back(row);
        }

        return display_grid;
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
                                           float &drawSpaceHeight, float &drawCenterX, float &drawCenterY) {
        if (mGridSizeLarge) {
            drawSpaceWidth = static_cast<float>(COL) * cellSize;
            drawSpaceHeight = static_cast<float>(ROW) * cellSize;
        } else {
            drawSpaceWidth = static_cast<float>(grid.GetWidth()) * cellSize;
            drawSpaceHeight = static_cast<float>(grid.GetHeight()) * cellSize;
        }

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
        // Initialize action_id and timer
        size_t action_id = 0;
        timer.restart();

        // While the timer is going
        while (mWindow.isOpen() && timer.getElapsedTime().asSeconds() < mInputWaitTime) {
            sf::Event event;

            // Check through all events generated in this frame
            while (mWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    mWindow.close();
                    exitCleanup();

                } else if (event.type == sf::Event::TextEntered) {
                    if (mTextBox->IsSelected()) {
                        mTextBox->TypedOn(event);
                    }

                } else if (event.type == sf::Event::KeyPressed) {
                    action_id = HandleKeyEvent(event);

                } else if (event.type == sf::Event::Resized) {
                    HandleResize(event, grid);

                } else if (event.type == sf::Event::MouseMoved) {
                    auto c = mMenu.HandleMouseMove(mWindow);
                    if(c!="null"){
                        auto texture = mTextureHolder.GetTexture(c);
                        mMenu.SetInventoryItemDisplay(texture);
                    }
                } else if (event.type == sf::Event::MouseButtonPressed) {
                    MouseClickEvent(event, GetID(), item_map);

                }
            }

            // Check if a valid action was taken and return that if so
            if (action_id != 0) {
                return action_id;
            }

            // Otherwise update the screen drawing again...
            DrawGrid(grid, type_options, item_map, agent_map);

        }
        // The timer has ended or the window has been closed
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
    size_t MainInterface::HandleKeyEvent(const sf::Event &event) {
        size_t action_id = 0;
        if (mTextBox->IsSelected()) {
            // TextBox is selected, handle specific cases
            switch (event.key.code) {
                case sf::Keyboard::Enter:
                    mMessageBoard->Send(mTextBox->GetText());
                    mTextBox->SetString("");
                    mTextBox->SetSelected(false);
                    break;
                case sf::Keyboard::Escape:
                    mTextBox->SetString("");
                    mTextBox->SetSelected(false);
                    break;
                default:
                    break;
            }
        } else {
            // TextBox is not selected, handle movement keys
            switch (event.key.code) {
              case sf::Keyboard::W:
                  if (mTextBox->IsSelected())break;
                  action_id = GetActionID("up");
                  break;
              case sf::Keyboard::A:
                  if (mTextBox->IsSelected())break;
                  action_id = GetActionID("left");
                  break;
              case sf::Keyboard::S:
                  if (mTextBox->IsSelected())break;
                  action_id = GetActionID("down");
                  break;
              case sf::Keyboard::D:
                  if (mTextBox->IsSelected())break;
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
              case sf::Keyboard::H:
                  action_id = GetActionID("heal");
                  break;
              case sf::Keyboard::T:
                  /// TODO: make this more robust (AJF)
                  if (GetName() == "Interface") action_id = GetActionID("drop");
                  else if (GetName() == "Interface3") action_id = GetActionID("stats");
                  break;
              case sf::Keyboard::C:
                  action_id = GetActionID("use_axe");
                  break;
              case sf::Keyboard::V:
                  action_id = GetActionID("use_boat");
                  break;
              case sf::Keyboard::F:
                  action_id = GetActionID("attack");
                  break;
              case sf::Keyboard::G:
                  action_id = GetActionID("special");
                  break;
              case sf::Keyboard::B:
                  action_id = GetActionID("buff");
                  break;
              case sf::Keyboard::R:
                  action_id = GetActionID("run");
                  break;
              case sf::Keyboard::Y:
                  action_id = GetActionID("help");
                  break;
              default:
                  break; // The user pressed an unknown key.
            }
        }
        // If we waited for input, but don't understand it, notify the user.
        if (action_id == 0 && !mTextBox->IsSelected()) {
            std::cout << "Unknown key." << std::endl;
        }
        // No action performed
        return action_id;
    }


    /**
     * @brief Handles the window resize event
     * Restricts window resizing if below a minimum size.
     * Matches the window's view to the new size of the window.
     *
     * @param event The SFML event object containing the resize event information.
     * @param grid The WorldGrid containing information on the world structure.
     */
    void MainInterface::HandleResize(const sf::Event &event, const WorldGrid &grid) {
        // Check size limits of window
        float widthWindow = event.size.width;
        float heightWindow = event.size.height;
        float widthMin, heightMin;
        if (mGridSizeLarge) {
            widthMin = COL * MIN_SIZE_CELL;
            heightMin = ROW * MIN_SIZE_CELL;
        } else {
            widthMin = grid.GetWidth() * MIN_SIZE_CELL;
            heightMin = grid.GetHeight() * MIN_SIZE_CELL;
        }

        widthWindow = std::max(widthWindow, widthMin);
        heightWindow = std::max(heightWindow, heightMin);

        mMenu.SetWorldSize(sf::Vector2f(widthWindow, heightWindow));
        if (mMenu.IsInventoryOpen()) {

            mMenu.DeconstructInventory();
            mMenu.ConstructInventory(mAgentInventory);
        }
        // Restrict window size if necessary
        if (widthWindow <= widthMin || heightWindow <= heightMin) {
            mWindow.setSize(sf::Vector2u(widthWindow, heightWindow));
        }

        // Resize the view to match window size to prevent deformation
        sf::FloatRect viewArea(sf::Vector2f(0, 0), sf::Vector2f(widthWindow, heightWindow));
        mWindow.setView(sf::View(viewArea));

        // Update TextBox position for resizing
        if (mTextBox) {
            // Define the percentage values for the position
            float xPosPercentage = 0.01f; // 5% of the window width
            float yPosPercentage = 0.9f;  // 80% of the window height

            // Calculate the actual position based on percentages
            float xPos = (widthWindow * xPosPercentage);
            float yPos = (heightWindow * yPosPercentage);

            // Set the position of your textbox
            mTextBox->SetPosition({xPos, yPos});

        }
    }

    /*
     * This function chooses the world to load the texture for it's images
     * and sets the current texture map for drawing
     */
    void MainInterface::ChooseTexture() {
        if (GetName() == "Interface1") {
            mTexturesDefault = mTextureHolder.MazeTexture();
            mTexturesCurrent = mTexturesDefault;
        } else if (GetName() == "Interface") {
            mTexturesSecondWorld = mTextureHolder.SecondWorldTexture();
            mTexturesCurrent = mTexturesSecondWorld;
        } else if (GetName() == "Interface3") {
            mTexturesManualWorld = mTextureHolder.ManualWorldTexture();
            mTexturesCurrent = mTexturesManualWorld;
        } else if (GetName() == "Interface2") {
            mTexturesGenerativeWorld = mTextureHolder.GenerativeWorldTexture();
            mTexturesCurrent = mTexturesGenerativeWorld;
        }

    }

    /**
     * this function handles mouseclick event
     * @param event for mouse click
     */
    void MainInterface::MouseClickEvent(const sf::Event &event, const size_t entity_id, const item_map_t &item_map) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            // Check if the mouse click is inside the TextBox bounds
            if (mTextBox->Contains(mousePos)) {
                // Toggle the selected state of the TextBox
                mTextBox->SetSelected(!mTextBox->IsSelected());
            } else {
                // If the click is outside the TextBox, deselect it
                mTextBox->SetSelected(false);
            }

            // Check if the mouse is over specific menu items
            if (mMenu.GetMenu()[4]->IsMouseOver(mWindow) ) {
                SetLargeGrid(true);
            } else if (mMenu.GetMenu()[3]->IsMouseOver(mWindow)) {
                SetLargeGrid(false);
            } else if (mMenu.GetMenu()[2]->IsMouseOver(mWindow)) {
                exitCleanup();
            } else {
                // Handle mouse button press for the general menu
                mAgentInventory.clear();
                for(const auto &[x,y]:item_map){
                    if(GetID() == y->GetOwnerID()){
                        mAgentInventory.push_back(y->GetName());
                    }
                }
                mMenu.HandleMouseButtonPressed(mWindow, mAgentInventory);
            }
        }
    }

    /**
    * This is a helper function for DrawGrid. jsut using the switch statement to draw the grids
    * @param cellRect - cell for texture
    * @param cell  - cell for the solid
    * @param symbol  - symbol of the cell
    * @param renderTexture  - Texture for teh whole grid
    */
    void MainInterface::SwitchCellSelect(sf::RenderTexture &renderTexture, sf::RectangleShape &cellRect,
                                         sf::RectangleShape &cell, char symbol) {
        switch (symbol) {
            case '#':
                DrawWall(renderTexture, cellRect, mTexturesCurrent[symbol]);
                break;
            default:
                DrawAgentCell(renderTexture, cellRect, cell, mTexturesCurrent[symbol]);
                break;
        }
    }

    /**
     * @brief Draw the cell with Agent texture and specified color.
     * @param cellRect The rectangle shape of the cell.
     * @param cell The rectangle shape for the cell.
     * @param agent The agent texture.
     * @param renderTexture The Texture for the whole grid.
     */
    void MainInterface::DrawAgentCell(sf::RenderTexture &renderTexture, sf::RectangleShape &cellRect,
                                      sf::RectangleShape &cell, sf::Texture &agent) {
        cellRect.setTexture(&agent);
        cell.setTexture(&mTexturesCurrent[' ']);
        if (&agent == &mTexturesCurrent['+'] and GetName() == "Interface") {
            cellRect.setFillColor(sf::Color::Green);
        }
        renderTexture.draw(cell);
        renderTexture.draw(cellRect);
    }

    /**
     * @brief Draw the wall texture based on the provided parameters.
     *
     * @param cellRect The rectangle shape of the cell.
     * @param wallTexture The texture for the wall.
     * @param renderTexture The Texture for the whole grid
    */
    void MainInterface::DrawWall(sf::RenderTexture &renderTexture,
                                 sf::RectangleShape &cellRect, sf::Texture &wallTexture) {

        cellRect.setTexture(&wallTexture);
        renderTexture.draw(cellRect);
    }
    /**
     * Sets the inputwait time for netwrok interface
     *
     * @param waitTime
     */
    void MainInterface::setMInputWaitTime(double waitTime) {
        MainInterface::mInputWaitTime = waitTime;
    }

    void MainInterface::CheckLargerGrid()
    {
        if (mGridWidth == mGridHeight)
        {
            mGridSizeLarge = true;
        }
    }


}
