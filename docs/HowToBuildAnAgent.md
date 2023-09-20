# How to Build an Autonomous Agent

When building your own agent, the first steps are boilerplate:

1. Include the AgentBase file If you're in the Agents/ directory, do this with: `#include "../core/AgentBase.hpp"`

2. Open your namespace.  Everything else you do should be done in your team's namespace.

3. Build your agent class, making sure to derive it from `AgentBase`.  Something like `class MYAgent : public AgentBase {`, but of course use your agent name instead of "MYAgent".

In the private (or protected, if you prefer) section of the class, you may create any member variables that your agent might use.  Remember that you also have the option of using agent properties (see below) instead of private variables if you want the values to be accessible to other modules.

In the public section, you should create:

4. A constructor and destructor, as needed for your implementation (agents are not required to have any specific code here.)

5. An Initialize function that gets run _after_ the agent has been configured by the world.  This will allow an agent to set itself up based on the actions that were provided by the world.  This function has the format `bool Initialize() override {...}` and should return `false` if it detected a problem during initialization (such as a required action name is not available).

6. A `SelectAction` function that identifies the action that the organism will take. The format for `SelectionAction` is:
```
    size_t SelectAction(const WorldGrid & grid,
                        const type_options_t & type_options,
                        const item_set_t & item_set,
                        const agent_set_t & agent_set) override
    {
      // An agent should use their current state as well as the provided
      // grid, type_options, item_set, and agent set to decide next action.
    }
```
The arguments passed in to `SelectAction` will be the full set of possibilities by default (i.e., the full grid, all type options, all items, and all agents), but some worlds may choose to maintain more limited versions of these variables containing only what an organism currently knows about.  In such cases it would be these more limited version that would be passed in to the function.

As part of their current state, agents have two relevant member variables inherited from their base class.  (1) A map of strings to action IDs called `action_map`, that indicates which value to return to take an action by a given name, and (2) an `int` called `action_result` that indicates the results of their last action.  Usually `action_result` will be 1 or 0 to indicate success or failure of that action.


## Agent Properties

While properties are typically added to agents from a World, they are technically allowed to add properties to themselves, typically in their own constructor.  These properties might be ones that affect the behavior of the agents, but should also be accessible from outside of the Agent class itself.

For example, you might want to put `double facing = 1.0;` inside the private section if facing should only ever be used by this class.  Alternatively you can put `SetProperty("facing", 1.0);` in the constructor if you want it accessible to the world.  Any other class would be able to run `agent.HasProperty("facing")` to determine if the property exists in `agent`, or `agent.GetProperty("facing")` to retrieve its value.
