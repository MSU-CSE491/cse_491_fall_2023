/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/PacingAgent.hpp"
#include "Interfaces/MainInterface.hpp"
#include "Worlds/SecondWorld.hpp"

size_t level_zero = 0;
size_t level_one = 1;
size_t level_two = 2;

int main()
{
  group4::SecondWorld world;
  world.AddAgent<cse491::PacingAgent>("Pacer 1")
      .SetPosition(3, 1)
      .SetProperty("Health", 30.0)
      .SetProperty("Strength", 10.0)
      .SetProperty("Defense", 5.0);
  world.AddAgent<cse491::PacingAgent>("Pacer 2")
      .SetPosition(6, 1)
      .SetProperty("Health", 30.0)
      .SetProperty("Strength", 8.0)
      .SetProperty("Defense", 4.0);

  world.Run();
}
