#include "../pch.h"
#include <iostream>
#include "Warrior.h"

Warrior::Warrior(int id, std::string name, unsigned int maxHp, std::string type, int attackPower)
	: Character(id, name, maxHp, type), attackPower(attackPower)
{

}

bool Warrior::performAction(Character& target)
{
	if (target.isAlive()) 
	{
		target.reduceHp(attackPower);
		std::cout << "Warrior " << name << " vua danh " << target.getName() << " giam " << attackPower << " HP." << std::endl;
		return true;
	}
	else
	{
		std::cout << "Loi: Target co hp = 0, khong the thuc hien hanh dong!" << std::endl;
		return false;
	}
}
void Warrior::display() const
{
    Character::display();

    std::cout
        << "Attack Power = "
        << attackPower
        << std::endl;
}

bool Warrior::setAttackPower(int newAttackPower)
{
	if (newAttackPower < static_cast<int>(WARRIOR_ATTACK_POWER_LOWER) || newAttackPower > static_cast<int>(WARRIOR_ATTACK_POWER_UPPER)) return false;
	attackPower = static_cast<unsigned int>(newAttackPower);
	return true;
}