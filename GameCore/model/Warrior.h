#pragma once
#include "Character.h"

const unsigned int WARRIOR_ATTACK_POWER_UPPER = 500;
const unsigned int WARRIOR_ATTACK_POWER_LOWER = 1;

class Warrior : public Character
{
private:
	unsigned int attackPower;

public:
	Warrior() = default;
	Warrior(int, std::string, unsigned int, std::string, int);
	~Warrior() = default;
	unsigned int getAttackPower() const { return attackPower; }
	bool setAttackPower(int newAttackPower);
	bool performAction(Character& target) override;
	void display() const override;
};