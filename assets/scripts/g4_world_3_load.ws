# Load the actual world
loadWorld("../assets/grids/third_floor.grid")

# Load the agents for this world
loadAgents("../assets/third_floor_input.json")

# Reset player position
setAgentPosition(player,0,0)

# Set game end indicator
next_world = "GAME_END"

