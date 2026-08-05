#include "../pch.h"
#include "Mage.h"
#include <iostream>

Mage::Mage(int id, std::string name, unsigned int maxHp, std::string type, unsigned int maxMana, unsigned int spellDamage, unsigned int manaCost, unsigned int fallbackDamage)
	: Character(id, name, maxHp, type), maxMana(maxMana), spellDamage(spellDamage), manaCost(manaCost), fallbackDamage(fallbackDamage)
{

}

bool Mage::performAction(Character& target)
{
	if (target.isAlive())
	{
		if (maxMana >= manaCost) 
		{
			target.reduceHp(spellDamage);
			maxMana -= manaCost;
			std::cout << "Mage " << name << " vua dung spell danh " << target.getName() << " giam " << spellDamage << " HP." << std::endl;
		} 
		else 
		{
			target.reduceHp(fallbackDamage);
			std::cout << "Mage " << name << " khong du mana, dung fallback attack danh " << target.getName() << " giam " << fallbackDamage << " HP." << std::endl;
		}
		return true;
	}
	else
	{
		std::cout << "Loi: Target co hp = 0, khong the thuc hien hanh dong!" << std::endl;
		return false;
	}
}