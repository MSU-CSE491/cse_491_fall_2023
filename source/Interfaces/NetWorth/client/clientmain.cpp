/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A client-side networking main to prototype communication
 * @note Status: PROPOSAL
 **/

// // Include the modules that we will be using.
#include "../../TrashInterface.hpp"
#include "../../../Agents/PacingAgent.hpp"
#include "../../../Worlds/MazeWorld.hpp"
#include "networkingagent.hpp"

int main()
{
  /**Connect to server using arguments provided (argc/argv)
   * Once connection is established, the server will create an agent
   * for the client to control, returning it through the initial response
   * **/
  cse491::MazeWorld world;
  world.AddAgent<cse491::netWorth::NetworkingAgent>("Pacer 1").SetPosition(3,1);
  world.AddAgent<cse491::netWorth::NetworkingAgent>("Pacer 2").SetPosition(6,1);
  world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("char", '@');

  world.Run();
}
