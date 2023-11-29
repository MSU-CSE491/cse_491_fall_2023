# agent=attacker
# opponent=defender

# == stats that matter ==
# Strength: base damage (on agent)
# Damage: additional damage (from items)
# Defense: reduced damage (on agent)
# Armor: reduced damage (from items)

print(agent,"attacks",opponent)

# str=getAgentProperty(agent,"Strength")
# def=getAgentProperty(opponent,"Defense")
str=10
def=1
damage=rand(1,str-def)
print(damage," damage")

hp=getAgentProperty(opponent,"Health")-damage
if (hp<0){
	hp=0
}
setAgentProperty(opponent,"Health",hp)
