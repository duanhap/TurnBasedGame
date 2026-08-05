#pragma once
#include "Character.h"

class Mage : public Character
{
private:
	unsigned int maxMana, spellDamage, manaCost, fallbackDamage;
public:
	Mage();
	Mage(int, std::string, unsigned int, std::string, unsigned int, unsigned int, unsigned int, unsigned int);
	~Mage();
	bool performAction(Character& target) override;
};