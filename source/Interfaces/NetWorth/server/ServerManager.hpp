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
        //std::map<size_t ,std::thread> m_clientThreads; ///Map of all agent ids and their client threads
        std::set<size_t> m_interface_set;   /// Set of interfaces on the server

        std::map<size_t, size_t> m_action_map; ///Map of agent IDs to most recent action selected

        std::mutex m_actionMapMutex; ///Mutex regarding the action map

        std::mutex m_connectionThreadMutex; ///Mutex regarding all connection threads

        std::string m_currentSerializedAgents; ///String with all current serialized agents

    protected:

    public:
        const static constexpr unsigned short m_initConnectionPort = 55000; ///Port for initial client connection

        unsigned short m_maxClientPort = 55000; ///Port that is incremented for client thread handoff

        bool hasNewAgent;

        bool interfacesPresent = false;

        /**
         * Default constructor (AgentBase)
         * @param id agent ID
         * @param name agent name
         */
        ServerManager() = default;

        std::string GetSerializedAgents(){return m_currentSerializedAgents;}

        void SetSerializedAgents(std::string & serializedAgents) {m_currentSerializedAgents = serializedAgents;}


        /**
         * Convert action map to packet to send to client
         * @return packet containing action map as series of integers
         */
        sf::Packet ActionMapToPacket()
        {
            sf::Packet pkt;

            // serialize action map
            pkt << m_action_map.size();
            for (auto pair: m_action_map) {
                pkt << pair.first << pair.second;
            }

            return pkt;
        }

        std::mutex & GetThreadMutex(){return m_connectionThreadMutex;}

        std::mutex & GetActionMutex(){return m_actionMapMutex;}

        /**
         * Increases the max client port
         */
        void IncreasePort(){++m_maxClientPort;}

        /**
         * Joins all client threads at the end of the server's lifespan
         */
        void JoinAllClients(){
//            for (auto &thread_pair: m_clientThreads){
//                thread_pair.second.join();
//            }
            m_interface_set.clear();
            interfacesPresent = false;
        }

        void JoinClient(size_t id){
//            m_clientThreads.at(id).join();
//            m_clientThreads.erase(id);
//            if (m_clientThreads.empty()){
//                interfacesPresent = false;
//            }
            m_interface_set.erase(id);
            if (m_interface_set.empty()) interfacesPresent = false;
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

        void RemoveFromActionMap(size_t key){
            m_action_map.erase(key);
        }

        void WriteToActionMap(size_t key, size_t val){
            std::lock_guard<std::mutex> actionLock(m_actionMapMutex);
            m_action_map.insert_or_assign(key, val);
        }

        void AddToThreadMap(size_t agent_id){
            std::lock_guard<std::mutex> threadLock(m_connectionThreadMutex);
            //m_clientThreads.insert_or_assign(agent_id, std::move(thread));
            m_interface_set.insert(agent_id);
            interfacesPresent = true;
        }

    }; // End of class ServerManager
} // End of namespace netWorth