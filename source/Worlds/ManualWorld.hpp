/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A World that consists of trees, water, and grass cells.
 * @note Status: PROPOSAL
 * @author Evan Orvis
 * @author Grant Carr
 * @author Lauren Garvey
 * @author Nate Gu
 **/

/// REVIEW UPDATES
/// * Many functions that used to take item or agent pointers now take references!

#pragma once

#include <cassert>
#include <stdlib.h>
#include <map>
#include <tuple>

#include "../core/WorldBase.hpp"
#include "../core/AgentBase.hpp"

namespace cse491_team8 {

  class ManualWorld : public cse491::WorldBase {
  protected:
    enum ActionType { REMAIN_STILL=0, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, USE_AXE, USE_BOAT, STATS, HEAL, RUN,
            ATTACK, SPECIAL, BUFF, HELP };
    enum FacingDirection { UP=0, RIGHT, DOWN, LEFT};

    size_t grass_id;  ///< Easy access to floor CellType ID.
    size_t tree_id;   ///< Easy access to tree CellType ID.
    size_t water_id;  ///< Easy access to water CellType ID.
    size_t bridge_id; ///< Easy access to bridge CellType ID.
    size_t rock_id;   ///< Easy access to rock CellType ID.
    size_t portal_id_a; ///< Easy access to first portal CellType ID.
    size_t portal_id_b; ///< Easy access to second portal CellType ID.
    size_t portal_id_c; ///< Easy access to third portal CellType ID.
    size_t portal_id_d; ///< Easy access to fourth portal CellType ID.

    /// Provide the agent with movement actions.
    void ConfigAgent(cse491::AgentBase & agent) override {
      agent.AddAction("up", MOVE_UP);
      agent.AddAction("down", MOVE_DOWN);
      agent.AddAction("left", MOVE_LEFT);
      agent.AddAction("right", MOVE_RIGHT);
      agent.AddAction("use_axe", USE_AXE);
      agent.AddAction("use_boat", USE_BOAT);
      agent.AddAction("stats", STATS);
      agent.AddAction("heal", HEAL);
      agent.AddAction("run", RUN);
      agent.AddAction("attack", ATTACK);
      agent.AddAction("special", SPECIAL);
      agent.AddAction("buff", BUFF);
      agent.AddAction("help", HELP);
      agent.SetProperties("Strength", 10, "Health", 100, "Max_Health", 150, "Direction", 0, "Battling", false);
    }

