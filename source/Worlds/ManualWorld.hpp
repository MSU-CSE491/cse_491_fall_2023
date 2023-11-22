/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A World that consists of trees, water, and grass cells.
 * @note Status: PROPOSAL
 **/

/// REVIEW UPDATES
/// * Many functions that used to take item or agent pointers now take references!

#pragma once

#include <cassert>
#include <stdlib.h>
#include <map>
#include <tuple>

#include "../core/WorldBase.hpp"

namespace cse491_team8 {

  class ManualWorld : public cse491::WorldBase {
  protected:
    enum ActionType { REMAIN_STILL=0, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, ATTACK, HEAL, STATS, USE_AXE, USE_BOAT };
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
      agent.AddAction("heal", HEAL);
      agent.AddAction("stats", STATS);
      agent.AddAction("use_axe", USE_AXE);
      agent.AddAction("use_boat", USE_BOAT);
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

    /// @brief Generates move sets for all the agents
    /// Sets the move sets as a property for each agent
    /// @return None
    void GenerateMoveSets()
    {
        for (auto & [id, agent_ptr] : agent_map)
        {
            auto agent_strength = agent_ptr->GetProperty<int>("Strength");
            if (agent_ptr->GetName() == "Interface")
            {
                std::map<std::string, std::tuple<char, double>> move_set = {
                    {"Attack", std::make_tuple('d', 1.0)},
                    {"Special", std::make_tuple('d', 1.5)}, {"Run", std::make_tuple('d', 0.0)},
                    {"Heal", std::make_tuple('h', 0.25)}};
                // agent_ptr->SetProperty<std::map<std::string, std::tuple<char, double>>>("MoveSet", move_set);
                agent_ptr->SetProperty("MoveSet", move_set);
                continue;
            }
            std::map<std::string, std::tuple<char, double>> move_set = {{"Attack", std::make_tuple('d', 1.0)}};
            if (agent_strength >= 10)
            {
                move_set["Special"] = std::make_tuple('d', 1.5);
            }
            if (agent_strength >= 15)
            {
                move_set["Heal"] = std::make_tuple('h', 0.25);
            }
            if (agent_strength >= 20)
            {
                move_set["Hyper"] = std::make_tuple('d', 2.0);
            }
            if (agent_strength >= 25)
            {
                move_set["De-Buff"] = std::make_tuple('s', -0.5);
            }
            if (agent_strength >= 30)
            {
                move_set["Buff"] = std::make_tuple('s', 0.5);
            }
            agent_ptr->SetProperty<std::map<std::string, std::tuple<char, double>>>("MoveSet", move_set);
        }
    }

    /// @brief adds a move to the move set for an agent
    /// @param agent the agent to add the move to
    /// @param move the name of the move to add
    /// @param stat the stat that the move affects
    /// @param modification the modification percent of the move to that stat
    /// Adds a move to the move set map
    /// @return None
    void AddMove(cse491::AgentBase& agent, std::string& move, char stat, double modification)
    {
      if (!agent.HasProperty("MoveSet")) {
        agent.Notify("Error: Agent does not have a Move Set\n");
        return;
      }
      else
      {
        std::map<std::string, std::tuple<char, double>> move_set = agent.GetProperty<std::map<std::string, std::tuple<char, double>>>("MoveSet");
        move_set[move] = std::make_tuple(stat, modification);
        agent.SetProperty<std::map<std::string, std::tuple<char, double>>>("MoveSet", move_set);
      }
    }
    /// @brief removes a move from the move set for an agent
    /// @param agent the agent to remove a move from
    /// @param move the name of the move to remove
    /// Removes a move from the move set map
    /// @return true for success, false for failure
    bool RemoveMove(cse491::AgentBase& agent, std::string& move)
    {
      if (!agent.HasProperty("MoveSet")) {
        agent.Notify("Error: Agent does not have a Move Set\n");
        return false;
      }
      else
      {
        std::map<std::string, std::tuple<char, double>> move_set = agent.GetProperty<std::map<std::string, std::tuple<char, double>>>("MoveSet");
        move_set.erase(move);
        agent.SetProperty<std::map<std::string, std::tuple<char, double>>>("MoveSet", move_set);
        return true;
      }
    }

