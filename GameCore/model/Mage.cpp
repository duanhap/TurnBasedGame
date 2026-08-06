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
void Mage::display() const
{
    Character::display();

    std::cout
        << "Mana = " << maxMana
        << " | Spell Damage = " << spellDamage
        << " | Mana Cost = " << manaCost
        << " | Fallback Damage = " << fallbackDamage
        << std::endl;
}

bool Mage::setMaxMana(unsigned int newMaxMana)
{
	if (newMaxMana < MAGE_MANA_LOWER || newMaxMana > MAGE_MANA_UPPER) return false;
	maxMana = newMaxMana;
	return true;
}

bool Mage::setSpellDamage(unsigned int newSpellDamage)
{
	if (newSpellDamage < MAGE_SPELL_DAMAGE_LOWER || newSpellDamage > MAGE_SPELL_DAMAGE_UPPER) return false;
	spellDamage = newSpellDamage;
	return true;
}

bool Mage::setManaCost(unsigned int newManaCost)
{
	if (newManaCost < MAGE_MANA_COST_LOWER || newManaCost > MAGE_MANA_COST_UPPER) return false;
	manaCost = newManaCost;
	return true;
}

bool Mage::setFallbackDamage(unsigned int newFallbackDamage)
{
	if (newFallbackDamage < MAGE_FALLBACK_DAMAGE_LOWER || newFallbackDamage > MAGE_FALLBACK_DAMAGE_UPPER) return false;
	fallbackDamage = newFallbackDamage;
	return true;
}