#include "../pch.h"
#include "Character.h"
#include <iostream>
#include <cctype>

Character::Character(int id, std::string name, unsigned int maxHp, std::string type)
	: id(id), name(name), maxHp(maxHp), type(type), currentHp(maxHp), currentMana(0)
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
	return currentHp > 0;
}

void Character::reduceHp(unsigned int amount)
{
	if (amount >= currentHp) {
		currentHp = 0;
	}
	else {
		currentHp -= amount;
	}
}

void Character::spendMana(unsigned int amount)
{
	if (amount >= currentMana) {
		currentMana = 0;
	}
	else {
		currentMana -= amount;
	}
}

void Character::resetForBattle()
{
	currentHp   = maxHp;
	currentMana = getMaxMana();
}

bool Character::setName(const std::string& newName)
{
	if (newName.empty()) return false;
	for (char c : newName) {
		if (!std::isalnum(static_cast<unsigned char>(c))) return false;
	}
	name = newName;
	return true;
}

bool Character::setMaxHp(unsigned int newMaxHp)
{
	if (newMaxHp < CHARACTER_MAX_HP_LOWER || newMaxHp > CHARACTER_MAX_HP_UPPER) return false;
	maxHp = newMaxHp;
	return true;
}