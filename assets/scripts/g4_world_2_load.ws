# Load the actual world
loadWorld("../assets/grids/second_floor.grid")

# Load the agents for this world
loadAgents("../assets/second_floor_input.json")

# Reset player position
print(player)
setAgentPosition(player,0,0)

# Place items for second floor
addItem("Hammer","H",20,43,"Damage",35)
addItem("Dagger","D",12,1,"Damage",10)
addItem("Leather Armor","+",1,6,"Armor",6)

# Set link to next world
next_world = "../assets/scripts/g4_world_3_load.ws"

