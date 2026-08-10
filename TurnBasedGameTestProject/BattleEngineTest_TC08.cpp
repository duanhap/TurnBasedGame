#include "pch.h"
#include "../GameCore/manager/BattleEngine.h"
#include "../GameCore/manager/CharacterRoster.h"
#include "../GameCore/model/Team.h"
#include "../GameCore/model/Warrior.h"
#include "../GameCore/model/Mage.h"

// Test TC-08 với kiểm tra HP/mana không thay đổi khi hành động thất bại
TEST(BattleEngineTest, TC08_PerformAction_InvalidTarget_HPmanaUnchanged)
{
    // Arrange: Red Team có Ares, Blue Team có Luna
    CharacterRoster roster;
    roster.add(std::make_unique<Warrior>(1, "Ares", 100, "Warrior", 30));
    roster.add(std::make_unique<Mage>(2, "Luna", 80, "Mage", 50, 40, 10, 5));
    
    Team redTeam(1, "RedTeam");
    redTeam.addCharacter(1);
    
    Team blueTeam(2, "BlueTeam");
    blueTeam.addCharacter(2);
    
    BattleEngine battleEngine;
    
    // Select và start battle
    bool selectResult = battleEngine.selectTeams(&redTeam, &blueTeam, roster);
    EXPECT_TRUE(selectResult);
    bool startResult = battleEngine.startBattle();
    EXPECT_TRUE(startResult);
    
    // Lấy trạng thái ban đầu của Battle
    const Battle& battle = battleEngine.getBattle();
    
    // Lấy slot của Ares và Luna để kiểm tra HP/mana ban đầu
    // Team A (0): Ares, Team B (1): Luna
    const Character* aresSlotBefore = battle.getSlot(0, 0);
    const Character* lunaSlotBefore = battle.getSlot(1, 0);
    
    unsigned int aresHpBefore = aresSlotBefore->getCurrentHp();
    unsigned int lunaHpBefore = lunaSlotBefore->getCurrentHp();
    unsigned int lunaManaBefore = lunaSlotBefore->getCurrentMana();
    
    // Xác nhận HP ban đầu đúng
    EXPECT_EQ(aresHpBefore, 100u); // Ares có 100 HP
    EXPECT_EQ(lunaHpBefore, 80u);  // Luna có 80 HP
    EXPECT_EQ(lunaManaBefore, 50u); // Luna có 50 mana
    
    // Xác nhận lượt hiện tại là Ares (Team A)
    const Character* currentActor = battleEngine.getCurrentActor();
    ASSERT_NE(currentActor, nullptr);
    EXPECT_EQ(currentActor->getId(), 1); // Ares
    
    // Act 1: Ares target chính mình (target không hợp lệ - cùng team)
    bool actionResult1 = battleEngine.performCurrentAction(1);
    EXPECT_FALSE(actionResult1); // Hành động thất bại
    
    // Kiểm tra HP/mana không thay đổi
    const Character* aresSlotAfter1 = battleEngine.getBattle().getSlot(0, 0);
    const Character* lunaSlotAfter1 = battleEngine.getBattle().getSlot(1, 0);
    
    EXPECT_EQ(aresSlotAfter1->getCurrentHp(), aresHpBefore);     // Ares HP không đổi
    EXPECT_EQ(lunaSlotAfter1->getCurrentHp(), lunaHpBefore);     // Luna HP không đổi  
    EXPECT_EQ(lunaSlotAfter1->getCurrentMana(), lunaManaBefore); // Luna mana không đổi
    
    // Act 2: Target nhân vật không tồn tại trong battle
    bool actionResult2 = battleEngine.performCurrentAction(999);
    EXPECT_FALSE(actionResult2);
    
    // Kiểm tra HP/mana vẫn không đổi
    const Character* aresSlotAfter2 = battleEngine.getBattle().getSlot(0, 0);
    const Character* lunaSlotAfter2 = battleEngine.getBattle().getSlot(1, 0);
    
    EXPECT_EQ(aresSlotAfter2->getCurrentHp(), aresHpBefore);
    EXPECT_EQ(lunaSlotAfter2->getCurrentHp(), lunaHpBefore);
    EXPECT_EQ(lunaSlotAfter2->getCurrentMana(), lunaManaBefore);
    
    // Battle vẫn trong trạng thái IN_PROGRESS
    EXPECT_TRUE(battleEngine.isInProgress());
    EXPECT_FALSE(battleEngine.isFinished());
    
    // Current actor vẫn là Ares (lượt chưa chuyển)
    const Character* stillCurrentActor = battleEngine.getCurrentActor();
    ASSERT_NE(stillCurrentActor, nullptr);
    EXPECT_EQ(stillCurrentActor->getId(), 1);
}