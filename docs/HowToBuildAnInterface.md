# How to Build an Interface

When building your own interface, the first steps are boilerplate:

1. Include the InterfaceBase file If you're in the Interfaces/ directory, do this with: `#include "../core/InterfaceBase.hpp"`

2. Open your namespace.  Everything else you do should be done in your team's namespace.

3. Build your Interface class, making sure to derive it from `InterfaceBase`.  Something like `class MYInterface : public InterfaceBase {`, but of course use your Interface name instead of "MYInterface".

In the private (or protected, if you prefer) section of the class, you may create any member functions or variables that your interface might use.  I recommend a series of helper functions, such as a `DrawGrid` function to display the current state of the world.

In the public section, you should create:

4. A constructor and destructor, as needed for your implementation.  Interfaces will likely need to do substantial setup in the constructor and teardown in the destructor.

5. Add an Initialize function. This gets run _after_ the interface has been configured by the world, thus you will have access to all of the available actions in order to map them to buttons or other inputs.  This function has the format `bool Initialize() override {...}` and should return `true` as long as everything initialized correctly (false otherwise).

6. A `SelectAction` function that presents to the user the current state of the world and then reports back the next action for the user's agent should take. The format for `SelectionAction` is:
```
    size_t SelectAction(const WorldGrid & grid,
                        const type_options_t & type_options,
                        const item_set_t & item_set,
                        const agent_set_t & agent_set) override
    {
      // An interface should keep track of the user's actions and report them
      // back.  If the system is not supposed to wait for the user (i.e., real
      // time) then return a zero any time the user has not selected an action.
    }
```
The arguments passed in to `SelectAction` will be the full set of possibilities by default (i.e., the full grid, all type options, all items, and all agents), but some worlds may choose to maintain more limited versions of these variables containing only what an organism currently knows about.  In such cases it would be these more limited version that would be passed in to the function.

As part of their current state, agents have two relevant member variables inherited from their base class.  (1) A map of strings to action IDs called `action_map`, that indicates which value to return to take an action by a given name, and (2) an `int` called `action_result` that indicates the results of their last action.  Usually `action_result` will be 1 or 0 to indicate success or failure of that action.
