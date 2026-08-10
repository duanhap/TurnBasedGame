#pragma once
#include <string>

const unsigned int CHARACTER_MAX_HP_UPPER = 500;
const unsigned int CHARACTER_MAX_HP_LOWER = 1;

class Character
{
protected:
	int id;
	std::string name;
	unsigned int maxHp;
	std::string type;
	// Battle state (session data) — reset trước mỗi trận
	unsigned int currentHp;
	unsigned int currentMana;
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
	unsigned int getCurrentHp() const { return currentHp; }
	unsigned int getCurrentMana() const { return currentMana; }
	std::string getName() const;
	std::string getType() const;

	// setter
	bool setName(const std::string& newName);
	bool setMaxHp(unsigned int newMaxHp);
	// display
	virtual void display() const;

	// business logic — thao tác trên battle state (currentHp/currentMana)
	bool isAlive() const;
	void reduceHp(unsigned int amount);
	void spendMana(unsigned int amount);
	// Khôi phục currentHp/currentMana về maxHp/maxMana — gọi khi bắt đầu trận
	void resetForBattle();

	// FR-04: hành động trong lượt — Warrior/Mage implement riêng (runtime polymorphism)
	virtual bool performAction(Character& target) = 0;
};
