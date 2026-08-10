#include "pch.h"
#include "../GameCore/manager/BattleEngine.h"
#include "../GameCore/manager/CharacterRoster.h"
#include "../GameCore/model/Team.h"
#include "../GameCore/model/Warrior.h"
#include "../GameCore/model/Mage.h"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static std::unique_ptr<Warrior> makeTestWarrior(int id, const std::string& name, unsigned int hp = 100, unsigned int atk = 30)
{
    return std::make_unique<Warrior>(id, name, hp, "Warrior", atk);
}

static std::unique_ptr<Mage> makeTestMage(int id, const std::string& name, unsigned int hp = 80)
{
    return std::make_unique<Mage>(id, name, hp, "Mage", 50, 40, 10, 5);
}

static Team createTeamWithCharacters(int teamId, const std::string& teamName, 
                                     CharacterRoster& roster, 
                                     const std::vector<int>& characterIds)
{
    Team team(teamId, teamName);
    for (int id : characterIds) {
        team.addCharacter(id);
    }
    return team;
}

// ===========================================================================
// TC-06: Đảm bảo thỏa mãn yêu cầu: 2 team battle phải khác nhau, 
// 1 team không thể không có character nào
// ===========================================================================

TEST(BattleEngineTest, TC06_StartBattle_EmptyTeam_ReturnsFalse)
{
    // Arrange: Team rỗng (không có character nào)
    CharacterRoster roster;
    roster.add(makeTestWarrior(1, "Ares"));
    
    Team emptyTeam(1, "EmptyTeam");
    Team validTeam(2, "ValidTeam");
    validTeam.addCharacter(1);
    
    BattleEngine battleEngine;
    
    // Act: Chọn team rỗng và team hợp lệ
    bool selectResult = battleEngine.selectTeams(&emptyTeam, &validTeam, roster);
    EXPECT_FALSE(selectResult); // selectTeams phải trả false với team rỗng
    
    // Act: Thử start battle với team rỗng (đã không setup thành công)
    bool startResult = battleEngine.startBattle();
    
    // Assert
    EXPECT_FALSE(startResult); // Không được phép start với team rỗng
    EXPECT_FALSE(battleEngine.isInProgress());
    EXPECT_FALSE(battleEngine.isFinished());
}

TEST(BattleEngineTest, TC06_StartBattle_SameTeam_ReturnsFalse)
{
    // Arrange: Cùng một team
    CharacterRoster roster;
    roster.add(makeTestWarrior(1, "Ares"));
    
    Team teamA(1, "TeamA");
    teamA.addCharacter(1);
    
    BattleEngine battleEngine;
    
    // Act: Chọn cùng một team cho cả hai bên
    bool selectResult = battleEngine.selectTeams(&teamA, &teamA, roster);
    EXPECT_FALSE(selectResult); // selectTeams phải trả false với cùng team
    
    // Act: Thử start battle với cùng một team (đã không setup thành công)
    bool startResult = battleEngine.startBattle();
    
    // Assert
    EXPECT_FALSE(startResult); // Không được phép start với cùng một team
    EXPECT_FALSE(battleEngine.isInProgress());
    EXPECT_FALSE(battleEngine.isFinished());
}

TEST(BattleEngineTest, TC06_StartBattle_InvalidSetup_ReturnsFalse)
{
    // Arrange: Không setup team
    BattleEngine battleEngine;
    
    // Act: Thử start battle mà chưa select teams
    bool startResult = battleEngine.startBattle();
    
    // Assert
    EXPECT_FALSE(startResult); // Không được phép start khi chưa setup
    EXPECT_FALSE(battleEngine.isInProgress());
    EXPECT_FALSE(battleEngine.isFinished());
}

// ===========================================================================
// TC-07: Đảm bảo Battle được khởi tạo đúng theo yêu cầu
// Start Red Team có và Blue Team có Luna
// HP/mana được reset; trạng thái IN_PROGRESS; Ares có lượt đầu.
// ===========================================================================

