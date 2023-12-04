# Load the actual world
loadWorld("assets/grids/second_floor.grid")

# Place the player agent into the world
player=addAgent("Player","PlayerAgent","@",0,0)
setProperty(player,"Health",100.0)
setProperty(player,"Strength",7.0)
setProperty(player,"Defense",7.0)

# Place items
addItem("Sword of Power","S",1,2,"Damage",20)
addItem("Inferno Slicer","S",3,4,"Damage",12.5,"Speed",15.0,"Burning Duration",2.5)
addItem("Daedric Armor","+",5,0,"Health",99,"ExtraSpace",5)
addItem("Axe of Health","A",1,3,"Damage",8.5,"Health",35)
addItem("Electric Dagger","D",6,2,"Damage",25)
chest = addItem("Ender Chest","C",0,4)
fire_dagger = addItem("Fire Dagger",-1,-1,"Damage",15.0)
addInventoryItem(chest,fire_dagger)

