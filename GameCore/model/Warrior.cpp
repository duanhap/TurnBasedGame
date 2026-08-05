#include "../pch.h"
#include <iostream>
#include "Warrior.h"

Warrior::Warrior(int id, std::string name, unsigned int maxHp, std::string type, int attackDamage)
	: Character(id, name, maxHp, type), attackPower(attackDamage)
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