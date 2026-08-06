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
	// cons
	Character();
	Character(int, std::string, unsigned int, std::string);
	
public:
	// des
	virtual ~Character();
	// getter
	int getId() const;
	int getMaxHp() const;
	std::string getName() const;
	std::string getType() const;
	// setter
	bool setName(const std::string& newName);
	bool setMaxHp(unsigned int newMaxHp);
	// display
	virtual void display() const;
	// business logic
	bool isAlive() const;
	void reduceHp(unsigned int amount);
	virtual bool performAction(Character& target) = 0;
};