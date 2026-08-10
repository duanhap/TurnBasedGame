#pragma once
#include "Character.h"

const unsigned int ARCHER_NORMAL_DAMAGE_UPPER = 500;
const unsigned int ARCHER_NORMAL_DAMAGE_LOWER = 1;
const unsigned int ARCHER_CRITICAL_DAMAGE_UPPER = 1000;
const unsigned int ARCHER_CRITICAL_DAMAGE_LOWER = 1;

class Archer : public Character
{
private:
	unsigned int normalDamage;
	unsigned int criticalDamage;
	unsigned int attackCount;

public:
	Archer() = default;
	Archer(int id, std::string name, unsigned int maxHp, std::string type, int normalDamage, int criticalDamage);
	~Archer() = default;

	unsigned int getNormalDamage() const { return normalDamage; }
	unsigned int getCriticalDamage() const { return criticalDamage; }

	bool setNormalDamage(int newNormalDamage);
	bool setCriticalDamage(int newCriticalDamage);

	bool performAction(Character& target) override;
	void display() const override;
};
