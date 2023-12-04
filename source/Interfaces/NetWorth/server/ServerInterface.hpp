/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A networking interface that allows information to be sent across a  network
 * @note Status: PROTOTYPE
 **/

#pragma once

#include "ServerManager.hpp"
#include <thread>
#include <sstream>
#include "Interfaces/NetWorth/NetworkInterface.hpp"
#include "Worlds/MazeWorld.hpp"

namespace netWorth
{
	using namespace sf;

/**
 * The server that will be running and that allows clients to connect to, acts as a sort of clone
 */
	class ServerInterface : public NetworkingInterface
	{
	private:
		ServerManager* m_manager = nullptr;

		unsigned short m_world_update_port;
	protected:

	public:
		/**
		 * Default constructor (AgentBase)
		 * @param id agent ID
		 * @param name agent name
		 */
		ServerInterface(size_t id, const std::string& name)
			: cse491::InterfaceBase(id, name),
			  NetworkingInterface(id, name)
		{

		}

		bool Initialize() override
		{
			// resolve port and IP from entity properties
			m_ip = sf::IpAddress::resolve(NetworkingInterface::GetProperty<std::string>("client_ip"));
			m_port = NetworkingInterface::GetProperty<unsigned short>("client_port");
			m_world_update_port = m_port;
			m_manager = GetProperty<netWorth::ServerManager*>("server_manager");
			return InitialConnection(m_ip, m_port);
		}

		/**
		 * The initial connection for the server to a client
		 * @param sender address of the sender
		 * @param port port of the connection
		 * @return true if successful
		 */
		bool InitialConnection(std::optional<IpAddress>& sender, unsigned short& port)
		{
			Packet send_pkt, recv_pkt;
			std::string str;

			std::cout << sf::IpAddress::getLocalAddress().value() << std::endl;
			BindSocket(m_socket, GetProperty<unsigned short>("server_port"));

			// Await client
			if (!ReceivePacket(recv_pkt, sender, port))
				return false;

			m_port = port;

			recv_pkt >> str;
			std::cout << str << std::endl;
			std::cout << sender.value() << " has connected successfully." << std::endl;

			// Acknowledge client
			send_pkt << "Connection established.";
			if (!SendPacket(send_pkt, sender.value(), m_port))
				return false;

			recv_pkt.clear();
			std::string str2;
			// await request for map
			if (!ReceivePacket(recv_pkt, sender, m_port))
				return false;

			recv_pkt >> str2;
			std::cout << str2 << std::endl;

			// set non-blocking for gameplay
			//m_socket.setBlocking(false);
			return true;
		}

		/**
		 * The grid that will be sent to the client from the server after the connection
		 * so the client can start asking to make moves
		 * @param grid the grid to send to the server
		 * @param type_options different cell types of the world
		 * @param item_map the items that may be apart of the grid
		 * @param agent_map the agents that may be apart of the grid
		 * @return the grid that will be sent to the client
		 */
		static Packet GridToPacket(const cse491::WorldGrid& grid,
			const cse491::type_options_t& type_options,
			const cse491::item_map_t& item_map,
			const cse491::agent_map_t& agent_map)
		{
			std::vector<std::string> packet_grid(grid.GetHeight());

			// Load the world into the symbol_grid;
			for (size_t y = 0; y < grid.GetHeight(); ++y)
			{
				packet_grid[y].resize(grid.GetWidth());
				for (size_t x = 0; x < grid.GetWidth(); ++x)
				{
					packet_grid[y][x] = type_options[grid.At(x, y)].symbol;
				}
			}

			// Add in the agents / entities
			for (const auto& [id, entity_ptr] : item_map)
			{
				cse491::GridPosition pos = entity_ptr->GetPosition();
				packet_grid[pos.CellY()][pos.CellX()] = '+';
			}

			for (const auto& [id, agent_ptr] : agent_map)
			{
				cse491::GridPosition pos = agent_ptr->GetPosition();
				char c = '*';
				if (agent_ptr->HasProperty("symbol"))
				{
					c = agent_ptr->GetProperty<char>("symbol");
				}
				packet_grid[pos.CellY()][pos.CellX()] = c;
			}

			// Print out the symbol_grid with a box around it.
			std::ostringstream oss;
			oss << '+' << std::string(grid.GetWidth(), '-') << "+\n";
			for (const auto& row : packet_grid)
			{
				oss << "|";
				for (char cell : row)
				{
					oss << cell;
				}
				oss << "|\n";
			}
			oss << '+' << std::string(grid.GetWidth(), '-') << "+\n";
			oss << "\nYour move? ";
			std::string gridString = oss.str();

			Packet gridPacket;
			gridPacket << gridString;

			return gridPacket;
		}

		/**
		 * Choose action for player agent (mirror client agent)
		 * @param grid the server-side grid
		 * @param type_options different cell types of the world
		 * @param item_map the items that may be apart of the grid
		 * @param agent_map the agents that may be apart of the grid
		 * @return action ID of the interface
		 */
		size_t SelectAction(const cse491::WorldGrid& grid,
			const cse491::type_options_t& type_options,
			const cse491::item_map_t& item_set,
			const cse491::agent_map_t& agent_set) override
		{
			if (m_manager->hasNewAgent)
			{
				sf::Packet serializedAgentPkt;
				serializedAgentPkt << m_manager->GetSerializedAgents();
				SendPacket(serializedAgentPkt, m_ip.value(), m_world_update_port);
				m_manager->hasNewAgent = false;
			}

			// send action map to client
			sf::Packet send_pkt = m_manager->ActionMapToPacket();
			std::cout << "Sending action map to " << m_ip.value().toString() << " on port " << m_port << std::endl;
			SendPacket(send_pkt, m_ip.value(), m_port);

			// print server-side map (for test purposes)
			sf::Packet map_pkt = GridToPacket(grid, type_options, item_set, agent_set);
			std::string map;
			map_pkt >> map;
			std::cout << map << std::endl;

			// receive player input
			sf::Packet recv_pkt;
			size_t action_id;

			// comment this out for speed
			ReceivePacket(recv_pkt, m_ip, m_port);
			recv_pkt >> action_id;

			// uncomment this out for speed
//                std::optional<sf::IpAddress> temp_ip;
//                unsigned short temp_port;
//                if (m_socket.receive(recv_pkt, temp_ip, temp_port) == sf::Socket::Status::Done) {
//                    // received from client
//                    recv_pkt >> action_id;
//                } else {
//                    action_id = 0;
//                }

			// TODO: Figure out how to quit (client-side exit(0) in MainInterface upon q/esc)
			//            if (input == "quit") exit(0);
			if (action_id == 9999)
			{
				m_manager->RemoveFromActionMap(GetID());
			}

			return action_id;
		}

		/**
		 * Process client input packet (just print action for now)
		 * @param packet packet from client
		 */
		void ProcessPacket(Packet packet)
		override
		{
			size_t str;
			packet >> str;
			std::cout << str << std::endl;
		}

	}; // End of class ServerInterface
// End of namespace netWorth
}