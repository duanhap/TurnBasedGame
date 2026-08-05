#include "pch.h"
#include "../GameCore/model/Warrior.h"
#include "../GameCore/model/Mage.h"


TEST(MageTest, TC10LunaDanhAres)
{
	// Arrange
	Warrior ares(1, "Ares", 100, "Warrior", 30); // attackPower = 30
	Mage luna(2, "Luna", 80, "Mage", 10, 40, 10, 10); // mana = 10, spellDamage = 40, manaCost = 10, fallbackDamage = 10
	// Act
	bool result = luna.performAction(ares);
	// Assert
	EXPECT_TRUE(result);
	EXPECT_EQ(ares.getMaxHp(), 60u); // 100 - 40 = 60
	EXPECT_EQ(luna.getMaxMana(), 0u); // 10 - 10 = 0
}


TEST(MageTest, PerformActionWhenInsufficientManaUsesFallbackDamage)
{
	// Arrange
	Warrior ares(1, "Ares", 100, "Warrior", 30);
	Mage luna(2, "Luna", 80, "Mage", 5, 40, 10, 8); // mana = 5 < manaCost = 10, fallbackDamage = 8
	// Act
	bool result = luna.performAction(ares);
	// Assert
	EXPECT_TRUE(result);
	EXPECT_EQ(ares.getMaxHp(), 92u); // 100 - 8 = 92
	EXPECT_EQ(luna.getMaxMana(), 5u); // fallback does not consume mana
}


TEST(MageTest, PerformActionReturnsFalseWhenTargetIsDead)
{
	// Arrange
	Warrior deadAres(1, "DeadAres", 0, "Warrior", 30); // hp = 0 -> already dead
	Mage luna(2, "Luna", 80, "Mage", 100, 40, 10, 10);
	// Act
	bool result = luna.performAction(deadAres);
	// Assert
	EXPECT_FALSE(result);
	EXPECT_EQ(deadAres.getMaxHp(), 0u);
	EXPECT_EQ(luna.getMaxMana(), 100u); // mana should not be consumed
}


TEST(MageTest, PerformActionSpellDamageReducesHpToZero)
{
	// Arrange
	Mage target(1, "Target", 30, "Mage", 10, 10, 5, 2);
	Mage luna(2, "Luna", 80, "Mage", 50, 30, 10, 5); // spellDamage = 30, target hp = 30
	// Act
	bool result = luna.performAction(target);
	// Assert
	EXPECT_TRUE(result);
	EXPECT_EQ(target.getMaxHp(), 0u);
	EXPECT_FALSE(target.isAlive());
	EXPECT_EQ(luna.getMaxMana(), 40u); // 50 - 10 = 40
}


TEST(MageTest, PerformActionFallbackDamageDoesNotReduceBelowZero)
{
	// Arrange
	Mage target(1, "Target", 5, "Mage", 10, 10, 5, 2);
	Mage luna(2, "Luna", 80, "Mage", 0, 50, 10, 15); // no mana -> fallbackDamage = 15
	// Act
	bool result = luna.performAction(target);
	// Assert
	EXPECT_TRUE(result);
	EXPECT_EQ(target.getMaxHp(), 0u); // should cap at 0, not negative
	EXPECT_EQ(luna.getMaxMana(), 0u); // mana stays 0
}


TEST(MageTest, PerformActionZeroManaCostCastsSpellWithoutDrainingMana)
{
	// Arrange
	Warrior ares(1, "Ares", 100, "Warrior", 30);
	Mage luna(2, "Luna", 80, "Mage", 20, 25, 0, 10); // manaCost = 0
	// Act
	bool result = luna.performAction(ares);
	// Assert
	EXPECT_TRUE(result);
	EXPECT_EQ(ares.getMaxHp(), 75u); // 100 - 25 = 75
	EXPECT_EQ(luna.getMaxMana(), 20u); // mana unchanged because cost is 0
}



