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


// ===========================================================================
// setMaxMana()
// ===========================================================================

TEST(MageTest, SetMaxMana_ValidValue_ReturnsTrueAndUpdates)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setMaxMana(100);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(m.getMaxMana(), 100u);
}

TEST(MageTest, SetMaxMana_LowerBound_ReturnsTrueAndUpdates)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setMaxMana(MAGE_MANA_LOWER);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(m.getMaxMana(), MAGE_MANA_LOWER);
}

TEST(MageTest, SetMaxMana_UpperBound_ReturnsTrueAndUpdates)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setMaxMana(MAGE_MANA_UPPER);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(m.getMaxMana(), MAGE_MANA_UPPER);
}

TEST(MageTest, SetMaxMana_Zero_ReturnsFalseAndDoesNotChange)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setMaxMana(0);
    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(m.getMaxMana(), 50u);
}

TEST(MageTest, SetMaxMana_AboveUpper_ReturnsFalseAndDoesNotChange)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setMaxMana(MAGE_MANA_UPPER + 1);
    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(m.getMaxMana(), 50u);
}


// ===========================================================================
// setSpellDamage()
// ===========================================================================

TEST(MageTest, SetSpellDamage_ValidValue_ReturnsTrueAndUpdates)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setSpellDamage(60);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(m.getSpellDamage(), 60u);
}

TEST(MageTest, SetSpellDamage_LowerBound_ReturnsTrueAndUpdates)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setSpellDamage(MAGE_SPELL_DAMAGE_LOWER);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(m.getSpellDamage(), MAGE_SPELL_DAMAGE_LOWER);
}

TEST(MageTest, SetSpellDamage_UpperBound_ReturnsTrueAndUpdates)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setSpellDamage(MAGE_SPELL_DAMAGE_UPPER);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(m.getSpellDamage(), MAGE_SPELL_DAMAGE_UPPER);
}

TEST(MageTest, SetSpellDamage_Zero_ReturnsFalseAndDoesNotChange)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setSpellDamage(0);
    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(m.getSpellDamage(), 30u);
}

TEST(MageTest, SetSpellDamage_AboveUpper_ReturnsFalseAndDoesNotChange)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setSpellDamage(MAGE_SPELL_DAMAGE_UPPER + 1);
    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(m.getSpellDamage(), 30u);
}


// ===========================================================================
// setManaCost()
// ===========================================================================

TEST(MageTest, SetManaCost_ValidValue_ReturnsTrueAndUpdates)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setManaCost(25);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(m.getManaCost(), 25u);
}

TEST(MageTest, SetManaCost_LowerBound_ReturnsTrueAndUpdates)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setManaCost(MAGE_MANA_COST_LOWER);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(m.getManaCost(), MAGE_MANA_COST_LOWER);
}

TEST(MageTest, SetManaCost_UpperBound_ReturnsTrueAndUpdates)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setManaCost(MAGE_MANA_COST_UPPER);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(m.getManaCost(), MAGE_MANA_COST_UPPER);
}

TEST(MageTest, SetManaCost_AboveUpper_ReturnsFalseAndDoesNotChange)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setManaCost(MAGE_MANA_COST_UPPER + 1);
    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(m.getManaCost(), 10u);
}


// ===========================================================================
// setFallbackDamage()
// ===========================================================================

TEST(MageTest, SetFallbackDamage_ValidValue_ReturnsTrueAndUpdates)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setFallbackDamage(15);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(m.getFallbackDamage(), 15u);
}

TEST(MageTest, SetFallbackDamage_LowerBound_ReturnsTrueAndUpdates)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setFallbackDamage(MAGE_FALLBACK_DAMAGE_LOWER);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(m.getFallbackDamage(), MAGE_FALLBACK_DAMAGE_LOWER);
}

TEST(MageTest, SetFallbackDamage_UpperBound_ReturnsTrueAndUpdates)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setFallbackDamage(MAGE_FALLBACK_DAMAGE_UPPER);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(m.getFallbackDamage(), MAGE_FALLBACK_DAMAGE_UPPER);
}

TEST(MageTest, SetFallbackDamage_Zero_ReturnsFalseAndDoesNotChange)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setFallbackDamage(0);
    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(m.getFallbackDamage(), 5u);
}

TEST(MageTest, SetFallbackDamage_AboveUpper_ReturnsFalseAndDoesNotChange)
{
    // Arrange
    Mage m(1, "Luna", 80, "MAGE", 50, 30, 10, 5);
    // Act
    bool result = m.setFallbackDamage(MAGE_FALLBACK_DAMAGE_UPPER + 1);
    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(m.getFallbackDamage(), 5u);
}
