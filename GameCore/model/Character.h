#pragma once

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
	// des
	virtual ~Character();
public:
	// getter
	int getId() const;
	int getMaxHp() const;
	std::string getName() const;
	// business logic
	bool isAlive() const;
	void reduceHp(unsigned int amount);
	virtual bool performAction(Character& target) = 0;
};