#include "../pch.h"
#include "Mage.h"
#include <iostream>

Mage::Mage(int id, std::string name, unsigned int maxHp, std::string type,
           unsigned int maxMana, unsigned int spellDamage,
           unsigned int manaCost, unsigned int fallbackDamage)
	: Character(id, name, maxHp, type)
	, maxMana(maxMana)
	, spellDamage(spellDamage)
	, manaCost(manaCost)
	, fallbackDamage(fallbackDamage)
{
	currentMana = maxMana; // base ctor không gọi được getMaxMana() (virtual), set ở đây
}

bool Mage::performAction(Character& target)
{
	if (!target.isAlive()) {
		std::cout << "[LOI] Target da bi ha!" << std::endl;
		return false;
	}
	// Mage dùng currentMana (battle state) — đủ mana thì spell, hết thì fallback
	if (currentMana >= manaCost)
	{
		spendMana(manaCost);
		target.reduceHp(spellDamage);
		std::cout << name << " (Mage) dung spell danh " << target.getName()
		          << " gay " << spellDamage << " sat thuong. Mana con: " << currentMana << std::endl;
	}
	else
	{
		target.reduceHp(fallbackDamage);
		std::cout << name << " (Mage) het mana, fallback danh " << target.getName()
		          << " gay " << fallbackDamage << " sat thuong." << std::endl;
	}
	return true;
}

void Mage::display() const
{
	Character::display();
	std::cout << "MaxMana=" << maxMana
	          << " | SpellDmg=" << spellDamage
	          << " | ManaCost=" << manaCost
	          << " | Fallback=" << fallbackDamage
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
