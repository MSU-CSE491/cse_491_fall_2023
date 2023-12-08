/**
 * @file MainInterface.hpp
 * @author :Gaya Kanagaraj, Vincenzo Felici, Mui Pham
 * @date: 10/03/2023
 * @brief MainInterface class manages the game's user interface, including the menu, textbox,
 *        message box, inventory, and texture holder. It serves as the main class responsible
 *        for creating and managing the game window, drawing the grid, handling player movements,
 *        and displaying menu and inventory details.
 */

#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include "Button.hpp"
#include "Menu.hpp"
#include <memory>

#include "../core/Data.hpp"
#include "../core/InterfaceBase.hpp"
#include "TextureHolder.hpp"
#include "TextBox.hpp"
#include "MessageBoard.hpp"


namespace i_2D {

    using namespace cse491;


    /**
    * @class MainInterface
    *
    * @brief Represents the main interface for a 2D maze game.
    *
    * This class inherits from `InterfaceBase` and provides functionality
    * for creating and displaying a 2D maze game world, handling user input,
    * and updating the graphical representation of the game.
    */
    class MainInterface : public virtual InterfaceBase {

    private:
        sf::RenderWindow mWindow; ///< render window
        float const MIN_SIZE_CELL = 16; ///< Pixels

        // Menu and message vars
        Menu mMenu; ///< for menu class
        sf::Font mFont; ///< one font for all objects using font
        std::unique_ptr<TextBox> mTextBox; ///< for chatting and possible event handling by text
        std::unique_ptr<MessageBoard> mMessageBoard; ///< message box object
        std::vector<std::string> mAgentInventory;

        // Texture vars
        TextureHolder mTextureHolder; ///< for the texture holder
        std::map<char, sf::Texture> mTexturesDefault; ///< for the texture holder default grid
        std::map<char, sf::Texture> mTexturesSecondWorld; ///< for the texture holder SecondWorld grid
        std::map<char, sf::Texture> mTexturesManualWorld; ///< for the texture holder Manualworld grid
        std::map<char, sf::Texture> mTexturesGenerativeWorld; ///< for the texture holder GenerativeWorld grid
        std::map<char, sf::Texture> mTexturesCurrent; ///< for the texture holder current world grid

        // Render range vars
        sf::Vector2i mPlayerPosition = sf::Vector2i(0, 0); ///< xy world grid location of the player
        bool mGridSizeLarge = false; ///< flag for the largegrid
        int const ROW = 9; ///< row to enlarge te grid
        int const COL = 23; ///< column to enlarge the grid

        int mGridWidth = 0; ///< for the gridwidth
        int mGridHeight = 0; ///< for the gridheight

        double mInputWaitTime = 0.5f;  ///< for the waittime of the player

        size_t HandleKeyEvent(const sf::Event &event);

        void CalculateDrawSpace(const WorldGrid &grid, float cellSize, float &drawSpaceWidth, float &drawSpaceHeight,
                                float &drawCenterX, float &drawCenterY);

        sf::Vector2f CalculateCellSize(const WorldGrid &grid);

        void HandleResize(const sf::Event &event, const WorldGrid &grid);

        void ChooseTexture();


        std::vector<std::string> LargeDisplayGrid(const std::vector<std::string> &symbol_grid);

        void MouseClickEvent(const sf::Event &event,const size_t entity_id, const item_map_t &item_map);

        void DrawAgentCell(sf::RenderTexture &renderTexture, sf::RectangleShape &cellRect, sf::RectangleShape &cell,
                           sf::Texture &agent);

        void
        SwitchCellSelect(sf::RenderTexture &renderTexture, sf::RectangleShape &cellRect, sf::RectangleShape &cell,
                         char symbol);

        void DrawWall(sf::RenderTexture &renderTexture, sf::RectangleShape &cellRect, sf::Texture &wallTexture);

        void DrawTimer();

        void DrawHealthInfo();

    public:
        MainInterface(size_t id, const std::string &name);

        /**
         * @brief Destructor for the `MainInterface` class.
         */
        ~MainInterface() = default;


        std::vector<std::string> CreateVectorMaze(
                const WorldGrid &grid,
                const type_options_t &type_options,
                const item_map_t &item_map,
                const agent_map_t &agent_map);

        void DrawGrid(const WorldGrid &grid, const type_options_t &type_options,
                      const item_map_t &item_map, const agent_map_t &agent_map);
        
        void setMInputWaitTime(double mInputWaitTime);
        void SetLargeGrid(bool b){ mGridSizeLarge = b; } 

        /**
         * @brief Initializes the main interface.
         *
         * @return True if initialization is successful; otherwise, false.
         */
        bool Initialize() override {
            return true;
        }

        size_t SelectAction(const WorldGrid &grid,
                            const type_options_t &type_options,
                            const item_map_t &item_map,
                            const agent_map_t &agent_map) override;


        /**
         * @brief notifies the world if the player have any progress message
         * @param message that notifies the world
         */
        void Notify(const std::string &message,
                    const std::string & /*msg_type*/= "none") override {
            std::cout << message << std::endl;
            mMessageBoard->Send(message);
        }
    };

} // End of namespace 2D

