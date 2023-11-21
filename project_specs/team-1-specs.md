# Team Wall-E : Behavioral Function Library for Autonomous Agents
### Members:
- David Rackerby  
- Matt Kight  
- Monika Kanphade  
- Yousif Murrani  
## Documentation

#### Code Style
1. Code should be styled according to the [Google C++ Style
Guide](https://google.github.io/styleguide/cppguide.html)  
2. All code should be contained inside of the namespace `walle`
3. If your text editor doesn't have a built-in linter/formatter, use
   `clang-format` with the `--style=google`


## Goals 

#### Library Functions
Implement a set of common library functions that interface with the other
modules.

##### Path algorithms
1. `get_shortest_path(start : Point, end : Point, map : grid)` 
   i. A* search to get path between start and end point on map  
   ii. Returns : List of instructions to navigate from start to end  
   iii. Exact actions and return values will be dependent on the Agent calling
   the function

2. `get_random_path(start: Point, num_moves : int, map: grid)` 
    i. Return a list of actions of len num_moves

3. `walk_route(start: Point, end: Point, pace: double)`
    i. find certain path between two points
    ii. walk between the path between the two points at certain pace


#### Interaction Functions
1. `is_colliding(agent1 : Agent, agent2: Agent)`  
    i. Checks the position of agent1 in relation to agent2   
    ii. Returns true if there is overlap between agent1 and agent2

2. `can_see(agent1: Agent, agent2: Agent)`  
    i. Returns true if agent2 is in line of sight of agent1

3. `damage(agent1 : Agent, agent2 : Agent)`
    i. Changes the health points of agent2 by the number of damage points
    agent 1 is able to do.

4. `heal(agent1 : Agent, agent2 : Agent)`
    i. Changes the health points of agent2 by the number of heal points
    agent 1 is able to do.

5. `interact_with_object(agent: Agent, object: Object, action: Action)`  
    i. Performs action on object
    ii. Returns bool if action was successful  
    iii. This is a placeholder as we learn more about how the agent can
    interaction with objects in the world this will occur

6. `interact_with_world(agent: Agent, world: World, action: Action, point: Pos)`
    i. Performs an action at a certain position
    ii. Returns bool if action was completed

#### Grid-fill algorithms
1. `flood_fill(start: Point, map: grid, criteria: callable, action: Action )`   
    i. takes action to the left, right, up, and down from the start point if the
    grid point matches the criteria
    
2. `game_of_life()`   
    i. Allow for automation of objects based on the rules of [Conway's Game of
    Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)  
    ii. Params and returns will be added after discussion with other groups to
    see if this is wanted


#### Challenge 
1. Creating an Autonomous agent that is capable of moving around an interacting
   with the world
2. Working with other agent teams to implement functionality they need for their agents
