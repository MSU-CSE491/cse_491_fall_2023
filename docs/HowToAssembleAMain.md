# How to Assemble a Main

Once you have your Agents, World, and Interface picked out, it's easy to assemble them into an `main()` function that you can turn into a working executable.

Create the appropriate .cpp file, and follow these steps:

1. Include all of the modules that you will be using.  In the case of `simple_main.cpp` this looks like:
```
#include "Agents/PacingAgent.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Worlds/MazeWorld.hpp"
```

2. Inside your `main()` function, start by building your world.  It should look something like:
```
main() {
  cse491::MazeWorld world;
```
Some world types may be able to take arguments to their constructors, but most probably won't.

3. Add in your agents using the `AddAgent` member function, specifying the type of agent with a template argument and the agent's name as a regular argument.  `AddAgent` will return a reference to the new agent, so you can immediately modify it, such as by setting its position.
```
  world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
  world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);
```
Furthermore, AddAgent can take additional parameters to set properties BEFORE configuration is run. For example, if you want to set stats for strength and speed, you could do something like:
```
  world.AddAgent<cse491::PacingAgent>("Pacer 1", "strength", 1.0, "speed", 5.5).SetPosition(3,1);
  world.AddAgent<cse491::PacingAgent>("Pacer 2", "strength", 8.6, "speed", 1.2).SetPosition(6,1);
```
This will allow you to change what you do in agent configuration based on the properties provided.

4. Add in your interface as a type of agent.  For example:
```
  world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("char", '@');
```
This will set up any "player" characters.

5. Launch the run using `world.Run();`.


## What if I want to collect extra data for analysis?

If you want to track the agents and the world during a run, you will skip step 5 above and instead manually run the agents and update the world, collecting data as you go.

For example, in place of `world.Run();` you could have:
```
  while (!world.GetRunOver()) {
    world.RunAgents();
    world.UpdateWorld();
    // Collect extra data about the world here!
  }
```

You should have access to all of the basic data in the world; we may need to expand the API to provide access if you don't have everything you need.

## Compiling a main
By default, any `.cpp` files in `/source/` (not in subdirectories) should be automatically detected and compiled with CMake (see `HowToCompile.md`).
 
If your main requires other source files (i.e., other `.cpp` files), you will also need to make a `.cmake` file. 
For example, `example.cpp` would need a `example.cmake` file. 

This `.cmake` file will give you fine-grained control over the compilation process of your main file. To add extra source files, simply add new lines like so: 
```
add_source_to_target(${EXE_NAME} "source/Agents/PathAgent.cpp")
```
The `${EXE_NAME}` argument should ***NOT*** change, as it is a variable from the other CMake infrastructure. 
You will, however, need to update the other argument, which a string to the other source file you want to compile and link against. You will need one copy of this line for each source file you need to include. 
