#include "Warrior.h"


Warrior::Warrior(int id, std::string name, unsigned int maxHp, std::string type, int attackDamage)
	: Character(id, name, maxHp, type), attackDamage(attackDamage)
{

}

bool Warrior::performAction(Character& target)
{
	if (target.isAlive()) 
	{
		target.reduceHp(attackDamage);
		std::cout << "Warrior " << name << " vua danh " << target.getName() << " giam " << attackDamage << " HP." << std::endl;
		return true;
	}
	else
	{
		std::cout << "Loi: Target co hp = 0, khong the thuc hien hanh dong!" << std::endl;
		return false;
	}
}