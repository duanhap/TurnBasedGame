#include "../pch.h"
#include "Character.h"
#include <iostream>

Character::Character(int id, std::string name, unsigned int maxHp, std::string type)
	: id(id), name(name), maxHp(maxHp), type(type)
{

}

Character::~Character()
{

}

int Character::getId() const
{
	return id;
}	

int Character::getMaxHp() const
{
	return maxHp;
}	

std::string Character::getName() const
{
	return name;
}

std::string Character::getType() const
{
	return type;
}

void Character::display() const
{
    std::cout << "[" << type << "] ID=" << id
              << " | " << name
              << " | HP=" << maxHp
              << std::endl;
}

bool Character::isAlive() const
{
	return maxHp > 0;
}

void Character::reduceHp(unsigned int amount)
{
	if (amount >= maxHp) {
		maxHp = 0;
	}
	else {
		maxHp -= amount;
	}
}