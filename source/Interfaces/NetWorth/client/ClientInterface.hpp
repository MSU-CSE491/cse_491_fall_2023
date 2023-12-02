/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A networking interface that allows information to be sent across a  network
 * @note Status: PROTOTYPE
 **/

#pragma once


#include <chrono>
#include <thread>
#include "Interfaces/NetWorth/NetworkInterface.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Interfaces/MainInterface.hpp"
#include "ClientManager.hpp"

namespace netWorth{
    /**
     * The interface of our client that will be interacting and connection with our server
     */
    using namespace sf;

    class ClientInterface : public NetworkingInterface, i_2D::MainInterface {
        private:
            unsigned short m_serverPort;
            netWorth::ClientManager *m_manager = nullptr;
            sf::UdpSocket *m_game_update_socket = nullptr;

        protected:

        public:
            /**
             * Default constructor (AgentBase)
             * @param id agent ID
             * @param name agent name
             */
            ClientInterface(size_t id, const std::string & name) : cse491::InterfaceBase(id, name),
                                                                   NetworkingInterface(id, name),
                                                                   i_2D::MainInterface(id, name) {}
            /**
             * Establish connection with server, initializing interface
             * @return True if successful, false if error
             */
            bool Initialize() override {
                // resolve port and IP from entity properties
                m_ip = sf::IpAddress::resolve(NetworkingInterface::GetProperty<std::string>("server_ip"));
                m_port = NetworkingInterface::GetProperty<unsigned short>("server_port");
                m_manager = GetProperty<netWorth::ClientManager *>("manager");
                m_game_update_socket = GetProperty<sf::UdpSocket *>("socket");
                m_game_update_socket->setBlocking(false);
                m_manager->SetupSocket(&m_socket, m_ip, m_port);

                Packet send_pkt, recv_pkt,two_pkt;

                // send request message
                send_pkt << "New client requesting connection.";
                std::cout << "Requesting connection" << std::endl;
                auto ip = m_ip.value();
                if (!SendPacket(send_pkt, ip, m_port)) return false;

                // receive from server
                if (!ReceivePacket(recv_pkt, m_ip, m_port)) return false;
                std::cout<<"lol" << std::endl;
                // print received string (Connection established.)
                std::string msg;
                recv_pkt >> msg;
                std::cout << msg << std::endl;

                // request map to start send/receive loop
                send_pkt.clear();
                send_pkt << "Requesting start";
                if (!SendPacket(send_pkt, m_ip.value(), m_port)) return false;

                std::cout<<"I love bread" << std::endl;
                return true;
            }




        /**
             * Choose action for player agent
             * https://chat.openai.com/share/df584b9b-a1cc-4c79-beb6-b8c66ac47ba4 used for part of this function
             * @param grid the client-side grid
             * @param type_options different cell types of the world
             * @param item_map the items that may be apart of the grid
             * @param agent_map the agents that may be apart of the grid
             * @return action ID of the interface
             */
            size_t SelectAction(const cse491::WorldGrid & grid,
                                const cse491::type_options_t & type_options,
                                const cse491::item_map_t & item_set,
                                const cse491::agent_map_t & agent_set) override
            {
                // Receive and draw map
                sf::Packet send_pkt, recv_pkt;
                std::string map;
                //std::cout<<"going to receive packet" << std::endl;
                //ReceivePacket(rec_pkt, m_ip, m_serverPort);
                //rec_pkt >> map;
                //std::cout<<"hiiiiii" << std::endl;
                //ProcessPacket(rec_pkt);

                std::string userInput;
                std::chrono::milliseconds timeout(1000);
                std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

                std::cout << "Enter action within " << 1000 << " milliseconds: ";

                size_t action_id = 0;
                // Asynchronously check for user input
                std::thread inputThread([&]() {
                    action_id = i_2D::MainInterface::SelectAction(grid, type_options,
                                                      item_set, agent_set);
                });

                // Wait for either user input or timeout
                while (inputThread.joinable() &&
                       std::chrono::steady_clock::now() - start < timeout) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }

                // If the input thread is still joinable, it means it hasn't finished
                if (inputThread.joinable()) {
                    std::cout << "\nTimeout reached. No input detected.\n";
                    DrawGrid(grid, type_options, item_set, agent_set);
                    inputThread.join(); // Make sure to join the thread before returning
                } else {
                    inputThread.join(); // Join the thread before returning
                    std::cout << "Input received: " << userInput << "\n";

                }

                // grab action ID from MainInterface

                std::cout << action_id << std::endl;

                // Send instruction to server
                send_pkt << action_id;
                SendPacket(send_pkt, m_ip.value(), m_port);

                m_manager->ClearActionMap();

                // Do the action!
                return action_id;
            }

            /**
             * Process packet from server (just print agent action map for now)
             * @param packet packet from server
             */
            void ProcessPacket(Packet packet) override {
                size_t data_size, data;
                packet >> data_size;
                std::cout << data_size << " agents" << std::endl;
                for (size_t i = 0; i < data_size; i++) {
                    packet >> data;
                    std::cout << "agent " << data;
                    packet >> data;
                    std::cout << " action " << data << std::endl;
                }
                std::cout << std::endl;
            }

    }; // End of ClientInterface
} // End of namespace NetWorth
