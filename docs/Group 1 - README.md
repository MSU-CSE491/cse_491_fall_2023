# Group 1: Agent Library Function Descriptions

Below is a set of classes and functions designed to create multiple different Agents with various functionalities.

<details>

<summary> Path Agent </summary>
Agent that has a user-defined custom movement pattern passed a sequence to be sequentially applied as the agent is updated

- Constructor with ```std::vector```
    - param ```id```: unique agent id
    - param ```name```: name of path agent
    - param ```offsets```: collection of offsets to move the agent
    ```PathAgent(size_t id, std::string const& name, std::vector<cse491::GridPosition> && offsets = {}) : cse491::AgentBase(id, name), offsets_(offsets);```

- Constructor with ```std::string```
    - param ```id```: unique agent id
    - param ```name```: name of path agent
    - param ```commands```: sequence of commands to be interpreted as offsets
    ```PathAgent::PathAgent(size_t id, std::string const& name, std::string_view commands) : cse491::AgentBase(id, name), offsets_(str_to_offsets(commands))```

- Initializer
    - Checks that the agent is able to freely change its own grid location.         Verifies that it can currently index into a valid offset
    - return: ```true``` if so; ```false``` otherwise.
    ```bool PathAgent::Initialize();```

- Select Action
    - Moves the agent by applying the current offset
    - param ```WorldGrid```: reference grid
    - param ```type_options_t```: type options
    - param ```item_set_t```: set of items
    - param ```agent_set_t```: agent set
    ```size_t PathAgent::SelectAction(const WorldGrid & /* grid*/, const type_options_t & /* type_options*/, const item_set_t & /* item_set*/, const agent_set_t & /* agent_set*/)```


- Set Properties with ```std::vector```
    - Assigns the offsets_member to a new series of offsets
    - param ```offsets```: collection of grid positions used as the new offsets
    - param ```start_index = 0```: which offset to start indexing into (beginning by default)
    - return: ```self```
    - **Attention:** throws an `std::invalid_argument` when an invalid start index is provided
    ```PathAgent& PathAgent::SetProperties(std::vector<cse491::GridPosition> && offsets, size_t start_index = 0)```
    
- Set properties with ```std::string```
    - Assigns the offsets_ member to a new series of offsets, taking a command string
    - param ```commands```: formatted string of commands used as offsets
    - param ```start_index = 0```: which command to begin indexing into (first command by default)
    - return: ```self```
    - **Attention:** throws an `std::invalid_argument` when mis-formatted commands an invalid index is provided
    ```PathAgent& PathAgent::SetProperties(std::string_view commands, size_t start_index = 0)```

- Helper function for a formatted ```std::string``` input
    - Converts a string to a sequence of offsets
    - This convenience method takes a string with a special formatting that allows one to specify a sequence of inputs in linear directions.
    - The format is [steps[*]]<direction> where `steps` is a positive integer and optional (assumed to be 1 by default)
    - star `*` represents scaling the movement by `steps`. Optional, but cannot be used if `steps` is not provided
        - if the star is not present, then `steps` individual offsets are created in the direction `direction`
    - Example: "n w 3e 10*s 5*w x" should create the sequence of offsets
        - `{-1, 0}, {0, 1}, {0, -1}, {0, -1}, {0, -1}, {10, 0}, {0, 5}, {0, 0}`
    - param `commands`: string in a format of sequential directions
    - **note:** throws an `std::invalid_argument` when input string is poorly formatted
    - **note:** this includes when a negative integer is passed as `steps`. If a zero is used, treated as the default (one)

    ```static std::vector<cse491::GridPosition> str_to_offsets(std::string_view commands)```

  </details>

  More agents to be documented...
