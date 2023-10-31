/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A World that consists only of walls and open cells.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <cassert>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include "../../../core/WorldBase.hpp"


#include "ServerInterface.hpp"

const std::string filePath = "../assets/grids/default_maze.grid";

namespace netWorth{

    using namespace sf;
    /**
     * The world that is being sent over the network between the client and the server
     */
    class NetworkMazeWorld : public cse491::WorldBase {
    private:
        std::shared_ptr<netWorth::ServerInterface> m_server; /// The server that will be used to make changes to the world
        /// and send back to the client
        std::string m_player_action;

    protected:
        enum ActionType { REMAIN_STILL=0, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };

        size_t floor_id; ///< Easy access to floor CellType ID.
        size_t wall_id;  ///< Easy access to wall CellType ID.

        /// Provide the agent with movement actions.
        void ConfigAgent(cse491::AgentBase & agent) override {
            agent.AddAction("up", MOVE_UP);
            agent.AddAction("down", MOVE_DOWN);
            agent.AddAction("left", MOVE_LEFT);
            agent.AddAction("right", MOVE_RIGHT);
        }

    public:
        NetworkMazeWorld() {
            floor_id = AddCellType("floor", "Floor that you can easily walk over.", ' ');
            wall_id = AddCellType("wall", "Impenetrable wall that you must find a way around.", '#');

            //Set your proper filepath here for now until we figure out a workaround
            main_grid.Read(filePath, type_options);
        }

        ~NetworkMazeWorld() = default;

        /**
         * Set a server to the one that is connected to the clients
         * @param server the server we want to set
         */
        void SetServer(std::shared_ptr<netWorth::ServerInterface> &server){
            m_server = server;
        }

        /**
         * gets a grid packet from the grid used in Trash Interface
         * @return the grid packet
         */
        Packet GetGridPacket(){
            Packet gridPacket = m_server->GridToPacket(GetGrid(), GetCellTypes(), item_set, agent_set);
            return gridPacket;
        }

        /// Allow the agents to move around the maze.
        int DoAction(cse491::AgentBase & agent, size_t action_id) override {
            if (agent.HasProperty("isPlayer")) {
                action_id = agent.GetActionID(m_player_action);
            }

            // Determine where the agent is trying to move.
            cse491::GridPosition new_position;
            switch (action_id) {
                case REMAIN_STILL: new_position = agent.GetPosition(); break;
                case MOVE_UP:      new_position = agent.GetPosition().Above(); break;
                case MOVE_DOWN:    new_position = agent.GetPosition().Below(); break;
                case MOVE_LEFT:    new_position = agent.GetPosition().ToLeft(); break;
                case MOVE_RIGHT:   new_position = agent.GetPosition().ToRight(); break;
            }

            // Don't let the agent move off the world or into a wall.
            if (!main_grid.IsValid(new_position)) { return false; }
            if (main_grid.At(new_position) == wall_id) { return false; }

            // Set the agent to its new postion.
            agent.SetPosition(new_position);

            return true;
        }

        void SetPlayerAction(const std::string &action) {
            m_player_action = action;
        }

    };
}