    /// @brief Heals an Agent
    /// @param agent The Agent getting healed
    /// Calculates the healing of the agent
    /// @return None
    void HealAction(cse491::AgentBase & agent)
    {
      size_t can_heal = FindItem(agent, "Health Potion");
      if (can_heal != SIZE_MAX) {
        int healing_req = agent.GetProperty<int>("Max_Health") - agent.GetProperty<int>("Health");
        int healing = item_map[can_heal]->GetProperty<int>("Healing");
        if (healing_req >= healing) {
          agent.Notify("You healed " + std::to_string(healing) + " health!\n");
          agent.SetProperty("Health", agent.GetProperty<int>("Health") + healing);
          RemoveItem(can_heal);
        } 
        else if (healing_req == 0) {
          agent.Notify("You already have max health");
        } else {
          agent.Notify("You healed " + std::to_string(healing_req) + " health!\n");
          agent.SetProperty("Health", agent.GetProperty<int>("Health") + healing_req);
          item_map[can_heal]->SetProperty("Healing", healing - healing_req);
        }
      }

    }

    void StatsAction(cse491::AgentBase & agent)
    {
        agent.Notify("Items owned by the player:");
        for (auto & item : item_map)
        {
            if (item.second->IsOwnedBy(agent.GetID()))
            {
                int result = 0;
                if (item.second->HasProperty("Uses"))
                {
                    result = item.second->GetProperty<int>("Uses");
                }
                else if (item.second->HasProperty("Strength"))
                {
                    result = item.second->GetProperty<int>("Strength");
                }
                else if (item.second->HasProperty("Healing"))
                {
                    result = item.second->GetProperty<int>("Healing");
                }
                agent.Notify(item.second->GetName() + ": " + std::to_string(result));
            }
        }
        agent.Notify("\nProperties of the player:");
        agent.Notify("Strength: " + std::to_string(agent.GetProperty<int>("Strength")));
        agent.Notify("Health: " + std::to_string(agent.GetProperty<int>("Health")));
        agent.Notify("Max Health: " + std::to_string(agent.GetProperty<int>("Max_Health")));
    }

    /// @brief Determines the damage of the other agent
    /// @param other_agent The NPC agent
    /// @param agent The player agent
    /// Gets the damage of the NPC agent
    /// @return The damage of the NPC
    int OtherAction(cse491::AgentBase & other_agent, cse491::AgentBase & agent)
    {
        int other_damage = 0;
        std::map<std::string, std::tuple<char, double>> move_set = other_agent.GetProperty<std::map<std::string, std::tuple<char, double>>>("MoveSet");
        auto iterator = move_set.begin();
        int random = rand() % move_set.size();
        std::advance(iterator, random);
        std::string random_key = iterator->first;
        agent.Notify("\nThe enemy has used: " + random_key);
        std::tuple<char, double> move_info = iterator->second;
        char stat_char = std::get<0>(move_info);
        double stat_modification = std::get<1>(move_info);
        if (stat_char == 'd') {
            other_damage = static_cast<int>(other_agent.GetProperty<int>("Strength") * stat_modification);
        }
        if (stat_char == 'h') {
            HealAction(other_agent);
        }
        if (stat_char == 's') {
          if (stat_modification < 0) {
            int agent_strength = agent.GetProperty<int>("Strength");
            int new_strength = static_cast<int>(agent_strength - abs(stat_modification) * agent_strength);
            agent.SetProperty<int>("Strength", new_strength);
          }
          else {
            int other_agent_strength = other_agent.GetProperty<int>("Strength");
            int new_strength_other = static_cast<int>(other_agent_strength + abs(stat_modification) * other_agent_strength);
            other_agent.SetProperty<int>("Strength", new_strength_other);
          }
        }
        return other_damage;
    }

