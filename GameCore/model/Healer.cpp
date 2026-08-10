#include <iostream>
#include "Healer.h"


Healer::Healer(int id, std::string name, unsigned int maxHp, unsigned int healingPower)
		: Character(id, name, maxHp, "Healer"), healingPower(healingPower)
{
}

Healer::~Healer()
{
}

unsigned int Healer::getHealingPower() const
{
	return healingPower;
}

bool Healer::setHealingPower(unsigned int newHealingPower)
{
	healingPower = newHealingPower;
	return true;
}

void Healer::display() const
{
	Character::display();
	std::cout << " | Healing Power=" << healingPower << std::endl;
}


bool Healer::performAction(Character& target)
{
	if (!target.isAlive())
	{
		std::cout << "Khong the hoi mau cho nhan vat da bi loai.\n";
		return false;
	}
	return target.heal(healingPower);
}