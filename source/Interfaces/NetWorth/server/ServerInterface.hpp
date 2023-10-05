/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A networking interface that allows information to be sent across a  network
 * @note Status: PROTOTYPE
 **/

#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/Packet.hpp>

#include "../NetworkInterface.hpp"
#include "networkingworld.hpp"



using namespace sf;

namespace cse491 {
    namespace netWorth{

        class NetworkMazeWorld;

        class ServerInterface : public NetworkingInterface {
        private:
        std::shared_ptr<cse491::netWorth::NetworkMazeWorld> mWorld;


        protected:


        public:
            ServerInterface() : NetworkingInterface() { }
            ~ServerInterface() = default;


            void SetWorld(std::shared_ptr<cse491::netWorth::NetworkMazeWorld> &world) {
                mWorld = world;
            }

            void InitialConnection(sf::IpAddress sender, sf::Packet send_pkt, sf::Packet recv_pkt, unsigned short port, std::string str){
                mSocket.bind(55002);

                mSocket.receive(recv_pkt, sender, port);
                recv_pkt >> str;
                std::cout << sender.toString() << " has connected successfully." << std::endl;

                if(!str.empty()) {
                    std::string msg = "Connection established.";
                    send_pkt << msg;
                    mSocket.send(send_pkt, sender, port);
                }
            }
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
                    if(agent_ptr->HasProperty("char")){
                        c = static_cast<char>(agent_ptr->GetProperty("char"));
                    }
                    packet_grid[pos.CellY()][pos.CellX()] = c;
                }


                // Print out the symbol_grid with a box around it.
                std::ostringstream oss;
                oss << '+' << std::string(grid.GetWidth(),'-') << "+\n";
                for (const auto & row : packet_grid) {
                    oss << "|";
                    for (char cell : row) {
                        // std::cout << ' ' << cell;
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
} // End of namespace cse491