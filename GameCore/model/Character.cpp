#include "Character.h"

Character::Character(int id, std::string name, unsigned int maxHp, std::string type)
	: id(id), name(name), maxHp(maxHp), type(type)
{

}

Character::getId() const
{
	return id;
}	

Character::getMaxHp() const
{
	return maxHp;
}	

Character::getName() const
{
	return name;
}

Character::isAlive() const
{
	return maxHp > 0;
}

Character::reduceHp(unsigned int amount)
{
	if (amount >= maxHp) {
		maxHp = 0;
	} else {
		maxHp -= amount;
	}
}