TEST(BattleEngineTest, TC07_StartBattle_ValidTeams_InitializesCorrectly)
{
    // Arrange: Red Team có Ares, Blue Team có Luna
    CharacterRoster roster;
    roster.add(makeTestWarrior(1, "Ares", 100, 30));
    roster.add(makeTestMage(2, "Luna", 80));
    
    Team redTeam(1, "RedTeam");
    redTeam.addCharacter(1);
    
    Team blueTeam(2, "BlueTeam");
    blueTeam.addCharacter(2);
    
    BattleEngine battleEngine;
    
    // Act: Select teams và start battle
    bool selectResult = battleEngine.selectTeams(&redTeam, &blueTeam, roster);
    EXPECT_TRUE(selectResult);
    
    bool startResult = battleEngine.startBattle();
    
    // Assert: Battle khởi tạo đúng
    EXPECT_TRUE(startResult);
    EXPECT_TRUE(battleEngine.isInProgress());
    EXPECT_FALSE(battleEngine.isFinished());
    
    // Kiểm tra current actor là Ares (lượt đầu)
    const Character* currentActor = battleEngine.getCurrentActor();
    ASSERT_NE(currentActor, nullptr);
    EXPECT_EQ(currentActor->getId(), 1); // Ares có lượt đầu
    EXPECT_EQ(currentActor->getName(), "Ares");
    
    // Kiểm tra HP và mana đã được reset (nếu có)
    // Lưu ý: Để kiểm tra chi tiết HP/mana cần truy cập Battle trực tiếp
}

TEST(BattleEngineTest, TC07_StartBattle_MultipleCharacters_InitializesCorrectly)
{
    // Arrange: Mỗi team có nhiều nhân vật
    CharacterRoster roster;
    roster.add(makeTestWarrior(1, "Ares", 100, 30));
    roster.add(makeTestWarrior(2, "Thor", 120, 40));
    roster.add(makeTestMage(3, "Luna", 80));
    roster.add(makeTestMage(4, "Merlin", 90));
    
    Team teamA(1, "TeamA");
    teamA.addCharacter(1);
    teamA.addCharacter(2);
    
    Team teamB(2, "TeamB");
    teamB.addCharacter(3);
    teamB.addCharacter(4);
    
    BattleEngine battleEngine;
    
    // Act
    bool selectResult = battleEngine.selectTeams(&teamA, &teamB, roster);
    EXPECT_TRUE(selectResult);
    
    bool startResult = battleEngine.startBattle();
    
    // Assert
    EXPECT_TRUE(startResult);
    EXPECT_TRUE(battleEngine.isInProgress());
    
    // Team A đi trước
    const Character* currentActor = battleEngine.getCurrentActor();
    ASSERT_NE(currentActor, nullptr);
    EXPECT_EQ(currentActor->getId(), 1); // Ares có lượt đầu
}

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

TEST(BattleEngineTest, TC08_PerformAction_ValidCurrentTurn_Succeeds)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeTestWarrior(1, "Ares", 100, 30));
    roster.add(makeTestMage(2, "Luna", 80));
    
    Team teamA(1, "TeamA");
    teamA.addCharacter(1);
    
    Team teamB(2, "TeamB");
    teamB.addCharacter(2);
    
    BattleEngine battleEngine;
    battleEngine.selectTeams(&teamA, &teamB, roster);
    battleEngine.startBattle();
    
    // Xác nhận lượt hiện tại là Ares
    const Character* currentActor = battleEngine.getCurrentActor();
    ASSERT_NE(currentActor, nullptr);
    EXPECT_EQ(currentActor->getId(), 1);
    
    // Act: Ares hành động target Luna (hợp lệ)
    bool actionResult = battleEngine.performCurrentAction(2);
    
    // Assert: Hành động thành công
    EXPECT_TRUE(actionResult);
    
    // Sau khi hành động, lượt chuyển sang team B
    // (Nhưng cần kiểm tra implementation cụ thể)
}

