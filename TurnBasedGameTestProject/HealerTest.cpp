#include "pch.h"
#include "../GameCore/model/Healer.h"


TEST(HealerTest, TC23KhongTheHoiMauChoTargetBiLoai)
{
	// Arrange
	Healer healer(1, "Healer", 100, 30); // healingPower = 30
	Healer target(2, "Target", 100, 20);
	target.reduceHp(150); // Giảm HP của target xuống dưới 0 để target bị loại
	// Act
	bool result = healer.performAction(target);
	// Assert
	EXPECT_FALSE(result);
}

TEST(HealerTest, TC24KHongTheHoiMauVuotQuaMaxHP)
{
	// Arrange
	Healer healer(1, "Healer", 100, 20); // healingPower = 20
	Healer target(2, "Target", 100, 20);
	target.reduceHp(10); // currentHp cua target = 90
	// Act
	bool result = healer.performAction(target);
	// Assert
	EXPECT_TRUE(result);
	EXPECT_EQ(target.getCurrentHp(), 100u);
}


TEST(HealerTest, TC25HoiDungLuongMau)
{
	// Arrange
	Healer healer(1, "Healer", 100, 20); // healingPower = 20
	Healer target(2, "Target", 100, 20);
	target.reduceHp(30); // currentHp cua target = 70
	// Act
	bool result = healer.performAction(target);
	// Assert
	EXPECT_TRUE(result);
	EXPECT_EQ(target.getCurrentHp(), 90u);
}


TEST(HealerTest, TC26CoTheHoiMauChoBanThan)
{
	// Arrange
	Healer healer(1, "Healer", 100, 20); // healingPower = 20
	healer.reduceHp(30); // currentHp cua healer = 70
	// Act
	bool result = healer.performAction(healer);
	// Assert
	EXPECT_TRUE(result);
	EXPECT_EQ(healer.getCurrentHp(), 90u);
}
