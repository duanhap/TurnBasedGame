#include "../pch.h"
#include <iostream>
#include "Warrior.h"

Warrior::Warrior(int id, std::string name, unsigned int maxHp, std::string type, int attackPower)
	: Character(id, name, maxHp, type), attackPower(attackPower)
{

}

void Warrior::performActionInBattle(CombatantSlot& actorSlot, CombatantSlot& targetSlot)
{
	targetSlot.currentHp -= (int)attackPower;
	if (targetSlot.currentHp < 0) targetSlot.currentHp = 0;
	std::cout << name << " (Warrior) tan cong gay " << attackPower << " sat thuong." << std::endl;
}

bool Warrior::performAction(Character& target)
{
	// Dùng cho test trực tiếp — ghi vào maxHp của Character (standalone, không phải phiên đấu)
	if (!target.isAlive()) {
		std::cout << "[LOI] Target da bi ha!" << std::endl;
		return false;
	}
	target.reduceHp(attackPower);
	std::cout << name << " (Warrior) tan cong " << target.getName()
	          << " gay " << attackPower << " sat thuong." << std::endl;
	return true;
}
void Warrior::display() const
{
    Character::display();

    std::cout
        << "Attack Power = "
        << attackPower
        << std::endl;
}

bool Warrior::setAttackPower(int newAttackPower)
{
	if (newAttackPower < static_cast<int>(WARRIOR_ATTACK_POWER_LOWER) || newAttackPower > static_cast<int>(WARRIOR_ATTACK_POWER_UPPER)) return false;
	attackPower = static_cast<unsigned int>(newAttackPower);
	return true;
}