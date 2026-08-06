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

// ===========================================================================
// setName() — tested via Warrior (Character is abstract)
// ===========================================================================

TEST(WarriorTest, SetName_ValidAlphanumeric_ReturnsTrueAndUpdates)
{
    // Arrange
    Warrior w(1, "OldName", 100, "WARRIOR", 20);
    // Act
    bool result = w.setName("NewName123");
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(w.getName(), "NewName123");
}

TEST(WarriorTest, SetName_EmptyString_ReturnsFalseAndDoesNotChange)
{
    // Arrange
    Warrior w(1, "Ares", 100, "WARRIOR", 20);
    // Act
    bool result = w.setName("");
    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(w.getName(), "Ares");
}

TEST(WarriorTest, SetName_ContainsSpace_ReturnsFalse)
{
    // Arrange
    Warrior w(1, "Ares", 100, "WARRIOR", 20);
    // Act
    bool result = w.setName("New Name");
    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(w.getName(), "Ares");
}

TEST(WarriorTest, SetName_ContainsSpecialChar_ReturnsFalse)
{
    // Arrange
    Warrior w(1, "Ares", 100, "WARRIOR", 20);
    // Act
    bool result = w.setName("Ares@123");
    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(w.getName(), "Ares");
}

TEST(WarriorTest, SetName_ContainsUnderscore_ReturnsFalse)
{
    // Arrange
    Warrior w(1, "Ares", 100, "WARRIOR", 20);
    // Act
    bool result = w.setName("Ares_God");
    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(w.getName(), "Ares");
}


// ===========================================================================
// setMaxHp() — tested via Warrior (Character is abstract)
// ===========================================================================

TEST(WarriorTest, SetMaxHp_ValidValue_ReturnsTrueAndUpdates)
{
    // Arrange
    Warrior w(1, "Ares", 100, "WARRIOR", 20);
    // Act
    bool result = w.setMaxHp(200);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(w.getMaxHp(), 200u);
}

TEST(WarriorTest, SetMaxHp_LowerBound_ReturnsTrueAndUpdates)
{
    // Arrange
    Warrior w(1, "Ares", 100, "WARRIOR", 20);
    // Act
    bool result = w.setMaxHp(CHARACTER_MAX_HP_LOWER);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(w.getMaxHp(), CHARACTER_MAX_HP_LOWER);
}

TEST(WarriorTest, SetMaxHp_UpperBound_ReturnsTrueAndUpdates)
{
    // Arrange
    Warrior w(1, "Ares", 100, "WARRIOR", 20);
    // Act
    bool result = w.setMaxHp(CHARACTER_MAX_HP_UPPER);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(w.getMaxHp(), CHARACTER_MAX_HP_UPPER);
}

TEST(WarriorTest, SetMaxHp_Zero_ReturnsFalseAndDoesNotChange)
{
    // Arrange
    Warrior w(1, "Ares", 100, "WARRIOR", 20);
    // Act
    bool result = w.setMaxHp(0);
    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(w.getMaxHp(), 100u);
}

TEST(WarriorTest, SetMaxHp_AboveUpper_ReturnsFalseAndDoesNotChange)
{
    // Arrange
    Warrior w(1, "Ares", 100, "WARRIOR", 20);
    // Act
    bool result = w.setMaxHp(CHARACTER_MAX_HP_UPPER + 1);
    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(w.getMaxHp(), 100u);
}


// ===========================================================================
// setAttackPower()
// ===========================================================================

TEST(WarriorTest, SetAttackPower_ValidValue_ReturnsTrueAndUpdates)
{
    // Arrange
    Warrior w(1, "Ares", 100, "WARRIOR", 20);
    // Act
    bool result = w.setAttackPower(50);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(w.getAttackPower(), 50u);
}

TEST(WarriorTest, SetAttackPower_LowerBound_ReturnsTrueAndUpdates)
{
    // Arrange
    Warrior w(1, "Ares", 100, "WARRIOR", 20);
    // Act
    bool result = w.setAttackPower(static_cast<int>(WARRIOR_ATTACK_POWER_LOWER));
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(w.getAttackPower(), WARRIOR_ATTACK_POWER_LOWER);
}

TEST(WarriorTest, SetAttackPower_UpperBound_ReturnsTrueAndUpdates)
{
    // Arrange
    Warrior w(1, "Ares", 100, "WARRIOR", 20);
    // Act
    bool result = w.setAttackPower(static_cast<int>(WARRIOR_ATTACK_POWER_UPPER));
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(w.getAttackPower(), WARRIOR_ATTACK_POWER_UPPER);
}

TEST(WarriorTest, SetAttackPower_Zero_ReturnsFalseAndDoesNotChange)
{
    // Arrange
    Warrior w(1, "Ares", 100, "WARRIOR", 20);
    // Act
    bool result = w.setAttackPower(0);
    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(w.getAttackPower(), 20u);
}

TEST(WarriorTest, SetAttackPower_Negative_ReturnsFalseAndDoesNotChange)
{
    // Arrange
    Warrior w(1, "Ares", 100, "WARRIOR", 20);
    // Act
    bool result = w.setAttackPower(-10);
    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(w.getAttackPower(), 20u);
}

TEST(WarriorTest, SetAttackPower_AboveUpper_ReturnsFalseAndDoesNotChange)
{
    // Arrange
    Warrior w(1, "Ares", 100, "WARRIOR", 20);
    // Act
    bool result = w.setAttackPower(static_cast<int>(WARRIOR_ATTACK_POWER_UPPER) + 1);
    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(w.getAttackPower(), 20u);
}
