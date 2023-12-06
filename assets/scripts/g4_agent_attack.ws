# agent=attacker
# opponent=defender

# == stats that matter ==
# Strength: base damage (on agent)
# Damage: additional damage (from items)
# Defense: reduced damage (on agent)
# Armor: reduced damage (from items)

print(agent," attacks ",opponent)

str=getProperty(agent,"Strength")
def=getProperty(opponent,"Defense")
print("Base=",str,",",def)

# Check agent equipped item
if (getInventorySize(agent)){
	# Treat this as the equipped item
	item=getInventoryItem(agent,0)
	if (hasProperty(item,"Damage")){
		str=str+getProperty(item,"Damage")
	}
}
print("Str=",str)

# Check opponent equipped item
if (getInventorySize(opponent)){
	# Treat this as the equipped item
	item=getInventoryItem(opponent,0)
	if (hasProperty(item,"Armor")){
		def=def+getProperty(item,"Armor")
	}
}
print("Def=",def)

# Calculate damage
diff=str-def
if (diff<=0){
	diff=1
}
damage=rand(1,2*diff)
print(damage," damage")

hp=getProperty(opponent,"Health")-damage
if (hp<0){
	hp=0
	setProperty(opponent,"Dead",1)
	setProperty(opponent,"symbol","x")
}
setProperty(opponent,"Health",hp)
