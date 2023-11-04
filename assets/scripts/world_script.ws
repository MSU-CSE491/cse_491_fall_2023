_="List of cell types"
floor=addCellType("floor","An empty space"," ")
wall=addCellType("wall","A solid wall","#","Solid")
one=addCellType("one","The number 1","1","Collectible")
two=addCellType("two","The number 2","2","Collectible")
three=addCellType("three","The number 3","3","Collectible")

_="Load the actual world"
loadWorld("assets/grids/lang_load_test.grid")

_="Place the player agent into the world"
player=addAgent("Player","PlayerAgent","%",1,1)

