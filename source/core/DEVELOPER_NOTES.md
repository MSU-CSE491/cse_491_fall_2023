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

## `GridPosition.hpp`

- Should we disallow negative grid positions?  They can't map to `WorldGrid` coordinates, but may be meaningful in some worlds.  At the moment we are using them to represent invalid positions.
- Should we track a pointer back to the `WorldGrid` that this position is from?  It will take a little more space, but it might make it easier to have multiple grids co-exist without confusion.

## `WorldGrid.hpp`

- Should we add functionality to extract a portion of the grid?
- Should we allow a WorldGrid representing only a section of grid, but with an offset and possibly a pointer back to the main grid it's part of?

## `Entity.hpp`

- Should we use a better/different structure for properties?  Right now properties can only have a `double` value, but we could use `std::variant` to allow for a set of allowed values, or even `std::any`.

## `AgentBase.hpp`

- Should Agents keep a link back to the world they're from?
- Should Agents all have a facing?  (Or should all entities even?)
