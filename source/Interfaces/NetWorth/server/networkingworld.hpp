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

namespace cse491 {
    namespace netWorth{
class NetworkMazeWorld : public WorldBase {
  protected:
    enum ActionType { REMAIN_STILL=0, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };

    size_t floor_id; ///< Easy access to floor CellType ID.
    size_t wall_id;  ///< Easy access to wall CellType ID.

    /// Provide the agent with movement actions.
    void ConfigAgent(AgentBase & agent) override {
      agent.AddAction("up", MOVE_UP);
      agent.AddAction("down", MOVE_DOWN);
      agent.AddAction("left", MOVE_LEFT);
      agent.AddAction("right", MOVE_RIGHT);
    }

  public:
    NetworkMazeWorld() {
      floor_id = AddCellType("floor", "Floor that you can easily walk over.", ' ');
      wall_id = AddCellType("wall", "Impenetrable wall that you must find a way around.", '#');
      main_grid.Read("../../../../assets/grids/default_maze.grid", type_options);
    }
    ~NetworkMazeWorld() = default;

    Packet GridToPacket(const cse491::WorldGrid & grid, const cse491::type_options_t & type_options,
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

    void NetworkRun(sf::UdpSocket * serverSocket, IpAddress sender, unsigned short port){
      run_over = false;
      cse491::item_set_t item_set;
      cse491::agent_set_t agent_set;
      while (!run_over) {
        RunAgents();
        UpdateWorld();
        Packet gridPacket = GridToPacket(GetGrid(), GetCellTypes(), item_set, agent_set);
        serverSocket->send(gridPacket, sender, port);
      }
    }

    

    /// Allow the agents to move around the maze.
    int DoAction(AgentBase & agent, size_t action_id) override {
      // Determine where the agent is trying to move.
      GridPosition new_position;
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

  };
    }
  
} // End of namespace cse491
