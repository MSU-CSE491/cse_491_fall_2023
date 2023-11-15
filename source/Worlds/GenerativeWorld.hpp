/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A World that consists only of walls and open cells.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <cassert>

#include "BiomeGenerator.hpp"
#include "../core/WorldBase.hpp"

namespace group6 {
    using namespace cse491;

    class GenerativeWorld : public WorldBase {
    protected:
        enum ActionType {
            REMAIN_STILL = 0, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT
        };

        size_t floor_id; ///< Easy access to floor CellType ID.
        size_t wall_id;  ///< Easy access to wall CellType ID.

        size_t spike_id;  ///< Easy access to spike CellType ID.
        size_t tar_id; ///< Easy access to tar CellTypeID
        size_t key_id; ///< Easy access to key CellTypeID
        size_t door_id; ///< Easy access to door CellTypeID

        size_t tree_id;
        size_t grass_id;
        size_t dirt_id;

        /// Provide the agent with movement actions.
        void ConfigAgent(AgentBase &agent) override {
            agent.AddAction("up", MOVE_UP);
            agent.AddAction("down", MOVE_DOWN);
            agent.AddAction("left", MOVE_LEFT);
            agent.AddAction("right", MOVE_RIGHT);
            agent.SetProperty("key_property",0.0); ///< If key is set to 0, agent does not have possession of key. If it is 1, agent has possession of key and can exit through door
            agent.SetProperty("tar_property",5.0); ///< If it is set to 5.0, agent is free to move. if it is set to 6.0, agent is stuck
        }

        /**
         * Ends game if agent is the player
         * @param agent The agent being damaged
         */
        void DamageAgent(AgentBase &agent) {
            if (agent.IsInterface()) {
                EndGame(false);
            }
        }

        /**
         * Ends the game
         * @param win True if the game is ending in a win. False if it is a loss
         */
        void EndGame(bool win) {
            run_over = true;

            if (win) {
                std::cout << "You successfully exited maze!" << std::endl;
            } else {
                std::cout << "Game over, try again!" << std::endl;
            }
        }

    public:
        explicit GenerativeWorld(BiomeType biome, unsigned int width, unsigned int height, unsigned int seed) : WorldBase(seed) {
            floor_id = AddCellType("floor", "Floor that you can easily walk over.", ' ');
            wall_id = AddCellType("wall", "Impenetrable wall that you must find a way around.", '#');

            spike_id = AddCellType("spike", "Dangerous spike that resets the game.", 'X');

            tar_id = AddCellType("tar", "Slow tile that makes you take two steps to get through it", 'O');
            key_id = AddCellType("key", "item that can be picked up to unlock door and escape maze", 'K');
            door_id = AddCellType("door", "Door that can be walked through only with possession of key to leave maze",
                                  'D');

            tree_id = AddCellType("tree", "A tree that blocks the way.", 't');
            grass_id = AddCellType("grass", "Grass you can walk on.", 'M');
            dirt_id = AddCellType("dirt", "Dirt you can walk on.", '~');

            BiomeGenerator biomeGenerator(biome, width, height, seed);
            biomeGenerator.setWorld(this);
            biomeGenerator.generate();

            biomeGenerator.saveToFile("../assets/grids/generated_maze.grid");

            main_grid.Read("../assets/grids/generated_maze.grid", type_options);
        }

        ~GenerativeWorld() override = default;

        /// Allow the agents to move around the maze.
        int DoAction(AgentBase &agent, size_t action_id) override {
            // Determine where the agent is trying to move.
            GridPosition currentPosition = agent.GetPosition();
            GridPosition new_position;
            switch (action_id) {
                case REMAIN_STILL:
                    new_position = agent.GetPosition();
                    break;
                case MOVE_UP:
                    new_position = agent.GetPosition().Above();
                    break;
                case MOVE_DOWN:
                    new_position = agent.GetPosition().Below();
                    break;
                case MOVE_LEFT:
                    new_position = agent.GetPosition().ToLeft();
                    break;
                case MOVE_RIGHT:
                    new_position = agent.GetPosition().ToRight();
                    break;
            }

            // Don't let the agent move off the world or into a wall.
            if (!main_grid.IsValid(new_position)) { return false; }
            if (main_grid.At(new_position) == wall_id) { return false; }

    //check to see if player has shield on and is walking across spike tile
    if( main_grid.At(new_position) == spike_id )
    {
        for( const auto &pair : item_map )
        {
            //agent has shield item
            if( agent.HasItem(pair.first) && pair.second->GetName() == "Shield" )
            {
                //agent's shield has enough health and will protect player from spike tile
                if( pair.second->GetProperty("Health") > 0 )
                {
                    pair.second->SetProperty("Health", pair.second->GetProperty("Health") - 1);
                    agent.SetPosition(new_position);
                    return true;
                }
                //agent's shield has no health and game will end
                else
                {
                    std::cout << "Game over, try again!" << std::endl;
                    exit(0);  // Halting the program
                }
            }
            //item is not a shield and program continues to loop through inventory
            else
            {
                continue;
            }
        }
    }

    //check to see if player has boots on and is walking across tar
    if( main_grid.At(new_position) == tar_id)
    {
        for( const auto &pair : item_map )
        {
            //agent has boots item
            if( agent.HasItem(pair.first) && pair.second->GetName() == "Boots" )
            {
                //agent's boots have enough health and will protect player from tar
                if( pair.second->GetProperty("Health") > 0 )
                {
                    pair.second->SetProperty("Health", pair.second->GetProperty("Health") - 1);
                    agent.SetPosition(new_position);
                    return true;
                }
                //agents boots have no health and will not work
                else
                {
                    break;
                }
            }
            //item is not boots and program continues to loop through inventory
            else
            {
                continue;
            }
        }
    }

            //check to see if agent is walking on an item
            for (const auto &pair: item_map) {
                if (pair.second->GetPosition() == new_position) {
                    //Add item to inventory
                    agent.AddItem(pair.first);
                }
            }

    // agent is moving onto a key tile and picking it up
    if( main_grid.At(new_position) == key_id )
    {
        agent.SetProperty("key_property", 1.0);
        main_grid.At(new_position) = floor_id;
    }

            // player is exiting through door with key and ending game
            if (main_grid.At(new_position) == door_id && agent.IsInterface() && agent.GetProperty("key_property") == 1.0) {
                EndGame(true);
            }

            // Agent is on tar tile and trying to move to a tar tile
            if (main_grid.At(new_position) == tar_id && main_grid.At(currentPosition) == tar_id) {
                // Agent is stuck on tar
                if (agent.GetProperty("tar_property") == 6.0) {
                    agent.SetProperty("tar_property", 5.0);
                    new_position = currentPosition;
                    return false;
                } else {
                    agent.SetProperty("tar_property", 6.0);
                    agent.SetPosition(new_position);
                    return true;
                }
            }
            // determining if player is moving onto a tar tile and setting tar property to 6.0 if so
            if (main_grid.At(new_position) == tar_id) {
                agent.SetProperty("tar_property", 6.0);
            }

            //Determining if agent is stuck on tar or not
            if (main_grid.At(currentPosition) == tar_id) {
                // Agent is stuck on tar
                if (agent.GetProperty("tar_property") == 6.0) {
                    agent.SetProperty("tar_property", 5.0);
                    new_position = currentPosition;
                    return false;
                }
            }

            // Set the agent to its new position.
            agent.SetPosition(new_position);

            return true;
        }

    };

} // End of namespace cse491
