x,y=getAgentPosition(player)
nx,ny=0,0

if(action_id==MOVE_UP){
nx,ny=x,y-1
}
if(action_id==MOVE_DOWN){
nx,ny=x,y+1
}
if(action_id==MOVE_LEFT){
nx,ny=x-1,y
}
if(action_id==MOVE_RIGHT){
nx,ny=x+1,y
}

_="if (!main_grid.IsValid(new_position)) { return false; }"
_="if (!IsTraversable(agent, new_position)) { return false; }"

setAgentPosition(player,nx,ny)
