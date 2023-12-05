/**
 * @author : Team - 3
 * @date: 10/03/2023
 * MainInterface class creates a window and displays the default maze grid
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
#include "MessageBoard.h"


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

    protected:

        sf::RenderWindow mWindow; ///< render window
        float const MIN_SIZE_CELL = 16; ///< Pixels

        // Menu and message vars
        Menu mMenu; ///< for menu class
        sf::Font mFont; ///< one font for all objects using font
        std::unique_ptr<TextBox> mTextBox; /// for chatting and possible event handling by text
        std::unique_ptr<MessageBoard> mMessageBoard;
//        std::unique_ptr<Button> mTestButton;

        // Texture vars
        TextureHolder mTextureHolder; ///< for the texture holder
        std::map<char, sf::Texture> mTexturesDefault;
        std::map<char, sf::Texture> mTexturesSecondWorld;
        std::map<char, sf::Texture> mTexturesManualWorld;
        std::map<char, sf::Texture> mTexturesGenerativeWorld;
        std::map<char, sf::Texture> mTexturesCurrent;

        // Render range vars
        sf::Vector2i mPlayerPosition = sf::Vector2i(0,0); ///< xy world grid location of the player
        bool mGridSizeLarge = false;
        int const ROW = 9;
        int const COL = 23;

        int mGridWidth  = 0;
        int mGridHeight = 0;

        bool mPlayerHasMoved = false;

    public:

        MainInterface(size_t id, const std::string &name) ;

        /**
         * @brief Destructor for the `MainInterface` class.
         */
        ~MainInterface() = default;


        std::vector<std::string> CreateVectorMaze(
                const WorldGrid &grid,
                const type_options_t &type_options,
                const item_map_t &item_map,
                const agent_map_t &agent_map) ;

        void DrawGrid(const WorldGrid &grid, const type_options_t &type_options,
                      const item_map_t &item_map, const agent_map_t &agent_map);

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

        size_t HandleKeyEvent(const sf::Event &event);

        void CalculateDrawSpace(const WorldGrid &grid, float cellSize, float &drawSpaceWidth, float &drawSpaceHeight,
                                float &drawCenterX, float &drawCenterY);

        sf::Vector2f CalculateCellSize(const WorldGrid &grid);

        void HandleResize(const sf::Event &event, const WorldGrid &grid);

        void ChooseTexture();

        void Notify(const std::string & message,
                    const std::string & /*msg_type*/="none") override
        {
            std::cout << message << std::endl;
            mMessageBoard->Send(message);
        }
        std::vector<std::string> LargeDisplayGrid(const std::vector<std::string> &symbol_grid);

        void MouseClickEvent(const sf::Event &event);

        void DrawAgentCell(sf::RenderTexture &renderTexture, sf::RectangleShape &cellRect, sf::RectangleShape &cell,
                           sf::Texture &agent);

        void
        SwitchCellSelect(sf::RenderTexture &renderTexture, sf::RectangleShape &cellRect, sf::RectangleShape &cell,
                         char symbol);

        void DrawWall(sf::RenderTexture &renderTexture, sf::RectangleShape &cellRect, sf::Texture &wallTexture);

        void DrawTimer();
    };

} // End of namespace 2D

