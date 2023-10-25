# Notes and questions about future development of core functionality

## Levelization Map

A chart to track the dependencies in the project core.  Each file is dependant
only on files above it in the core.

| File              | Level | Depends On...                         |
| ----------------- | ----- | ------------------------------------- |
| CoreObject.hpp    | 0     | (standard library only)               |
| Data.hpp          | 0     | (standard library only)               |
| GridPosition.hpp  | 0     | (standard library only)               |
| Entity.hpp        | 1     | GridPosition                          |
| WorldGrid.hpp     | 1     | CoreObject, GridPosition              |
| AgentBase.hpp     | 2     | Data, Entity, GridPosition, WorldGrid |
| InterfaceBase.hpp | 3     | AgentBase                             |
| WorldBase.hpp     | 3     | AgentBase, Data, Entity, WorldGrid    |

## General

- The current files use 100-character max line width.  Many standards limit line width to 80 characters, but I find that modern IDEs make 100 or even 120 very readable.  We need to decide on this (and many other) style guidelines.
- We need to lock in a style.  Currently we have functions and types in CamelCase, variables in snake_case, and constants (including enum values) in ALL_CAPS.  Is everyone good with this?
- We currently do not separate private/protected class variables or functions from public ones.  One common technique is to end private ones in an underscore ('_').  Is this a convention we want to add?  It is informative, but can be ugly.
- We should make more objects derived from `CoreObject` to simplify saving and loading.

## `GridPosition.hpp`

- Should we disallow negative grid positions?  They can't map to `WorldGrid` coordinates, but may be meaningful in some worlds.  At the moment we are using them to represent invalid positions.
- Should we track a pointer back to the `WorldGrid` that this position is from?  It will take a little more space, but it might make it easier to have multiple grids co-exist without confusion.

## `WorldGrid.hpp`

- Should we add functionality to extract a portion of the grid?
- Should we allow a WorldGrid representing only a section of grid, but with an offset and possibly a pointer back to the main grid it's part of?

## `Entity.hpp`

- Should we use a better/different structure for properties?  Right now properties can only have a `double` value, but we could use `std::variant` to allow for a set of allowed values, or even `std::any`.
- If we have Entity derive from `CoreObject`, we have to think about how to keep requirements for serialize functions on subsequent derived classes.  One option is to simply provide tools (like a `SerializeEntity()` function), but don't build the required virtual functions yet.  Still, it would be nice to be able to require correctness (or at least detect common errors, like forgetting to run `SerializeEntity()`)

## `AgentBase.hpp`

- Should Agents keep a link back to the world they're from?
- Should Agents all have a facing?  (Or should all entities even?)


## `EasyLogging.h`


### 1. Basic Logging:
```cpp
Logger::Log() << Team::TEAM_1 << LogLevel::DEBUG << "This is a debug message." << std::endl;
Logger::Log() << Team::TEAM_2 << LogLevel::ERROR << "An error occurred!" << std::endl;
```

### 2. Logging with Colors:
```cpp
Logger::Log() << Team::TEAM_3 << LogLevel::INFO << Color::GREEN << "Success message." << Logger::endl;
Logger::Log() << Team::TEAM_4 << LogLevel::ERROR << Color::RED << "Error message." << Logger::endl;
```

### 3. Logging Variables:
```cpp
int variable = 42;
Logger::Log() << Team::TEAM_5 << LogLevel::DEBUG << "The value of variable is: " << variable << Logger::endl;
```

### 4. Logging File and Line Information:
```cpp
Logger::Log() << Team::TEAM_6 << LogLevel::INFO << LOG_RELLINE << "This log is from " << __FILE__ << " line " << __LINE__ << Logger::endl;
```

### 5. Logging Function Names:
```cpp
Logger::Log() << Team::TEAM_7 << LogLevel::DEBUG << LOG_FNC << "This log is inside the function." << Logger::endl;
```

one can also just use std::endl instead of Logger::endl they are equivalent.


calling `Team (or) LogLevel (or) std::endl (or) Logger::endl` will reset the color team, color and log level to the default values.


Current Feature set.

- setting team number
- setting log status [debug, info, warning, error]
- setting colors of print statements.
- Does not compile in RELEASE
- Line number and file name


Future Feature set.
- logging to a file
- adding flags for debuging in cmake.
- making it run on a different thread/ running it Asynchronously?. Especially helpful for improving performance and responsiveness
    - cuz of the io operation or if we want to send stuff thru the network
- More log levels like `Fatal` or `Trace`
- Categorization and filtering thru flags