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

if(isValid(nx,ny)){
if(isTraversable(player,nx,ny)){
_="Only set position if movement is allowed"
_="TODO check for other agents in this cell"
setAgentPosition(player,nx,ny)
}
}