  public:
    ManualWorld() {
      grass_id = AddCellType("grass", "Grass that you can easily walk over.", ' ');
      tree_id = AddCellType("tree", "Tree that you cannot pass without an axe.", '^');
      water_id = AddCellType("water", "Water that cannot be crossed without a boat.", '~');
      bridge_id = AddCellType("bridge", "Bridge that allows the playerto cross water.", '#');
      rock_id = AddCellType("rock", "Rock that the player cannot cross", '$');
      portal_id_a = AddCellType("portal_a", "Portal that teleports player to another a-portal spot.", '}');
      portal_id_b = AddCellType("portal_b", "Portal that teleports player to another b-portal spot.", '{');
      portal_id_c = AddCellType("portal_c", "Portal that teleports player to another c-portal spot.", '(');
      portal_id_d = AddCellType("portal_d", "Portal that teleports player to another d-portal spot.", ')');
      main_grid.Read("../assets/grids/team8_grid_v2.grid", type_options);
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
            move_set["Heal"] = std::make_tuple('h', 0);
          }
          if (agent_strength >= 20)
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
          agent.RemoveItem(can_heal);
          RemoveItem(can_heal);
        }
        else if (healing_req == 0) {
          agent.Notify("You already have max health");
        } else {
          agent.Notify("You healed " + std::to_string(healing_req) + " health!\n");
          agent.SetProperty("Health", agent.GetProperty<int>("Health") + healing_req);
          item_map[can_heal]->SetProperty("Healing", healing - healing_req);
        }
      } else {
        agent.Notify("You do not have any health potions!\n");
      }

    }

    /// @brief Displays the items and properties that the player has
    /// @param agent Agent stats being showed.
    void StatsAction(cse491::AgentBase & agent)
    {
        std::string output = "";
        output += "Items owned by the player:\n";
        for (const auto & [item_id, item] : item_map)
        {
            if (item->IsOwnedBy(agent.GetID()))
            {
                for (const auto & [name, entity] : item->GetProprtyMap())
                {
                    if (name == "Uses" || name == "Strength" || name == "Healing")
                    {
                        size_t value = item->GetProperty<int>(name);
                        output += item->GetName() + ": " + name + ": " + std::to_string(value) + "\n";
                    }
                }
            }
        }
        output += "\nProperties of the player:\n";
        for (const auto & [name, entity] : agent.GetProprtyMap())
        {
            if (name == "Strength" || name == "Health" || name == "Max_Health")
            {
                size_t value = agent.GetProperty<int>(name);
                output += name + ": " + std::to_string(value) + "\n";
            }
        }
        agent.Notify(output);
    }

    /// @brief Displays the moveset for the agent.
    /// @param agent Agent moveset being displayed.
    void MoveSetAction(cse491::AgentBase & agent)
    {
        agent.Notify("Your Moveset is:\nMove Up: W\nMove Down: S\nMove Left: A\nMove Right: D\nUse Axe: C\nUse Boat: V\nDisplay Stats: T\nHeal: H\nAttack: F\nSpecial: G\nRun: R\nBuff: B\nDebuff: X\nDisplay Moveset: Y");
    }

    /// @brief looks one tile ahead of the agent based on facing direction
    /// @param agent Agent that is looking ahead
    /// @return the grid position
    cse491::GridPosition LookAhead(cse491::AgentBase & agent)
    {
        size_t direction = agent.GetProperty<int>("Direction");
        cse491::GridPosition look_position;

        switch (direction)
        {
        case (0):
          look_position = agent.GetPosition().Above();
          break;
        case (1):
          look_position = agent.GetPosition().ToRight();
          break;
        case (2):
          look_position = agent.GetPosition().Below();
          break;
        case (3):
          look_position = agent.GetPosition().ToLeft();
          break;

        default:
          agent.Notify("Invalid Position: Returning Current Position");
          look_position = agent.GetPosition();
          break;
        }

        return look_position;
    }

    /// @brief Removes all items from other agent
    /// @param agent Agent that won the battle
    /// @param other_agent Agent that is dropping items
    void DropItems(cse491::AgentBase & agent, cse491::AgentBase & other_agent)
    {
        for (const auto& [id, item] : item_map)
        {
            if (item->IsOwnedBy(other_agent.GetID()))
            {
                if (item->HasProperty("Strength") && other_agent.HasProperty("Strength"))
                {
                    auto agent_health = other_agent.GetProperty<int>("Strength");
                    auto item_strength = item->GetProperty<int>("Strength");
                    other_agent.SetProperty<int>("Strength", (int)(agent_health - item_strength));
                }
                other_agent.RemoveItem(item->GetID());
                item->SetPosition(other_agent.GetPosition());
                agent.Notify(other_agent.GetName() + " dropped their " + item->GetName() + "!");
            }
        }
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
        int random = GetRandom(move_set.size());
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
    /// @param attack_type The attack that is being used by the player
    /// Prints the stronger agent and removes the weaker
    /// @see RemoveAgent
    /// @return None
    void DoBattle(cse491::AgentBase & other_agent, cse491::AgentBase & agent, char attack_type) {
        // User Input for Player Decision
        bool won = false;
        bool run = false;
        int damage = 0;
        switch (attack_type) {
        case 'a': case 'A': damage = agent.GetProperty<int>("Strength");    break;
        case 's': case 'S': damage = static_cast<int>(agent.GetProperty<int>("Strength") * 1.5);  break;
        case 'r': case 'R': won = false; run = true; break;
        case 'b': case 'B': agent.SetProperty<int>("Strength", static_cast<int>(1.5 * agent.GetProperty<int>("Strength"))); break;
        case 'h': case 'H': HealAction(agent); break;
        default: break;
        }

        // Other Agent Move Choice, will modify strength of player agent or NPC agent based on the move selected
        int other_damage = OtherAction(other_agent, agent);

        // Process the Player's Move and the Agent's Move
        if (run)
        {
            agent.SetProperty<int>("Health", agent.GetProperty<int>("Health") - other_damage);
        }
        else
        {
            other_agent.SetProperty<int>("Health", other_agent.GetProperty<int>("Health") - damage);
            if (other_agent.GetProperty<int>("Health") <= 0)
            {
                won = true;
                other_damage = 0;
            }
            agent.SetProperty<int>("Health", agent.GetProperty<int>("Health") - other_damage);
            if (agent.GetProperty<int>("Health") <= 0)
            {
                won = false;
                // run_over = true;
            }
        }

        agent.Notify("Player Health: " + std::to_string(agent.GetProperty<int>("Health"))+"\n"+
                "Player Strength: " + std::to_string(agent.GetProperty<int>("Strength"))+"\n"+
                "Enemy Health: " + std::to_string(other_agent.GetProperty<int>("Health"))+"\n"+
                "Enemy Strength: " + std::to_string(other_agent.GetProperty<int>("Strength")));

        std::string other_agent_name = other_agent.GetName();
        DataCollection::DataManager::GetInstance().GetAgentInteractionCollector().RecordInteraction(other_agent_name);

        if (!won) {
          if (run)
          {
            agent.Notify("You ran away, this means you don't gain health or strength and any battle damage stays!\n");
          }
          std::cout << agent.GetName()  << ": " <<  agent.GetProperty<int>("Health") << std::endl;
          if (agent.IsInterface() && agent.GetProperty<int>("Health") <= 0)
          {
            agent.Notify(other_agent_name + " has beat " + agent.GetName() + "\nYou Lost...\n");

            DropItems(agent, agent);

            agent.SetProperty<int>("Health", 100);
            agent.SetProperty<int>("Direction", 0);

            agent.SetProperty<bool>("Battling", false);
            other_agent.SetProperty<bool>("Battling", false);

            agent.SetPosition(80, 63);
            agent.Notify("You Have died and dropped all your items");
          }
        }
        else
        {
          agent.Notify(agent.GetName() + " has beat " + other_agent.GetName());
          agent.SetProperty<bool>("Battling", false);
          other_agent.SetProperty<bool>("Battling", false);
          DropItems(agent, other_agent);
          other_agent.SetProperty<bool>("deleted", true);
        }
    }

    /// @brief Looks for adjacencies
    void UpdateWorld() override
    {

    }

    /// @brief Runs agents, updates the world.
    void Run() override
    {
      run_over = false;
      while (!run_over) {
        RunAgents();
        CollectData();
        UpdateWorld();
      }
    }

    /// @brief Step through each agent giving them a chance to take an action.
    void RunAgents() override {
      for (auto & [id, agent_ptr] : agent_map) {
        if (agent_ptr->HasProperty("deleted")) {
          continue;
        }
        size_t action_id = agent_ptr->SelectAction(main_grid, type_options, item_map, agent_map);
        agent_ptr->storeActionMap(agent_ptr->GetName());
        int result = DoAction(*agent_ptr, action_id);
        agent_ptr->SetActionResult(result);
      }
    }

    /// @brief Attempt to pick up an item for the agent.
    /// @param agent The agent that is picking up the item.
    /// @param new_position New position of the agent to check if an item is there.
    /// @return Nothing
    void DoActionAttemptItemPickup(cse491::AgentBase & agent, const cse491::GridPosition & new_position) {
      for (const auto & [id, item_ptr] : item_map) {
        if (item_ptr->GetPosition() == new_position && item_ptr->GetOwnerID() == 0)
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

          DataCollection::DataManager::GetInstance().GetItemUseCollector().IncrementItemUsage(item_ptr->GetName());
          agent.Notify("Picked up the " + item_ptr->GetName() + "!\nYou gained " +
                        std::to_string(item_ptr->GetProperty<int>(uses_property)) + " " +
                        uses_property + "!\n");

          // remove it from the board
          agent.AddItem(item_ptr->GetID());
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
        cse491::GridPosition new_position, look_position;
        char move = ' ';

        bool battling = agent.GetProperty<bool>("Battling");
        // Update Direction property and get new position.
        switch (action_id) {
        case REMAIN_STILL:
        {
            new_position = agent.GetPosition();
            break;
        }
        case MOVE_UP:
        {
            if (battling)
            {
                new_position = agent.GetPosition();
            }
            else
            {
                agent.SetProperty<int>("Direction", UP);
                new_position = agent.GetPosition().Above();
            }
            break;
        }
        case MOVE_DOWN:
        {
            if (battling)
            {
                new_position = agent.GetPosition();
            }
            else
            {
                agent.SetProperty<int>("Direction", DOWN);
                new_position = agent.GetPosition().Below();
            }
            break;
        }
        case MOVE_LEFT:
        {
            if (battling)
            {
                new_position = agent.GetPosition();
            }
            else
            {
                agent.SetProperty<int>("Direction", LEFT);
                new_position = agent.GetPosition().ToLeft();
            }
            break;
        }
        case MOVE_RIGHT:
        {
            if (battling)
            {
                new_position = agent.GetPosition();
            }
            else
            {
                agent.SetProperty<int>("Direction", RIGHT);
                new_position = agent.GetPosition().ToRight();
            }
            break;
        }
        case USE_AXE:
        {
            new_position = agent.GetPosition();
            look_position = LookAhead(agent);
            if (main_grid.IsValid(look_position) && main_grid.At(look_position) == tree_id)
            {
                DoActionTestNewPositionTree(agent, look_position);
            } else {
                agent.Notify("You can not use an Axe here!");
            }
            break;
        }
        case USE_BOAT:
        {
            new_position = agent.GetPosition();
            if (main_grid.At(new_position) == water_id)
            {
              agent.Notify("Already on Water");
              break;
            }
            look_position = LookAhead(agent);
            if (main_grid.IsValid(look_position) && main_grid.At(look_position) == water_id)
            {
                if (DoActionTestNewPositionWater(agent))
                {
                    new_position = look_position;
                }
            } else {
                agent.Notify("You can not use a Boat here!");
            }
            break;
        }
        case STATS:
        {
            new_position = agent.GetPosition();
            StatsAction(agent);
            break;
        }
        case HEAL:
        {
            new_position = agent.GetPosition();
            if (battling)
            {
                move = 'h';
            }
            else
            {
                HealAction(agent);
            }
            break;
        }
        case RUN:
        {
            if (!battling)
            {
                new_position = agent.GetPosition();
                break;
            }
            new_position = agent.GetPosition();
            agent.SetProperty<bool>("Battling", false);

            auto agents = FindAgentsNear(agent.GetPosition(), 1);
            for (auto agent_id : agents)
            {
                if (!agent_map[agent_id]->IsInterface() && !agent_map[agent_id]->HasProperty("deleted"))
                {
                    agent.Notify("You are running away");
                    agent_map[agent_id]->SetProperty<bool>("Battling", false);
                    DoBattle(*agent_map[agent_id], agent, 'r');
                }
            }
            break;
        }
        case ATTACK:
        {
            move = 'a';
            break;
        }
        case SPECIAL:
        {
            move = 's';
            break;
        }
        case BUFF:
        {
            move = 'b';
            break;
        }
        case HELP:
        {
            new_position = agent.GetPosition();
            MoveSetAction(agent);
            break;
        }
      }

      if (move != ' ')
      {
          if (agent.GetProperty<bool>("Battling") == false)
          {
              agent.Notify("You are in a battle! Use Y and choose battling moves!");
          }
          auto agents = FindAgentsNear(agent.GetPosition(), 1);
          for (auto agent_id : agents)
          {
              // Battle other agent near the player
              if (!agent_map[agent_id]->IsInterface() && !agent_map[agent_id]->HasProperty("deleted"))
              {
                  agent.SetProperty<bool>("Battling", true);
                  agent_map[agent_id]->SetProperty<bool>("Battling", true);
                  DoBattle(*agent_map[agent_id], agent, move);
                  break;
              }
          }
          new_position = agent.GetPosition();
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
        if (item_id != SIZE_MAX)
        {
          agent.Notify("You have used your Axe to chop down this tree. You have " +
                        std::to_string(item_map[item_id]->GetProperty<int>("Uses") - 1) + " uses remaining");

          // decrement uses by 1, change the tree to grass
          item_map[item_id]->SetProperty("Uses", item_map[item_id]->GetProperty<int>("Uses") - 1);
          if (item_map[item_id]->GetProperty<int>("Uses") == 0)
          {
            agent.RemoveItem(item_id);
            RemoveItem(item_id);
          }
          main_grid[new_position] = grass_id;
        }
    }


    /// @brief Attempt to float on a water tile
    /// If the agent has a boat, prompts the user if they want to use the boat once
    /// @param agent The agent trying to interact
    /// @return True if the agent is able to (and chooses) to move to the new spot, else false
    bool DoActionTestNewPositionWater(cse491::AgentBase& agent) {
        size_t item_id = FindItem(agent, "Boat");
        if (item_id != SIZE_MAX)
        {
            agent.Notify("You have used your Boat to float on the water. You have " +
                          std::to_string(item_map[item_id]->GetProperty<int>("Uses") - 1) + " uses remaining");

            // decrement uses by 1
            item_map[item_id]->SetProperty("Uses", item_map[item_id]->GetProperty<int>("Uses") - 1);
            if (item_map[item_id]->GetProperty<int>("Uses") == 0)
            {
                agent.RemoveItem(item_id);
                RemoveItem(item_id);
            }
            return true;
        }
        return false;
    }

    /// @brief Central function for an agent to take any action
    /// @param agent The specific agent taking the action
    /// @param action The id of the action to take
    /// @return The result of this action (usually 0/1 to indicate success)
    /// @note Thus function must be overridden in any derived world.
    int DoAction(cse491::AgentBase & agent, size_t action_id) override {

      cse491::GridPosition new_position = DoActionFindNewPosition(agent, action_id);

      // Don't let the agent move off the world or into a wall.
      if (!main_grid.IsValid(new_position)) { return false; }

      DoActionAttemptItemPickup(agent, new_position);

      if (main_grid.At(new_position) == tree_id)
      {
          return false;
      }

      if (main_grid.At(new_position) == water_id)
      {
          if (agent.HasProperty("OnlyWater"))
          {
              agent.SetPosition(new_position);
              return true;
          }
          if (action_id != USE_BOAT && main_grid.At(agent.GetPosition()) != water_id) { return false; }
      }

      if (main_grid.At(new_position) == grass_id)
      {
        if (agent.HasProperty("OnlyWater")) { return false; }
      }

      if (main_grid.At(new_position) == rock_id)
      {
        return false;
      }

      if (main_grid.At(new_position) == portal_id_a)
      {
          // which portal should we go to - for medium grid
          if (new_position.GetX() == 33)
              new_position = cse491::GridPosition(3, 1);
          else if (new_position.GetX() == 3)
              new_position = cse491::GridPosition(33, 0);

          // for large grid, big right sand patch (x=137) to circle lake island (x=41)
          else if (new_position.GetX() == 137)
              new_position = cse491::GridPosition(41, 98);
          else if (new_position.GetX() == 41)
              new_position = cse491::GridPosition(137, 40);
      }

      if (main_grid.At(new_position) == portal_id_b)
      {
          // for medium grid
          if (new_position.GetX() == 44)
              new_position = cse491::GridPosition(2, 10);
          else if (new_position.GetX() == 2)
              new_position = cse491::GridPosition(44, 17);

          // for large grid, tree-locked upper left sand patch(x=32) to lower right along water (x=114)
          else if (new_position.GetX() == 32)
              new_position = cse491::GridPosition(114, 132);
          else if (new_position.GetX() == 114)
              new_position = cse491::GridPosition(32, 36);

      }

      if (main_grid.At(new_position) == portal_id_c)
      {
          // above top right lake (x=120) to left of circle lake (x=16)
          if (new_position.GetX() == 120)
              new_position = cse491::GridPosition(16, 140);
          else if (new_position.GetX() == 16)
              new_position = cse491::GridPosition(120, 25);
      }

      if (main_grid.At(new_position) == portal_id_d)
      {
          // from circle lake island (x=36) to very bottom right corner (x=146)
          if (new_position.GetX() == 36)
              new_position = cse491::GridPosition(146, 147);
          else if (new_position.GetX() == 146)
              new_position = cse491::GridPosition(36, 94);
      }

      // Set the agent to its new postion.
      agent.SetPosition(new_position);

      return true;
    }

    /// @brief Determine if this tile can be walked on, defaults to every tile is walkable
    /// @param pos The grid position we are checking
    /// @return If an agent should be allowed on this square
    [[nodiscard]] bool IsTraversable(const cse491::AgentBase & /*agent*/, cse491::GridPosition pos) const override {
      return main_grid.At(pos) == grass_id;
    }

  };

} // End of namespace cse491_team8
