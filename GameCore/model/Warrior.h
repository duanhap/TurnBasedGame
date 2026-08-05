#pragma once
#include "Character.h"

class Warrior : public Character
{
private:
	unsigned int attackPower;

public:
	Warrior() = default;
	Warrior(int, std::string, unsigned int, std::string, int);
	~Warrior() = default;
	unsigned int getAttackPower() const { return attackPower; }
	bool performAction(Character& target) override;
	void display() const override;
};