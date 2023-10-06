/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A World that consists of trees, water, and grass cells.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <cassert>
#include <stdlib.h>

#include "../core/WorldBase.hpp"

namespace cse491_team8 {

  class ManualWorld : public cse491::WorldBase {
  protected:
    enum ActionType { REMAIN_STILL=0, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, ATTACK };
    enum FacingDirection { UP=1, RIGHT, DOWN, LEFT};

    size_t floor_id; ///< Easy access to floor CellType ID.
    size_t tree_id;  ///< Easy access to tree CellType ID.
    size_t water_id;  ///< Easy access to water CellType ID.
    size_t bridge_id;  ///< Easy access to bridge CellType ID.

    /// Provide the agent with movement actions.
    void ConfigAgent(cse491::AgentBase & agent) override {
      agent.AddAction("up", MOVE_UP);
      agent.AddAction("down", MOVE_DOWN);
      agent.AddAction("left", MOVE_LEFT);
      agent.AddAction("right", MOVE_RIGHT);
      agent.AddAction("attack", ATTACK);
    }

  public:
    ManualWorld() {
      floor_id = AddCellType("grass", "Grass that you can easily walk over.", ' ');
      tree_id = AddCellType("tree", "Tree that you cannot pass without an axe.", '^');
      water_id = AddCellType("water", "Water that cannot be crossed without a boat.", '~');
      bridge_id = AddCellType("bridge", "Bridge that goes over water.", '#');
      main_grid.Read("../assets/grids/team8_large_grid.grid", type_options);
    }
    ~ManualWorld() = default;


    /// @brief Checks the strength between two agents
    /// @param other_agent The autonomous agent to compare
    /// @param agent The interface (player) agent to compare
    /// Prints the stronger agent and removes the weaker
    /// @see RemoveAgent
    /// @return None
    void StrengthCheck(const std::unique_ptr<cse491::AgentBase> & other_agent, 
                     const std::unique_ptr<cse491::AgentBase> & agent) {
        bool won = false;
        bool run = false;
        char input;
        while (other_agent->GetProperty("Health") > 0 && agent->GetProperty("Health") > 0)
        {
            bool valid_input = true;
            std::cout << "a for attack, s for special, r for run, h for heal" << "\n";
            std::cout << "Your Attack: ";
            std::cin >> input;
            double damage = 0.0;
            double other_damage = other_agent->GetProperty("Strength");
            switch (input) {
            case 'a': case 'A': damage = agent->GetProperty("Strength");    break;
            case 's': case 'S': damage = agent->GetProperty("Strength") * 1.5;  break;
            case 'r': case 'R': won = false; run = true; break;
            case 'h': case 'H': agent->SetProperty("Health",
                    agent->GetProperty("Health") + agent->GetProperty("Strength") * 0.5); break;
            default: valid_input = false; break;
            }
            if (!valid_input)
            {
                std::cout << "Invalid Input" << "\n";
                continue;
            }
            if (run)
            {
                agent->SetProperty("Health", agent->GetProperty("Health") - other_damage);
                break;
            }
            other_agent->SetProperty("Health", other_agent->GetProperty("Health") - damage);
            if (other_agent->GetProperty("Health") <= 0)
            {
                won = true;
                break;
            }
            agent->SetProperty("Health", agent->GetProperty("Health") - other_damage);
            if (agent->GetProperty("Health") <= 0)
            {
                break;
            }
        }

        if (!won) {
            if (run)
            {
                std::cout << "You ran away, this means you don't gain health or strength and any battle damage stays!" << "\n";
            }
            if (agent->GetName() == "Interface" && agent->GetProperty("Health") <= 0)
            {
                std::cout << other_agent->GetName() << " has beat " << agent->GetName() << "\n";
                std::cout << "You Lost..." << "\n";
                std::cout << "Would You Like To Continue? Y or N? ";
                char repeat_input;
                std::cin >> repeat_input;
                if (repeat_input == 'N')
                {
                    exit(0);
                }
                else
                {
                    agent->SetProperty("Health", 20);
                    agent->SetProperty("Strength", 7);
                    agent->SetPosition(40, 3);
                }
            }
            this->RemoveAgent(agent->GetName()); // If the agent is the interface, this does nothing
        }
        else {
        std::cout << agent->GetName() << " has beat " << other_agent->GetName() << "\n";
        // Gain the Agent's strength that you beat
        agent->SetProperty("Strength",
                agent->GetProperty("Strength") + other_agent->GetProperty("Strength"));
        cse491::GridPosition position = other_agent->GetPosition();
        int x = position.GetX();
        int y = position.GetY();
        this->RemoveAgent(other_agent->GetName());

        // get the loot dropped from the agent
        int random = rand() % 10;
        char loot = ' ';
        if (random <= 4) {
            loot = '/';
        }
        else if (random <= 7) {
            loot = '|';
        }
        else if (random <= 9) {
            loot = '{';
        }
        else {
            loot = '!';
        }

        // put the loot where the agent was
        cse491::WorldGrid grid = GetGrid();
        grid.AddLoot(x, y, loot);   // TK does not currently display properly

      }
    }
   
    /// @brief Checks for agents adjacent to the given agent
    /// Checks the strengths if an agent is found
    /// @param agent The agent to look around
    /// @see StrengthCheck
    /// @return true if an agent was found, else false
    bool CheckAround(const std::unique_ptr<cse491::AgentBase> & agent)
    {
      auto new_position = agent->GetPosition();
      for (auto& other_agent : agent_set)
      {
        if (other_agent->GetPosition() == new_position.Above() ||
            other_agent->GetPosition() == new_position.Below() ||
            other_agent->GetPosition() == new_position.ToLeft() ||
            other_agent->GetPosition() == new_position.ToRight())
        {
          StrengthCheck(other_agent, agent);
          return true;
        }
      }
      return false;
    }


    /// Finds an interface agent and calls CheckAround
    void HandleNeighbors()
    {
      for (const auto& agent : agent_set)
      {
        if (agent->IsInterface())
        {
          CheckAround(agent);
          break;
        }
      }
    }


    /// Runs agents, updates the world, and looks for adjacencies
    void Run() override
    {
      run_over = false;
      while (!run_over) {
        RunAgents();
        UpdateWorld();
        HandleNeighbors();
      }
    }

    /// Allow the agents to move around the maze.
    int DoAction(cse491::AgentBase & agent, size_t action_id) override {
      // Determine where the agent is trying to move.
      cse491::GridPosition new_position;

      switch (action_id) {
      case REMAIN_STILL:
      {
          new_position = agent.GetPosition();
          break;
      }
      case MOVE_UP:      
      {
        agent.SetFacingDirection(UP);
        new_position = agent.GetPosition().Above();
        break;
      }
      case MOVE_DOWN:    
      {
        agent.SetFacingDirection(DOWN);
        new_position = agent.GetPosition().Below();
        break;
      }
      case MOVE_LEFT:    
      {
        agent.SetFacingDirection(LEFT);
        new_position = agent.GetPosition().ToLeft();
        break;
      }
      case MOVE_RIGHT:   
      {
        agent.SetFacingDirection(RIGHT);
        new_position = agent.GetPosition().ToRight();
        break;
      }
      }

      // Don't let the agent move off the world or into a wall.
      if (!main_grid.IsValid(new_position)) { return false; }
      if (main_grid.At(new_position) == tree_id) { return false; }
      if (main_grid.At(new_position) == water_id) { return false; }

      // Set the agent to its new postion.
      agent.SetPosition(new_position);

      return true;
    }

  };

} // End of namespace cse491
