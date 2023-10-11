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

    size_t grass_id;  ///< Easy access to floor CellType ID.
    size_t tree_id;   ///< Easy access to tree CellType ID.
    size_t water_id;  ///< Easy access to water CellType ID.
    size_t bridge_id; ///< Easy access to bridge CellType ID.
    size_t rock_id;   ///< Easy access to rock CellType ID.

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
      grass_id = AddCellType("grass", "Grass that you can easily walk over.", ' ');
      tree_id = AddCellType("tree", "Tree that you cannot pass without an axe.", '^');
      water_id = AddCellType("water", "Water that cannot be crossed without a boat.", '~');
      bridge_id = AddCellType("bridge", "Bridge that allows the playerto cross water.", '#');
      rock_id = AddCellType("rock", "Rock that the player cannot cross", '$');
      main_grid.Read("../assets/grids/team8_grid_large.grid", type_options);
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
        while (other_agent->GetProperty<int>("Health") > 0 && agent->GetProperty<int>("Health") > 0)
        {
            bool valid_input = true;
            std::cout << "a for attack, s for special, r for run, h for heal" << "\n";
            std::cout << "Your Attack: ";
            std::cin >> input;
            int damage = 0;
            int other_damage = other_agent->GetProperty<int>("Strength");
            switch (input) {
            case 'a': case 'A': damage = (int)(agent->GetProperty<int>("Strength"));    break;
            case 's': case 'S': damage = (int)(agent->GetProperty<int>("Strength") * 1.5);  break;
            case 'r': case 'R': won = false; run = true; break;
            case 'h': case 'H': agent->SetProperty<int>("Health",
                    agent->GetProperty<int>("Health") + (int)(agent->GetProperty<int>("Max_Health") * 0.25)); break;
            default: valid_input = false; break;
            }
            if (agent->GetProperty<int>("Health") > agent->GetProperty<int>("Max_Health")) {
              agent->SetProperty<int>("Health", agent->GetProperty<int>("Max_Health"));
            }
            std::cout << agent->GetProperty<int>("Health") << " | " << other_agent->GetProperty<int>("Health") << std::endl;
            if (!valid_input)
            {
                std::cout << "Invalid Input" << "\n";
                continue;
            }
            if (run)
            {
                agent->SetProperty<int>("Health", agent->GetProperty<int>("Health") - other_damage);
                break;
            }
            other_agent->SetProperty<int>("Health", other_agent->GetProperty<int>("Health") - damage);
            if (other_agent->GetProperty<int>("Health") <= 0)
            {
                won = true;
                break;
            }
            agent->SetProperty<int>("Health", agent->GetProperty<int>("Health") - other_damage);
            if (agent->GetProperty<int>("Health") <= 0)
            {
                break;
            }
        }

        std::string other_agent_name = other_agent->GetName();

        if (!won) {
            if (run)
            {
                std::cout << "You ran away, this means you don't gain health or strength and any battle damage stays!" << "\n";
            }
            if (agent->GetName() == "Interface" && agent->GetProperty<int>("Health") <= 0)
            {
                std::cout << other_agent_name << " has beat " << agent->GetName() << "\n";
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
                    agent->SetProperty<int>("Health", 20);
                    agent->SetProperty<int>("Strength", 7);
                    agent->SetPosition(40, 3);
                }
            }
            this->RemoveAgent(agent->GetName()); // If the agent is the interface, this does nothing
        }
        else {
        std::cout << agent->GetName() << " has beat " << other_agent->GetName() << "\n";
        // Gain the Agent's strength that you beat
        agent->SetProperty<int>("Strength",
                agent->GetProperty<int>("Strength") + other_agent->GetProperty<int>("Strength"));
        cse491::GridPosition other_position = other_agent->GetPosition();
        this->RemoveAgent(other_agent->GetName());

        // get the loot dropped from the agent
        int random = rand() % 10;
        char symbol = ' ';
        std::string loot = "";
        std::string action = "";
        int num_actions = 0;
        if (random <= 5) {
            symbol = '/';
            loot = "Stick";
            action = "Hit";
            num_actions = 2;
        }
        else if (random <= 7) {
            symbol = 'U';
            loot = "Boat";
            action = "Swim";
            num_actions = 7;
        }
        else if (random <= 9) {
            symbol = 'P';
            loot = "Axe";
            action = "Chop";
            num_actions = 5;
        }
        else {
            symbol = '!';
            loot = "Sword";
            action = "Hit";
            num_actions = 5;
        }

        // put the loot where the agent was
        int x = other_position.CellX();
        int y = other_position.CellY();
        this->AddEntity(loot, "symbol", symbol).SetPosition(x, y);

        // set the entity to have the property of "action type": number of uses
        for (const auto& entity : item_set)
        {
            if (entity->GetPosition() == other_position)
            {
                entity->SetProperty<int>(action, num_actions);
                break;
            }
        }
        
        std::cout << other_agent_name << " dropped their " << loot << "!" << std::endl;

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
          // auto pos = agent->GetPosition().Above();
          // GetGrid().SetCell(pos, 1);
          break;
        }
      }
    }


    /// Runs agents, updates the world, and looks for adjacencies.
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

      // Update Direction property and get new position.
      switch (action_id) {
      case REMAIN_STILL:
      {
          new_position = agent.GetPosition();
          break;
      }
      case MOVE_UP:      
      {
        agent.SetProperty<int>("Direction", UP);
        new_position = agent.GetPosition().Above();
        break;
      }
      case MOVE_DOWN:    
      {
        agent.SetProperty<int>("Direction", DOWN);
        new_position = agent.GetPosition().Below();
        break;
      }
      case MOVE_LEFT:    
      {
        agent.SetProperty<int>("Direction", LEFT);
        new_position = agent.GetPosition().ToLeft();
        break;
      }
      case MOVE_RIGHT:   
      {
        agent.SetProperty<int>("Direction", RIGHT);
        new_position = agent.GetPosition().ToRight();
        break;
      }
      }

      // Don't let the agent move off the world or into a wall.
      if (!main_grid.IsValid(new_position)) { return false; }
      if (main_grid.At(new_position) == tree_id)
      {
          size_t uses_left = agent.CheckInventory("Axe");
          if (uses_left > 0)
          {
              std::cout << "You can use your Axe once to chop down this tree. You have "
                  << uses_left << " uses remaining. Chop this tree? Y/N:\n" << std::endl;
              char chop;
              std::cin >> chop;
              if (chop == 'Y' || chop == 'y')
              {
                  // decrement uses by 1, and change the tree to grass
                  agent.UpdateInventory("Axe", uses_left - 1);
                  main_grid.SetCell(new_position, grass_id);
              }
          }
          // don't have an axe, can't walk here
          return false;
      }
      if (main_grid.At(new_position) == water_id)
      {
          size_t uses_left = agent.CheckInventory("Boat");
          if (uses_left > 0)
          {
              std::cout << "You can use your Boat once to float over this tile. You have "
                  << uses_left << " uses remaining. Use your boat? Y/N:\n" << std::endl;
              char boat;
              std::cin >> boat;
              if (boat == 'Y' || boat == 'y')
              {
                  // decrement uses by 1, and change the boat to bridge
                  agent.UpdateInventory("Boat", uses_left - 1);
                  // don't change tile, stay on top of water
                  if (uses_left - 1 == 0)
                  {
                      // they're on the water, and they no longer have a boat
                      std::cout << "No boat uses left! Try again? Y/N:" << std::endl;
                      char again;
                      std::cin >> again;
                      if (again == 'N')
                      {
                          exit(0);
                      }
                      else
                      {
                          agent.SetPosition(40, 3);
                          return true;
                      }
                  }
              }
          }
          else
          {
              // can't float without a boat
              return false;
          }
      }

      // if player agent walks over an item, pick it up, add it to their properties, and remove from the entity set
      if (agent.IsInterface())
      {
        for (size_t i = 0; i < GetNumItems(); i++)
        {
          auto const & entity = GetItem(i);
          if (entity.GetPosition() == new_position)
          {
            std::string uses_property = "";
            if (entity.GetName() == "Stick")   { uses_property = "Hit"; }
            if (entity.GetName() == "Sword")   { uses_property = "Hit"; }
            if (entity.GetName() == "Boat")    { uses_property = "Swim"; }
            if (entity.GetName() == "Axe") { uses_property = "Chop"; }

            std::cout << "Picked up the " << entity.GetName() << "!\n"
                      << "This item has " << entity.GetProperty<int>(uses_property) << " uses left." << std::endl;

            // add it to their inventory
            agent.UpdateInventory(entity.GetName(), entity.GetProperty<int>(uses_property));

            // remove it from the board
            RemoveEntity(entity.GetName());
          }
        }
      }
      // Set the agent to its new postion.
      agent.SetPosition(new_position);

      return true;
    }

  };

} // End of namespace cse491
