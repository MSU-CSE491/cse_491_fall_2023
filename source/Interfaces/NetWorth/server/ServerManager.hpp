/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A networking interface that allows information to be sent across a  network
 * @note Status: PROTOTYPE
 **/

#pragma once
#include <thread>
#include <sstream>
#include <vector>
#include "Interfaces/NetWorth/NetworkInterface.hpp"

namespace netWorth{
    using namespace sf;

    /**
     * The server that will be running and that allows clients to connect to
     */
    class ServerManager {
    private:
        UdpSocket m_serverToClientSocket;

        std::vector<std::thread> m_clientThreads;
    protected:

    public:
        const static constexpr unsigned short m_initConnectionPort = 55000;
        unsigned short m_maxClientPort = 55000;
        /**
         * Default constructor (AgentBase)
         * @param id agent ID
         * @param name agent name
         */
        ServerManager()= default;

        void IncreasePort(){++m_maxClientPort;}

        void AddClient(std::thread &thread){m_clientThreads.push_back(std::move(thread));}

        void JoinClients(){
            for (auto &thread: m_clientThreads){
                thread.join();
            }
        }

    }; // End of class ServerManager
} // End of namespace netWorth