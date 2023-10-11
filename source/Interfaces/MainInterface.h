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

#include "../core/Data.hpp"
#include "../core/InterfaceBase.hpp"

#include "Menu.h"

using namespace cse491;

namespace i_2D {
    /**
    * @class MainInterface
    *
    * @brief Represents the main interface for a 2D maze game.
    *
    * This class inherits from `InterfaceBase` and provides functionality
    * for creating and displaying a 2D maze game world, handling user input,
    * and updating the graphical representation of the game.
    */
    class MainInterface : public InterfaceBase { // Rename to mainInterface

    protected:
        bool wait_for_input = true;
        sf::RenderWindow mWindow;
        sf::Clock clock;
        sf::Vector2i characterPosition; // Add character position variable
        WorldGrid wGrid;
        float const MIN_SIZE_CELL = 16; // Pixels

    public:

        MainInterface(size_t id, const std::string &name) ;

        /**
         * @brief Destructor for the `MainInterface` class.
         */
        ~MainInterface() = default;


        std::vector<std::string> CreateVectorMaze(const WorldGrid &grid, const type_options_t &type_options,
                                                  const item_set_t &item_set, const agent_set_t &agent_set) ;

        void DrawGrid(const WorldGrid &grid, const type_options_t &type_options,
                      const item_set_t &item_set, const agent_set_t &agent_set);


        void updateGrid(const WorldGrid &grid,
                        const type_options_t &type_options,
                        const item_set_t &item_set,
                        const agent_set_t &agent_set);
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
                            const item_set_t &item_set,
                            const agent_set_t &agent_set) override;
    };

} // End of namespace 2D

