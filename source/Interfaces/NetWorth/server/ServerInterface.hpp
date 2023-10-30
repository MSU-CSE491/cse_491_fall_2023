/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A networking interface that allows information to be sent across a  network
 * @note Status: PROTOTYPE
 **/

#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <sstream>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/Packet.hpp>

#include "../NetworkInterface.hpp"
#include "networkingworld.hpp"

namespace netWorth{
    using namespace sf;

    class NetworkMazeWorld;

    /**
     * The server that will be running and that allows clients to connect to
     */
    class ServerInterface : public NetworkingInterface {
    private:

    protected:

    public:
        ServerInterface() = default;
        virtual ~ServerInterface() = default;

        /**
         * The initial connection for the server to a client
         * @param sender address of the sender
         * @param send_pkt the packet that the server will send initially
         * @param recv_pkt the packet that the server will receive initially
         * @param port port of the connection
         * @param str string for the receive packet
         * @return true if successful
         */
        bool InitialConnection(std::optional<IpAddress> &sender, unsigned short &port){
            Packet send_pkt, recv_pkt;
            std::string str;

            BindSocket(m_socket, 55002);

            // Await client
            if (!ReceivePacket(recv_pkt, sender, port)) return false;

            recv_pkt >> str;
            std::cout << str << std::endl;
            std::cout << sender.value() << " has connected successfully." << std::endl;

            // Acknowledge client
            send_pkt << "Connection established.";
            if (!SendPacket(send_pkt, sender.value(), port)) return false;

            // await request for map
            if (!ReceivePacket(recv_pkt, sender, port)) return false;

            recv_pkt >> str;
            std::cout << str << std::endl;

            return true;
        }

        /**
         * The grid that will be sent to the client from the server after the connection
         * so the client can start asking to make moves
         * @param grid the grid to send to the server
         * @param type_options different cell types of the world
         * @param item_set the items that may be apart of the grid
         * @param agent_set the agents that may be apart of the grid
         * @return the grid that will be sent to the client
         */
        static Packet GridToPacket(const cse491::WorldGrid & grid, const cse491::type_options_t & type_options,
                                   const cse491::item_set_t & item_set, const cse491::agent_set_t & agent_set)
        {
            std::vector<std::string> packet_grid(grid.GetHeight());

            // Load the world into the symbol_grid;
            for (size_t y=0; y < grid.GetHeight(); ++y) {
                packet_grid[y].resize(grid.GetWidth());
                for (size_t x=0; x < grid.GetWidth(); ++x) {
                    packet_grid[y][x] = type_options[ grid.At(x,y) ].symbol;
                }
            }

            // Add in the agents / entities
            for (const auto & entity_ptr : item_set) {
                cse491::GridPosition pos = entity_ptr->GetPosition();
                packet_grid[pos.CellY()][pos.CellX()] = '+';
            }

            for (const auto & agent_ptr : agent_set) {
                cse491::GridPosition pos = agent_ptr->GetPosition();
                char c = '*';
                if(agent_ptr->HasProperty("symbol")){
                    c = agent_ptr->GetProperty<char>("symbol");
                }
                packet_grid[pos.CellY()][pos.CellX()] = c;
            }

            // Print out the symbol_grid with a box around it.
            std::ostringstream oss;
            oss << '+' << std::string(grid.GetWidth(),'-') << "+\n";
            for (const auto & row : packet_grid) {
                oss << "|";
                for (char cell : row) {
                    oss << cell;
                }
                oss << "|\n";
            }
            oss << '+' << std::string(grid.GetWidth(),'-') << "+\n";
            oss << "\nYour move? ";
            std::string gridString = oss.str();

            Packet gridPacket;
            gridPacket << gridString;

            return gridPacket;
        }

    };//End of class ServerInterface
}//End of namespace netWorth