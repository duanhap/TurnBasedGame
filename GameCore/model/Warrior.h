#pragma once
#include "Character.h"

class Warrior : public Character
{
private:
	unsigned int attackPower;

public:
	Warrior();
	Warrior(int, std::string, unsigned int, std::string, int);
	~Warrior();
	bool performAction(Character& target) override;
};