    /// @brief Checks the strength between two agents
    /// @param other_agent The autonomous agent to compare
    /// @param agent The interface (player) agent to compare
    /// Prints the stronger agent and removes the weaker
    /// @see RemoveAgent
    /// @return None
    void StrengthCheck(cse491::AgentBase & other_agent, cse491::AgentBase & agent) {
        // User Input for Player Decision
        bool won = false;
        bool run = false;
        char input;
        while (other_agent.GetProperty<int>("Health") > 0 && agent.GetProperty<int>("Health") > 0)
        {
          bool valid_input = true;
          agent.Notify("Player Health: " + std::to_string(agent.GetProperty<int>("Health"))+"\n"+
                       "Player Strength: " + std::to_string(agent.GetProperty<int>("Strength"))+"\n"+
                       "Enemy Health: " + std::to_string(other_agent.GetProperty<int>("Health"))+"\n"+
                       "Enemy Strength: " + std::to_string(other_agent.GetProperty<int>("Strength"))+"\n"+
                       "\na for attack, s for special, r for run, h for heal\nPlayer Attack: ");
          std::cin >> input;
          int damage = 0;
          switch (input) {
          case 'a': case 'A': damage = agent.GetProperty<int>("Strength");    break;
          case 's': case 'S': damage = static_cast<int>(agent.GetProperty<int>("Strength") * 1.5);  break;
          case 'r': case 'R': won = false; run = true; break;
          case 'h': case 'H': HealAction(agent); break;
          default: valid_input = false; break;
          }
          if (!valid_input)
          {
            agent.Notify("\nInvalid Input\n");
            continue;
          }

          // Other Agent Move Choice, will modify strength of player agent or NPC agent based on the move selected
          int other_damage = OtherAction(other_agent, agent);

          // Process the Player's Move and the Agent's Move
          if (run)
          {
            agent.SetProperty<int>("Health", agent.GetProperty<int>("Health") - other_damage);
            break;
          }
          other_agent.SetProperty<int>("Health", other_agent.GetProperty<int>("Health") - damage);
          if (other_agent.GetProperty<int>("Health") <= 0)
          {
            won = true;
            break;
          }
          agent.SetProperty<int>("Health", agent.GetProperty<int>("Health") - other_damage);
          if (agent.GetProperty<int>("Health") <= 0)
          {
            break;
          }
        }

        std::string other_agent_name = other_agent.GetName();

        if (!won) {
          if (run)
          {
            agent.Notify("You ran away, this means you don't gain health or strength and any battle damage stays!\n");
          }
          if (agent.GetName() == "Interface" && agent.GetProperty<int>("Health") <= 0)
          {
            agent.Notify(other_agent_name + " has beat " + agent.GetName());
            agent.Notify("You Lost...\n");
            while (true)
            {
              agent.Notify("Would You Like To Continue? Y or N? ");
              char repeat_input;
              std::cin >> repeat_input;
              if (repeat_input == 'N' || repeat_input == 'n')
              {
                run_over = true;
                break;
              }
              else if (repeat_input == 'Y' || repeat_input == 'y')
              {
                agent.SetProperty<int>("Strength", 15);
                agent.SetProperty<int>("Health", 15);
                agent.SetProperty<int>("Direction", 0);
                agent.SetPosition(40, 3);
                break;
              }
              else
              {
                agent.Notify("Invalid Input!\n");
                continue;
              }
            }
          }
          // this->RemoveAgent(agent.GetName()); // If the agent is the interface, this does nothing
        }
        else
        {
          agent.Notify(agent.GetName() + " has beat " + other_agent.GetName());
          // Gain the Agent's strength that you beat
          agent.SetProperty<int>("Strength",
                  agent.GetProperty<int>("Strength") + other_agent.GetProperty<int>("Strength"));
          cse491::GridPosition other_position = other_agent.GetPosition();
          this->RemoveAgent(other_agent.GetName());

          // get the loot dropped from the agent
          int random = rand() % 10;
          char symbol = ' ';
          std::string loot = "";
          std::string action = "";
          int num_actions = 0;
          if (random <= 4) {
            symbol = '/';
            loot = "Stick";
            action = "Strength";
            num_actions = 2;
          }
          else if (random <= 7) {
            symbol = 'U';
            loot = "Boat";
            action = "Uses";
            num_actions = 10;
          }
          else if (random <= 9) {
            symbol = 'P';
            loot = "Axe";
            action = "Uses";
            num_actions = 5;
          }
          else {
            symbol = '!';
            loot = "Sword";
            action = "Strength";
            num_actions = 5;
          }

          // put the loot where the agent was
          double x = other_position.GetX();
          double y = other_position.GetY();
          this->AddItem(loot, "symbol", symbol).SetPosition(x, y);

          // set the entity's _action_ property to have _num_action_ uses
          for (const auto& [id, entity] : item_map)
          {
            if (entity->GetPosition() == other_position)
            {
              entity->SetProperty<int>(action, num_actions);
              break;
            }
          }

          agent.Notify(other_agent_name + " dropped their " + loot + "!\n");
        }
    }
   
