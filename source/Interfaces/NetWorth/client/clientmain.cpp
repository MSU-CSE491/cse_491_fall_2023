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

int main(int argc, char *argv[])
{
  /**Connect to server using arguments provided (argc/argv)
   * Once connection is established, the server will create an agent
   * for the client to control, returning it through the initial response
   * **/
  if (argc != 3) {
    std::cout << "Must have an argument for server IP and port\nUsage: ./client [IP] [port]" << std::endl;
    return 1;
  }

  std::string IPstring(argv[1]);
  sf::IpAddress serverIP(IPstring);
  sf::UdpSocket socket;
  unsigned short port = stoi(std::string(argv[2]));

  std::string message = "ping!";
  if (socket.send(message.c_str(), message.size() + 1, serverIP, port) != Socket::Status::Done) {
    std::cout << "Could not connect to " << IPstring << " at port " << port << std::endl;
    return 1;
  } 
  
  sf::IpAddress sender;
  size_t received;
  char data[100];
  if (socket.receive(data, 100, received, serverIP, port) != sf::Socket::Done)
  {
      std::cout << "Failure to receive" << std::endl;
      return 1;
  }
  std::cout << data << std::endl;

  char input;
  bool valid_input = false;
  bool wait_for_input = true;
  sf::Packet recv_pkt;
  while (input != 'q')
  {
    do {
      std::cin >> input;
    } while (!std::cin && wait_for_input);

    switch (input) {
      case 'w': case 'W': valid_input = true;   break;
      case 'a': case 'A': valid_input = true;   break;
      case 's': case 'S': valid_input = true;   break;
      case 'd': case 'D': valid_input = true;   break;
      case 'q': case 'Q': valid_input = true;   break;
    }

    // If we waited for input, but don't understand it, notify the user.
    if (wait_for_input && !valid_input) {
      std::cout << "Unknown key '" << input << "'." << std::endl;
    } else {
      if (socket.send(&input, 1, serverIP, port) != Socket::Status::Done) {
        std::cout << "Could not connect to " << IPstring << " at port " << port << std::endl;
        return 1;
      }

      if (socket.receive(recv_pkt, serverIP, port) != sf::Socket::Done)
      {
        std::cout << "Failure to receive" << std::endl;
        return 1;
      }

      std::cout << recv_pkt << std::endl;
    }

    valid_input = false;
  }

  return 0;
}
