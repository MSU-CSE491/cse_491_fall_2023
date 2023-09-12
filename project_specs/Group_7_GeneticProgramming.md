# Group 7 GP Project Specifications

Team Members:
Aman Dhruva Thamminana, Rajmeet Singh Chandok, Simon Situ, and Jason Seguin


## Project Description:

### What is your high-level goal for this module?
- Create a versatile and adaptable genetic programming agent module that will be able to learn and interact with a given world to perform various tasks 
- The actions taken would be based on a decision tree

**Key features:**
- A customizable fitness evaluation to allow fitness functions to be plugged in to different worlds
- Modular design that will separate key components like population, selection, crossover, mutation, etc. to make it easy to tweak the algorithm

### What key features will you make available to other modules?
- A simple interface for other modules to query the agent state
- Actions the agents are taking within the world
- Metrics for the data analysis team
- Training data logs

### What features will you need other modules to provide?
- State of the world
- Available actions that can be taken in the world
- Different types of world for testing
- A way to speed up the world to train over long periods of time
- Score or a goal to work towards

### Why is this project challenging?
- A lot of computing power is needed to train over a long period of time
- Highly dependent on other groups' modules, particularly the worlds
- Difficult to create fitness functions that can create effective agents across many worlds
