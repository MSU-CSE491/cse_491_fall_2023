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
  if (argc != 3) {
    std::cout << "Must have an argument for server IP and port\nUsage: ./client [IP] [port]" << std::endl;
    return 1;
  }

  // create socket, get IP and port from args
  sf::UdpSocket socket;
  std::string ip_string(argv[1]);
  sf::IpAddress server_ip(ip_string);
  unsigned short port = stoi(std::string(argv[2]));

  // send ping to server
  std::string message = "ping!";
  if (socket.send(message.c_str(), message.size() + 1, server_ip, port) != Socket::Status::Done) {
    std::cout << "Could not connect to " << ip_string << " at port " << port << std::endl;
    return 1;
  } 
  
  // receive pong from server
  size_t received;
  char data[100];
  if (socket.receive(data, 100, received, server_ip, port) != sf::Socket::Done)
  {
      std::cout << "Failure to receive" << std::endl;
      return 1;
  }
  std::cout << data << std::endl;

  char input;
  bool valid_input = false;
  bool wait_for_input = true;
  sf::Packet recv_pkt;
  std::string recv_str;

  // receive initial map
  if (socket.receive(recv_pkt, server_ip, port) != sf::Socket::Done)
  {
    std::cout << "Failure to receive" << std::endl;
    return 1;
  }

  recv_pkt >> recv_str;
  std::cout << recv_str;

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
      default: valid_input = false;
    }

    // If we waited for input, but don't understand it, notify the user.
    if (wait_for_input && !valid_input) {
      std::cout << "Unknown key '" << input << "'." << std::endl;
    } else {
      if (socket.send(&input, 1, server_ip, port) != Socket::Status::Done) {
        std::cout << "Could not connect to " << ip_string << " at port " << port << std::endl;
        return 1;
      }

      if (socket.receive(recv_pkt, server_ip, port) != sf::Socket::Done)
      {
        std::cout << "Failure to receive" << std::endl;
        return 1;
      }

      recv_pkt >> recv_str;
      std::cout << recv_str;
    }

    valid_input = false;
  }

  return 0;
}
