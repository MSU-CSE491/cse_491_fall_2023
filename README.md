The ??? Software is a project started in the Fall 2023 CSE 491 C++ Capstone Course at Michigan Stat University.  The goal of the project is to build a modular framework for agent-based modelling on grid worlds, we new simulations or games can be quickly and easily crafted.

The current modules are:

Worlds:
- MazeWorld: A simple, initial maze structure for agents to navigate.
- SecondWorld
- GenerativeWorld
- ManualWorld

Agents:
- PacingAgent: Walks back and forth, turning around when it hits walls.
- AStarAgent: Finds a navigable path to a target position.
- PathAgent: Follows a designated path.
- RandomAgent: Goes in a random direction at each opportunity.
- TrackingAgent: Follows a player from the time any tracking agent sees it.
- GP Agents: Generate targeted agents with evolved behaviors for specific worlds.

Interfaces:
- Trash Interface: a simple, poorly done, text interface meant for testing.
- SFML Interface
- Network Interface
- Data Collection Interface

Other Tools:
- Agent Factory
- Agent Library