    /// @brief Checks for agents adjacent to the given agent
    /// Checks the strengths if an agent is found
    /// @param agent The agent to look around
    /// @see StrengthCheck
    /// @return true if an agent was found, else false
    bool CheckAround(cse491::AgentBase & agent)
    {
      auto new_position = agent.GetPosition();
      for (auto & [id, agent2_ptr] : agent_map)
      {
        if (agent.GetID() != agent2_ptr->GetID()) {
          if (agent2_ptr->GetPosition() == new_position.Above() ||
              agent2_ptr->GetPosition() == new_position.Below() ||
              agent2_ptr->GetPosition() == new_position.ToLeft() ||
              agent2_ptr->GetPosition() == new_position.ToRight() ||
              agent2_ptr->GetPosition() == new_position)
        {
          StrengthCheck(*agent2_ptr, agent);
          return true;
        }
        }
      }
      return false;
    }


    /// Finds an interface agent and calls CheckAround
    void HandleNeighbors()
    {
      for (const auto& [id, agent_ptr] : agent_map)
      {
        if (agent_ptr->IsInterface())
        {
          CheckAround(*agent_ptr);
          break;
        }
      }
    }

    /// @brief Looks for adjacencies
    void UpdateWorld() override {
      HandleNeighbors();
    }

    /// Runs agents, updates the world.
    void Run() override
    {
      run_over = false;
      while (!run_over) {
        RunAgents();
        UpdateWorld();
      }
    }

    /// @brief Attempt to pick up an item for the agent.
    /// @param agent The agent that is picking up the item.
    /// @param new_position New position of the agent to check if an item is there.
    /// @return Nothing
    void DoActionAttemptItemPickup(cse491::AgentBase & agent, const cse491::GridPosition & new_position) {
      for (const auto & [id, item_ptr] : item_map) {
        if (item_ptr->GetPosition() == new_position)
        {
          std::string uses_property = "";
          if (item_ptr->GetName() == "Stick" || item_ptr->GetName() == "Sword") { uses_property = "Strength"; }
          if (item_ptr->GetName() == "Boat" || item_ptr->GetName() == "Axe")  { uses_property = "Uses"; }
          if (item_ptr->GetName() == "Health Potion") { uses_property = "Healing"; }

          if (uses_property == "Strength")
          {
            if (agent.HasProperty(uses_property))
            {
              agent.SetProperty(uses_property, item_ptr->GetProperty<int>(uses_property) + agent.GetProperty<int>(uses_property));
            }
            else
            {
              agent.SetProperty(uses_property, item_ptr->GetProperty<int>(uses_property));
            }
          }

          agent.Notify("Picked up the " + item_ptr->GetName() + "!\nYou gained " +
                        std::to_string(item_ptr->GetProperty<int>(uses_property)) + " " +
                        uses_property + "!\n");

          // remove it from the board
          item_ptr->SetOwner(agent);
          // RemoveItem(item_ptr->GetID());

          break;
        }
      }
    }


    /// @brief Updates agent's position and direction
    /// @param agent The agent that is moving its position
    /// @param action_id A size_t representing the direction the agent moved
    /// @return The agent's new position
    cse491::GridPosition DoActionFindNewPosition(cse491::AgentBase& agent, size_t action_id) {
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
        case ATTACK:
        {
            new_position = agent.GetPosition();
            break;
        }
        case HEAL:
        {
            new_position = agent.GetPosition();
            HealAction(agent);
            break;
        }
        case STATS:
        {
            new_position = agent.GetPosition();
            StatsAction(agent);
            break;
        }
        case USE_AXE:
        {
          new_position = agent.GetPosition();
          agent.Notify("Using an Axe");
          break;
        }
        case USE_BOAT:
        {
          new_position = agent.GetPosition();
          agent.Notify("Using an Boat");
          break;
        }
      }

      // assume new position is valid
      return new_position;
    
    }

      /// @brief Check if an agent owns an item
      /// @param agent The agent to see if is an owner
      /// @param item_name Name of the item
      /// @return item_id
    size_t FindItem(cse491::AgentBase & agent, const std::string & item_name) {
      size_t item_id = SIZE_MAX;
      for (auto & item : item_map)
      {
        if (item.second->GetName() == item_name && item.second->IsOwnedBy(agent.GetID()))
        {
          item_id = item.second->GetID();
          break;
        }
      }
      return item_id;
    }

