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

#include "../core/Data.hpp"
#include "../core/InterfaceBase.hpp"
#include "TextureHolder.hpp"


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
    class MainInterface : public InterfaceBase {

    protected:

        sf::RenderWindow mWindow; ///< render window
        float const MIN_SIZE_CELL = 16; ///< Pixels

        Menu mMenu; ///< for menu class
        TextureHolder mTextureHolder; ///< for the texture holder
        std::map<char, sf::Texture> mTexturesDefault;
        std::map<char, sf::Texture> mTexturesSecondWorld;
        std::map<char, sf::Texture> mTexturesManualWorld;
        std::map<char, sf::Texture> mTexturesGenerativeWorld;
        std::map<char, sf::Texture> mTexturesCurrent;

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

        void DrawWall(sf::RectangleShape &cellRect, sf::Texture &wallTexture, bool isVerticalWall);

        void DrawEmptyCell(sf::RectangleShape &cellRect);

        void DrawDefaultCell(sf::RectangleShape &cellRect);

        void DrawAgentCell(sf::RectangleShape &cellRect, sf::RectangleShape &cell, sf::Texture &agent);


        void CalculateDrawSpace(const WorldGrid &grid, float cellSize, float &drawSpaceWidth, float &drawSpaceHeight,
                                float &drawCenterX, float &drawCenterY);

        sf::Vector2f CalculateCellSize(const WorldGrid &grid);

        void HandleResize(const sf::Event &event, const WorldGrid &grid);

        void ChooseTexture();
        void SwitchCellSelect(sf::RectangleShape& cellRect,sf::RectangleShape& cell, char symbol, bool isVerticalWall);

    };

} // End of namespace 2D

