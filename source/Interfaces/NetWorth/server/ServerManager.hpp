/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A networking interface that allows information to be sent across a  network
 * @note Status: PROTOTYPE
 **/

#pragma once
#include <thread>
#include <map>
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
        std::vector<std::thread> m_clientThreads; ///Vector of all client threads

        std::map<size_t, size_t> m_action_map; ///Map of agent IDs to most recent action selected

        std::mutex m_actionMapMutex; ///Mutex regarding the action map

        std::mutex m_connectionThreadMutex; ///Mutex regarding all connection threads
    protected:

    public:
        const static constexpr unsigned short m_initConnectionPort = 55000; ///Port for initial client connection

        unsigned short m_maxClientPort = 55000; ///Port that is incremented for client thread handoff

        /**
         * Default constructor (AgentBase)
         * @param id agent ID
         * @param name agent name
         */
        ServerManager()= default;

        std::mutex & GetThreadMutex(){return m_connectionThreadMutex;}

        std::mutex & GetActionMutex(){return m_actionMapMutex;}

        /**
         * Increases the max client port
         */
        void IncreasePort(){++m_maxClientPort;}

        /**
         * Joins all client threads at the end of the server's lifespan
         */
        void JoinClients(){
            for (auto &thread: m_clientThreads){
                thread.join();
            }
        }

        bool ActionMapContains(size_t key){return m_action_map.contains(key);}

        size_t ReadFromActionMap(size_t key){
            std::lock_guard<std::mutex> actionLock(m_actionMapMutex);
            try{
                return m_action_map.at(key);
            }

            catch (std::out_of_range & e){
                return 0;
            }
        }

        void WriteToActionMap(size_t key, size_t val){
            std::lock_guard<std::mutex> actionLock(m_actionMapMutex);
            m_action_map.insert_or_assign(key, val);
        }

        size_t AddToThreadVector(std::thread& thread){
            std::lock_guard<std::mutex> threadLock(m_connectionThreadMutex);
            m_clientThreads.push_back(std::move(thread));
        }

    }; // End of class ServerManager
} // End of namespace netWorth