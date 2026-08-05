#include "pch.h"
#include "../GameCore/model/Warrior.h"
#include "../GameCore/model/Mage.h"


TEST(WarriorTest, TC09AresDanhLuna)
{
  // Arrange
  Warrior ares(1, "Ares", 100, "Warrior", 30); // attackPower = 30
  Mage luna(2, "Luna", 80, "Mage", 100, 80, 40, 10); // hp = 80

  // Act
	bool result = ares.performAction(luna);
  // Assert
	EXPECT_TRUE(result);
	EXPECT_EQ(luna.getMaxHp(), 50u); // 80 - 30 = 50
}


TEST(WarriorTest, PerformActionReducesTargetHp)
{
    // Arrange
    Warrior warrior(1, "Warrior", 100, "WarriorType", 20);
    Mage target(2, "Target", 50, "MageType", 30, 10, 5, 2);

    // Act
    bool result = warrior.performAction(target);

    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(target.getMaxHp(), 30u);
}

TEST(WarriorTest, PerformActionReturnsFalseWhenTargetIsDead)
{
    // Arrange
    Warrior warrior(1, "Warrior", 100, "WarriorType", 20);
    Mage target(2, "Target", 0, "MageType", 30, 10, 5, 2);

    // Act
    bool result = warrior.performAction(target);

    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(target.getMaxHp(), 0u);
}

TEST(WarriorTest, PerformActionReducesHpToZeroWhenDamageExceedsCurrentHp)
{
    // Arrange
    Warrior warrior(1, "Warrior", 100, "WarriorType", 60);
    Mage target(2, "Target", 50, "MageType", 30, 10, 5, 2);

    // Act
    bool result = warrior.performAction(target);

    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(target.getMaxHp(), 0u);
    EXPECT_FALSE(target.isAlive());
}

TEST(WarriorTest, PerformActionDoesNotDamageBelowZero)
{
    // Arrange
    Warrior warrior(1, "Warrior", 100, "WarriorType", 100);
    Mage target(2, "Target", 30, "MageType", 30, 10, 5, 2);

    // Act
    warrior.performAction(target);

    // Assert
    EXPECT_EQ(target.getMaxHp(), 0u);
}
