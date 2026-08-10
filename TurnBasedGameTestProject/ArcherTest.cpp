#include "pch.h"
#include "../GameCore/model/Archer.h"
#include "../GameCore/model/Warrior.h"

TEST(ArcherTest, TC30DamBaoArcherGayDungDamageTheoLuot)
{
	// Arrange
	Archer archer(1, "Archer", 100, "Archer", 20, 40);
	Warrior target(2, "Target", 100, "Warrior", 10);

	// Act & Assert
	bool result1 = archer.performAction(target);
	EXPECT_TRUE(result1);
	EXPECT_EQ(target.getCurrentHp(), 80u);

	bool result2 = archer.performAction(target);
	EXPECT_TRUE(result2);
	EXPECT_EQ(target.getCurrentHp(), 60u);

	bool result3 = archer.performAction(target);
	EXPECT_TRUE(result3);
	EXPECT_EQ(target.getCurrentHp(), 20u);
}

TEST(ArcherTest, TC31DamBaoArcherKhongTheDanhNhanVatDaBiLoai)
{
	// Arrange
	Archer archer(1, "Archer", 100, "Archer", 20, 40);
	Warrior target(2, "Target", 0, "Warrior", 10); // HP = 0

	// Act
	bool result = archer.performAction(target);

	// Assert
	EXPECT_FALSE(result);
}
