move(act_id,cx,cy){
	if(action_id==act_id){
		nx,ny=cx,cy
	}
}

_="Handle player movement"
x,y=getAgentPosition(agent)
nx,ny=0,0

move(0,x,y)
move(MOVE_UP,x,y-1)
move(MOVE_DOWN,x,y+1)
move(MOVE_LEFT,x-1,y)
move(MOVE_RIGHT,x+1,y)

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