    /// @brief Attempt to interact with a tree
    /// If the agent can interact with the tree, prompts the user if they want to use one of their chops
    /// @param agent The agent trying to interact
    /// @param new_position The position being interacted with
    /// @return Nothing, the tree gets chopped if possible but the agent doesn't move
    void DoActionTestNewPositionTree(cse491::AgentBase & agent, const cse491::GridPosition & new_position) {
        size_t item_id = FindItem(agent, "Axe");
        // if (agent.HasProperty("Uses") && agent.GetProperty<int>("Uses") > 0)
        if (item_id != SIZE_MAX)
        {
          agent.Notify("You can use your Axe once to chop down this tree. You have " +
                        std::to_string(item_map[item_id]->GetProperty<int>("Uses")) + " uses remaining. Chop this tree? Y/N:");
          char chop;
          std::cin >> chop;
          if (chop == 'Y' || chop == 'y')
          {
            // decrement uses by 1, change the tree to grass, but don't move the agent's position
            item_map[item_id]->SetProperty("Uses", item_map[item_id]->GetProperty<int>("Uses") - 1);
                if (item_map[item_id]->GetProperty<int>("Uses") == 0) {
                  RemoveItem(item_id);
                }
            main_grid[new_position] = grass_id;
          }
        }
    }


    /// @brief Attempt to float on a water tile
    /// If the agent has a boat, prompts the user if they want to use the boat once
    /// @param agent The agent trying to interact
    /// @return True if the agent is able to (and chooses) to move to the new spot, else false
    bool DoActionTestNewPositionWater(cse491::AgentBase& agent) {
        if (agent.HasProperty("OnlyWater"))
        {
            return true;
        }

        size_t item_id = FindItem(agent, "Boat");

        if (item_id != SIZE_MAX)
        {
            agent.Notify("You can use your Boat once to float over this tile. You have "
                + std::to_string(item_map[item_id]->GetProperty<int>("Uses")) + " uses remaining. Use your boat? Y/N:\n");
            char boat;
            std::cin >> boat;
            if (boat == 'Y' || boat == 'y')
            {
                item_map[item_id]->SetProperty("Uses", item_map[item_id]->GetProperty<int>("Uses") - 1);
                if (item_map[item_id]->GetProperty<int>("Uses") == 0) {
                    RemoveItem(item_id);
                    
                    // they're on the water, and they no longer have a boat
                    agent.Notify("No boat uses left! Try again? Y/N:\n");
                    char again;
                    std::cin >> again;
                    if (again == 'N' || again == 'n')
                    {
                        run_over = true;
                    }
                    else
                    {
                        // reset the interface to starting position, don't update
                        // with the new position back in the calling function
                        agent.SetPosition(40, 3);
                    }
                    return false;
                }
                
                // still have boat uses left, used the boat so position is valid
                return true;
            }
        }
        return false;
    }

    /// Allow the agents to move around the maze.
    int DoAction(cse491::AgentBase & agent, size_t action_id) override {
      
      cse491::GridPosition new_position = DoActionFindNewPosition(agent, action_id);

      // Don't let the agent move off the world or into a wall.
      if (!main_grid.IsValid(new_position)) { return false; }

      // TK we might later want to let NPCs pick up items
      if (agent.IsInterface())
      {
        DoActionAttemptItemPickup(agent, new_position);
      }

      if (main_grid.At(new_position) == tree_id)
      {
          // chop the tree if possible, but don't move the agent either way
          DoActionTestNewPositionTree(agent, new_position);
          return false;
      }

      if (main_grid.At(new_position) == water_id)
      {
          bool moved = DoActionTestNewPositionWater(agent);
          
          // if they didn't move onto the water tile, we don't update their position
          // return false from here
          if (not moved) { return false; }
      }

      if (main_grid.At(new_position) == grass_id)
      {
        if (agent.HasProperty("OnlyWater"))
        {
          return false;
        }
      }

      if (main_grid.At(new_position) == rock_id)
      {
        return false;
      }

      // Set the agent to its new postion.
      agent.SetPosition(new_position);

      return true;
    }

  };

} // End of namespace cse491
