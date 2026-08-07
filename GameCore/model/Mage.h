#pragma once
#include "Character.h"

const unsigned int MAGE_MANA_UPPER = 500;
const unsigned int MAGE_MANA_LOWER = 1;
const unsigned int MAGE_SPELL_DAMAGE_UPPER = 500;
const unsigned int MAGE_SPELL_DAMAGE_LOWER = 1;
const unsigned int MAGE_MANA_COST_UPPER = 500;
const unsigned int MAGE_MANA_COST_LOWER = 0;
const unsigned int MAGE_FALLBACK_DAMAGE_UPPER = 500;
const unsigned int MAGE_FALLBACK_DAMAGE_LOWER = 1;

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
	bool setMaxMana(unsigned int newMaxMana);
	bool setSpellDamage(unsigned int newSpellDamage);
	bool setManaCost(unsigned int newManaCost);
	bool setFallbackDamage(unsigned int newFallbackDamage);
};
