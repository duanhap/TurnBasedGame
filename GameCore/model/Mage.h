#pragma once
#include "Character.h"

class Mage : public Character
{
private:
	unsigned int maxMana, spellDamage, manaCost, fallbackDamage;
public:
	Mage() = default;
	Mage(int, std::string, unsigned int, std::string,
	     unsigned int, unsigned int, unsigned int, unsigned int);
	~Mage() = default;

	bool performAction(Character& target) override;
	void performActionInBattle(CombatantSlot& actorSlot, CombatantSlot& targetSlot) override;

	void display() const override;

	unsigned int getMaxMana()        const override { return maxMana; }
	unsigned int getSpellDamage()    const { return spellDamage; }
	unsigned int getManaCost()       const { return manaCost; }
	unsigned int getFallbackDamage() const { return fallbackDamage; }
};
