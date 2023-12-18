# List of cell types
floor_id = addCellType("floor", "Floor that you can easily walk over.", " ")
flag_id = addCellType("flag", "Goal flag for a game end state", "g","Goal")
wall_id = addCellType("wall", "Impenetrable wall that you must find a way around.", "#", CELL_WALL)
hidden_warp_id = addCellType("hidden_warp", "Hidden warp tile that warps to floor 3.", "u","Warp")
water_id = addCellType("water","Water that distinguishes fire.","w",CELL_WATER)

# Load the actual world
loadWorld("../assets/grids/group4_maze.grid")

# Place the player agent into the world
# Name is interface to play nicely with the 2D interface
player=addAgent("Player2D","Interface","@",0,0)
setProperty(player,"Health",100.0)
setProperty(player,"Strength",7.0)
setProperty(player,"Defense",7.0)
setAgentPosition(player,0,0)

# Place items
addItem("Sword of Power","S",1,2,"Damage",20)
addItem("Inferno Slicer","S",3,4,"Damage",12.5,"Speed",15.0,"Burning Duration",2.5)
addItem("Daedric Armor","+",5,0,"Armor",20,"ExtraSpace",5)
addItem("Axe of Health","A",1,3,"Damage",8.5,"Health",35)
addItem("Electric Dagger","D",6,2,"Damage",25)
chest = addItem("Ender Chest","C",0,4,"Chest",1)
fire_dagger = addItem("Fire Dagger","D",-1,-1,"Damage",15.0)
addInventoryItem(chest,fire_dagger)

# Set link to next world
next_world = "../assets/scripts/g4_world_2_load.ws"

