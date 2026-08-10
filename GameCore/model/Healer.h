#pragma once
#include "Character.h"


/*
Author: Nguyen Dinh Dung
Last modified: 2026-08-10
*/
const unsigned int HEALER_HEALING_POWER_UPPER = 500;
const unsigned int HEALER_HEALING_POWER_LOWER = 1;

class Healer :
    public Character
{
private:
	/// Số lượng HP mà Healer có thể hồi phục cho đồng đội
	unsigned int healingPower;

public:
	// cons
	Healer() = default;
	Healer(int, std::string, unsigned int, unsigned int);
	// des
	virtual ~Healer();
	// getter
	unsigned int getHealingPower() const;
	// setter
	bool setHealingPower(unsigned int newHealingPower);
	// display
	virtual void display() const override;
	// business logic
	virtual bool performAction(Character& target) override;
};

