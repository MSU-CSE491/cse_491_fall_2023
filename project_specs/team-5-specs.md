# CSE 491 Project 5: Networking Interface

*Team Members: Zane O’Dell, Krish Magal, Chase Lichtman, Ethan Rylko*

## Module High Level Goal
Create an interface that allows various agents and world modules to interact with each other across a network. 
This interface will either be a Peer to Peer network or a Server/Client framework depending on the decision of the main project.  If it ends up being a LAN game, we will do Peer to Peer, and if it ends up being remote play we will do a Server/Client framework.
The interface will be creating generic functions that will allow other teams to send information to other parts of the overall system, including agent to agent communication, agent to world communication.

## Key Features Made Available To Other Modules
* Various agents will be able to interact with other agents across a network.
* Agents will be able to interact with the world that they are a part of across a network.
* A networking API will be able to be accessed by other groups. The API will consist of generic functions that various teams will be able to use to send information to other parts of the system over the network. 

## Features Other Modules Will Need To Provide
* Because of the nature of our project, we are the “binding” project that allows everything to work together across a network.
* It is imperative that our team understands the build and makeup of each project. This is due to the fact that we have to take everything built and make it work together across various networking protocols and standards.

## Project Challenges
* We believe that for our project, we will need to work with other groups the most out of every group. We are providing services and features that entirely depend on the structure of other groups and their codebases. 
* Various operating systems may have different networking APIs and other requirements that will all need to be considered when developing this project. We are in the process of finding a solution or solutions to this problem. Some libraries we are considering using are Valve’s GameNetworkingSockets library, Asio, and SFML. Once we understand the project specifications and a bit more research, we can decide which library best fits the needs of the project. 
* While all of our team members have written some networking code before, most have written it exclusively in Python, for much smaller scale projects. We have some, but relatively little C++ networking knowledge, especially outside the scope of a Linux environment.
