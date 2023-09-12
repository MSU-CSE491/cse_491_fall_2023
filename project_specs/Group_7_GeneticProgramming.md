# Group 7 GP Project Specifications

Team Members:
Aman Dhruva Thamminana, Rajmeet Singh Chandok, Simon Situ, and Jason Seguin


## Project Description:

### What is your high-level goal for this module?
- Create a versatile, adaptable genetic programming module that will effectively learn and interact with a given world to perform various tasks 
- The actions would be taken based on a decision tree

**Key features:**
- Customizable fitness evaluation - allow fitness functions to be plugged in for different worlds
- Modular design - separate key components like population, selection, crossover, mutation, etc. to make it easy to tweak the algorithm

### What key features will you make available to other modules?
- AGENTS (no shit sherlock)
- Metrics for the data analysis team
- Actions the agents are taking within the world 
- Data to the analysis team based on the interfaces agreed upon
- A simple interface for other modules to query the agent state, and get actions

### What features will you need other modules to provide?
- Actions possible for the world
    - Jump not possible???
    - Result of the action
- Different types of world for testing
- State of the world and a way to speed up on the world
- Score or a goal to work towards

### Why is this project challenging?
- Not a lot of experience with Genetic Programming
- Unsure about what the project is going to be on
- Long Training times, hard to debug and inavailability of Fast Hardware 
- Deciding the fitness function and Tuning the agents