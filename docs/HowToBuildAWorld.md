# How to Build A World

When building your own world, the first steps are boilerplate:

1. Include the WorldBase file If you're in the Worlds/ directory, do this with: `#include "../core/WorldBase.hpp"`

2. Open your namespace.  Everything else you do should be done in your team's namespace.

3. Build your world class, making sure to derive it from `WorldBase`.  Something like `class MYWorld : public WorldBase {`, but of course use your world name instead of "MYWorld".

In the private (or protected, if you prefer) section of the class, you may create any member variables that your world might use.  Additionally, you need to set up some internal functionality.  Specifically:

4. Create a enumeration where you will list all of the action types available to an agent.  The first action should always be a "do nothing" action and explicitly set to zero.  For example: `enum ActionType { REMAIN_STILL=0, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };`

5. Add a `ConfigAgent` function that provides a new agent with a list of the actions that it has available.  For example:
```
    /// Provide the agent with movement actions.
    void ConfigAgent(AgentBase & agent) override {
      agent.AddAction("up", MOVE_UP);
      agent.AddAction("down", MOVE_DOWN);
      agent.AddAction("left", MOVE_LEFT);
      agent.AddAction("right", MOVE_RIGHT);
    }
```

In the public section of the class, you need to build a few more functions:

6. A default constructor (i.e., `MYWorld() { ... }`).  You should do any normal set up for this world, including the member variables that you created above.  You should also specify the type options and initialize the `main_grid` for this world.
  - Specify the type options with the `AddCellType` function, which is defined in the base class.
    ```
    AddCellType("dirt", "Regular ground", ' ');
    AddCellType("tree", "A tall tree", '^');
    AddCellType("river", "Flowing water", '~');
    ```
  - An easy way to initialize the `main_grid` is to use the `Read()` member function on `WorldGrid` to load the grid from a file.  For example: `main_grid.Read("../assets/grids/MYWorld.grid", type_options);`

7. Add a destructor (i.e., `~MYWorld() { ... }`).  This function only needs to have a body if you need to clean up one of the member variables you added.

8. Build `int DoAction(AgentBase & agent, size_t action_id) override { ... }` for your new world.  This function will be run automatically whenever an agent takes an action, specifying both the agent and action involved.


Finally you need to build a main file that initializes this world, add any needed agents (either autonomous agents or interfaces), and run.


## Member variables inherited from `WorldBase`

`WorldBase` has a lot of boilerplate functionality though almost all of it can be replaced in a derived world.

There are five member variables maintained in the world:
- `WorldGrid main_grid` - the "main" world that is passed to agents by default.  You may maintain additional grids if you choose, and may decide which one of these is seen by an agent.
- `type_options_t type_options` - a vector of `CellType` information, which specifies the name of each cell, its description, and an (optional) ASCII symbol associated with it.  The position in this vector it he ID of a cell type.
- `item_set_t item_set` - a vector of pointers to all of the "items" in this world.  The world can define and use items however it likes, though they should be type `cse491::Entity`.  Like agents, items can have arbitrary properties.
- `agent_set_t agent_set` - a vector of all agents currently active in the world.
- `bool run_over` - a value that defaults to false, but if you change it to true the run will terminate at the end of the current iteration.


## Working with properties

Agents can have arbitrarily named properties; worlds can add properties to the agents, and modify the (`double`) value associated with that property at any time.

To add a property, simply give it a value, such as `agent.SetProperty("property_name", value)` where `value` is a floating point number (it will default to 0.0 if you don't include it.)  This might be a good way to add "hit points" or "size" to agents.  You can alter what happens to an agent based on these properties; if an agent's hit points reach zero, you might kill it off.

You can always set properties to new values later, or use `agent.HasProperty("property_name")` to test if an agent has a property or `agent.GetProperty("property_name")` to read its value.

If at any point you want to remove a property, you can do so with `agent.RemoveProperty("property_name")`.  For example, you might have a property called "invisible" that you add to an agent when it vanishes, but you remove when it reappears.

For properties that you want agents to always possess, it's probably a good idea to set them in the `ConfigAgent()` function.


## Providing limited data to agents

Some worlds will want to provide agents with limited data, perhaps only showing rooms that the agent is currently in or items/agents nearby. To do this, the world will be responsible for producing or maintaining the alternate data structures and those are the ones that should be passed to agents when they are called.

The default version of the function `WorldBase::RunAgents()` will use the full information provided in the world, but this is a virtual function.  If you override it in your derived class, you simply need to step through all of the agents and run `SelectAction` on each with the revised arguments.  (Of course, after each action is selected, make sure to also run `DoAction` and `SetActionResult` for each, as per the base class.)


## Managing non-agent activities in the world

In some worlds, you may want events happening in the background, independent of agents.  For example, crops might grow, seasons might change, or weather might occur. To manage these non-agent events, you simply need to override `UpdateWorld()` in your derived class.  Every time each agent is run (in the `Run()` function) UpdateWorld() is then called.  If you need a different balance for `UpdateWorld()` (perhaps you need it to go after every agent) simply override the Run() function in your derived World class.
