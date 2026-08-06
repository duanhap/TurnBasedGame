#pragma once
#include <string>
#include "CombatantSlot.h"

class Character
{
protected:
	int id;
	std::string name;
	unsigned int maxHp;
	std::string type;
	// cons
	Character();
	Character(int, std::string, unsigned int, std::string);

public:
	// des
	virtual ~Character();

	// getter
	int getId() const;
	int getMaxHp() const;
	virtual unsigned int getMaxMana() const { return 0; } // Warrior trả 0, Mage override
	std::string getName() const;
	std::string getType() const;

	// display
	virtual void display() const;

	// business logic
	bool isAlive() const;
	void reduceHp(unsigned int amount);

	// FR-04: dùng trong test và standalone
	// Warrior/Mage implement riêng — runtime polymorphism
	virtual bool performAction(Character& target) = 0;

	// FR-04: dùng trong BattleEngine — thao tác trên CombatantSlot phiên đấu
	// Không đụng dữ liệu gốc của Roster
	// Warrior/Mage đều phải override — không có default body
	virtual void performActionInBattle(CombatantSlot& actorSlot,
	                                   CombatantSlot& targetSlot) = 0;
};
