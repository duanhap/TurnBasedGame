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
{}

void Mage::performActionInBattle(CombatantSlot& actorSlot, CombatantSlot& targetSlot)
{
	// Mage tự đọc currentMana từ slot phiên đấu — không đụng maxMana của Roster
	// Không cần if/switch theo type ở BattleEngine — runtime polymorphism
	if ((unsigned int)actorSlot.currentMana >= manaCost)
	{
		actorSlot.currentMana -= (int)manaCost;
		targetSlot.currentHp  -= (int)spellDamage;
		if (targetSlot.currentHp < 0) targetSlot.currentHp = 0;
		std::cout << name << " (Mage) dung spell gay " << spellDamage
		          << " sat thuong. Mana con: " << actorSlot.currentMana << std::endl;
	}
	else
	{
		targetSlot.currentHp -= (int)fallbackDamage;
		if (targetSlot.currentHp < 0) targetSlot.currentHp = 0;
		std::cout << name << " (Mage) het mana, dung fallback gay "
		          << fallbackDamage << " sat thuong." << std::endl;
	}
}

bool Mage::performAction(Character& target)
{
	// Overload cho test — ghi trực tiếp vào maxHp/maxMana của Character
	if (!target.isAlive()) {
		std::cout << "[LOI] Target da bi ha!" << std::endl;
		return false;
	}
	if (maxMana >= manaCost)
	{
		maxMana -= manaCost;
		target.reduceHp(spellDamage);
		std::cout << name << " (Mage) dung spell danh " << target.getName()
		          << " gay " << spellDamage << " sat thuong. Mana con: " << maxMana << std::endl;
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