TEST(BattleEngineTest, TC12_PerformAction_DeadTarget_ReturnsFalse)
{
    // Arrange: Tạo nhân vật với HP = 0
    CharacterRoster roster;
    roster.add(makeTestWarrior(1, "Ares", 100, 30));
    
    // Tạo Mage với HP = 0 (đã chết)
    std::unique_ptr<Mage> deadMage = std::make_unique<Mage>(2, "DeadLuna", 0, "Mage", 50, 40, 10, 5);
    roster.add(std::move(deadMage));
    
    Team teamA(1, "TeamA");
    teamA.addCharacter(1);
    
    Team teamB(2, "TeamB");
    teamB.addCharacter(2);
    
    BattleEngine battleEngine;
    battleEngine.selectTeams(&teamA, &teamB, roster);
    battleEngine.startBattle();
    
    // Act: Thử target nhân vật đã chết
    bool actionResult = battleEngine.performCurrentAction(2);
    
    // Assert: Không được phép target nhân vật đã chết
    EXPECT_FALSE(actionResult);
}

// ===========================================================================
// Additional Tests for BattleEngine
// ===========================================================================

TEST(BattleEngineTest, Reset_ClearsBattleState)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeTestWarrior(1, "Ares", 100, 30));
    roster.add(makeTestMage(2, "Luna", 80));
    
    Team teamA(1, "TeamA");
    teamA.addCharacter(1);
    
    Team teamB(2, "TeamB");
    teamB.addCharacter(2);
    
    BattleEngine battleEngine;
    battleEngine.selectTeams(&teamA, &teamB, roster);
    battleEngine.startBattle();
    
    // Act: Reset battle
    battleEngine.reset();
    
    // Assert: Battle trở về trạng thái ban đầu
    EXPECT_FALSE(battleEngine.isInProgress());
    EXPECT_FALSE(battleEngine.isFinished());
    EXPECT_EQ(battleEngine.getCurrentActor(), nullptr);
}

TEST(BattleEngineTest, GetWinnerName_FinishedBattle_ReturnsWinner)
{
    // Arrange: Tạo battle và giả lập kết thúc
    // Lưu ý: Test này có thể phức tạp vì cần mô phỏng battle đến khi kết thúc
    // Có thể test thông qua việc kiểm tra logic của getWinnerName
}

TEST(BattleEngineTest, SelectTeams_ValidTeams_ReturnsTrue)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeTestWarrior(1, "Ares", 100, 30));
    roster.add(makeTestMage(2, "Luna", 80));
    
    Team teamA(1, "TeamA");
    teamA.addCharacter(1);
    
    Team teamB(2, "TeamB");
    teamB.addCharacter(2);
    
    BattleEngine battleEngine;
    
    // Act
    bool result = battleEngine.selectTeams(&teamA, &teamB, roster);
    
    // Assert
    EXPECT_TRUE(result);
}

TEST(BattleEngineTest, SelectTeams_InvalidCharacter_ReturnsFalse)
{
    // Arrange: Team có character không tồn tại trong roster
    CharacterRoster roster;
    roster.add(makeTestWarrior(1, "Ares", 100, 30));
    
    Team teamA(1, "TeamA");
    teamA.addCharacter(1); // Hợp lệ
    
    Team teamB(2, "TeamB");
    teamB.addCharacter(999); // Không tồn tại
    
    BattleEngine battleEngine;
    
    // Act
    bool result = battleEngine.selectTeams(&teamA, &teamB, roster);
    
    // Assert: Không được phép select teams có character không tồn tại
    EXPECT_FALSE(result);
}

TEST(BattleEngineTest, PrintStatus_DoesNotCrash)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeTestWarrior(1, "Ares", 100, 30));
    roster.add(makeTestMage(2, "Luna", 80));
    
    Team teamA(1, "TeamA");
    teamA.addCharacter(1);
    
    Team teamB(2, "TeamB");
    teamB.addCharacter(2);
    
    BattleEngine battleEngine;
    battleEngine.selectTeams(&teamA, &teamB, roster);
    battleEngine.startBattle();
    
    // Act & Assert: Gọi printStatus không crash
    EXPECT_NO_FATAL_FAILURE(battleEngine.printStatus(roster));
}