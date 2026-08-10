#include "../pch.h"
#include <iostream>
#include "Archer.h"

Archer::Archer(int id, std::string name, unsigned int maxHp, std::string type, int normalDamage, int criticalDamage)
	: Character(id, name, maxHp, type), normalDamage(normalDamage), criticalDamage(criticalDamage), attackCount(0)
{

}

bool Archer::performAction(Character& target)
{
	if (!target.isAlive()) {
		std::cout << "[LOI] Target da bi ha!" << std::endl;
		return false;
	}

	attackCount++;
	unsigned int damageToDeal;
	if (attackCount % 3 == 0) {
		damageToDeal = criticalDamage;
	}
	else {
		damageToDeal = normalDamage;
	}

	target.reduceHp(damageToDeal);
	std::cout << name << " (Archer) tan cong " << target.getName()
		<< " gay " << damageToDeal << " sat thuong." << std::endl;
	return true;
}

void Archer::display() const
{
	Character::display();
	std::cout
		<< "Normal Damage = " << normalDamage 
		<< " | Critical Damage = " << criticalDamage << std::endl;
}

bool Archer::setNormalDamage(int newNormalDamage)
{
	if (newNormalDamage < static_cast<int>(ARCHER_NORMAL_DAMAGE_LOWER) || newNormalDamage > static_cast<int>(ARCHER_NORMAL_DAMAGE_UPPER)) return false;
	normalDamage = static_cast<unsigned int>(newNormalDamage);
	return true;
}

bool Archer::setCriticalDamage(int newCriticalDamage)
{
	if (newCriticalDamage < static_cast<int>(ARCHER_CRITICAL_DAMAGE_LOWER) || newCriticalDamage > static_cast<int>(ARCHER_CRITICAL_DAMAGE_UPPER)) return false;
	criticalDamage = static_cast<unsigned int>(newCriticalDamage);
	return true;
}
