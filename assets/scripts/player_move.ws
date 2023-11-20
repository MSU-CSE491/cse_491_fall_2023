x,y=getAgentPosition(agent)
nx,ny=0,0

if(action_id==0){
	nx,ny=x,y
}
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

print(agent,nx,ny)
if(isValid(nx,ny)){
	if(isTraversable(agent,nx,ny)){
		_="Check for an agent at this position: attack if this is the case"
		opponent=findAgentAt(nx,ny)
		if(opponent>-1){
			_="Agent exists, attack it (opponent=id of opponent)"
			str=getAgentProperty(agent,"Strength")
			def=getAgentProperty(opponent,"Defense")
			damage=rand(1,str-def)
			
			hp=getAgentProperty(opponent,"Health")-damage
			if (hp<0){
				hp=0
				setAgentProperty(opponent,"symbol","x")
			}
			setAgentProperty(opponent,"Health",hp)
		}
		if(opponent<0){
			_="Only set position if movement is allowed"
			setAgentPosition(agent,nx,ny)
		}
	}
}
