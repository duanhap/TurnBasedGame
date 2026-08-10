#include "pch.h"
#include "../GameCore/manager/BattleEngine.h"
#include "../GameCore/manager/CharacterRoster.h"
#include "../GameCore/model/Warrior.h"
#include "../GameCore/model/Mage.h"

// ===========================================================================
// Battle Engine Tests verifying TC-13, TC-14, and TC-15
// ===========================================================================

TEST(BattleEngineTest, TC13_TC14_TC15_IntegrationTest) {
    // Arrange: Create a roster and add characters
    CharacterRoster roster;
    
    // Warrior A1: HP = 10, Attack = 10
    auto wA1 = std::make_unique<Warrior>(1, "Ares", 10, "Warrior", 10);
    // Warrior A2: HP = 100, Attack = 10
    auto wA2 = std::make_unique<Warrior>(2, "Thor", 100, "Warrior", 10);
    // Warrior B1: HP = 100, Attack = 50
    auto wB1 = std::make_unique<Warrior>(3, "Luna", 100, "Warrior", 50);

    roster.add(std::move(wA1));
    roster.add(std::move(wA2));
    roster.add(std::move(wB1));

    // Create Team A
    Team teamA(1, "RedTeam");
    teamA.addCharacter(1);
    teamA.addCharacter(2);

    // Create Team B
    Team teamB(2, "BlueTeam");
    teamB.addCharacter(3);

    BattleEngine engine;
    
    // Verify setup and start
    ASSERT_TRUE(engine.selectTeams(&teamA, &teamB, roster));
    ASSERT_TRUE(engine.startBattle());
    ASSERT_TRUE(engine.isInProgress());

    // --- TURN 1 ---
    // Actor: Ares (1), targets Luna (3)
    EXPECT_EQ(engine.getCurrentActor()->getId(), 1);
    EXPECT_TRUE(engine.performCurrentAction(3));

    // --- TURN 2 ---
    // Actor: Luna (3), targets Ares (1) and KOs him (Ares has 10 HP, Luna deals 50 dmg)
    EXPECT_EQ(engine.getCurrentActor()->getId(), 3);
    EXPECT_TRUE(engine.performCurrentAction(1));

    // --- TURN 3 (TC-13) ---
    // Since Ares (1) is dead, the Battle Engine must skip him and select the next alive character in RedTeam, which is Thor (2)
    const Character* currentActor = engine.getCurrentActor();
    ASSERT_NE(currentActor, nullptr);
    EXPECT_EQ(currentActor->getId(), 2); // Ares (1) skipped, Thor (2) selected! -> TC-13 PASS

    // Thor (2) attacks Luna (3)
    EXPECT_TRUE(engine.performCurrentAction(3));

    // --- TURN 4 ---
    // Actor: Luna (3), attacks Thor (2). Thor HP: 100 - 50 = 50
    EXPECT_EQ(engine.getCurrentActor()->getId(), 3);
    EXPECT_TRUE(engine.performCurrentAction(2));

    // --- TURN 5 ---
    // Actor: Thor (2), attacks Luna (3)
    EXPECT_EQ(engine.getCurrentActor()->getId(), 2);
    EXPECT_TRUE(engine.performCurrentAction(3));

    // --- TURN 6 ---
    // Actor: Luna (3), attacks Thor (2). Thor HP: 50 - 50 = 0 (KO)
    // Since all members of RedTeam (1 and 2) are now KO, the battle must immediately finish.
    EXPECT_EQ(engine.getCurrentActor()->getId(), 3);
    EXPECT_TRUE(engine.performCurrentAction(2));

    // --- TC-14 Verification ---
    // Battle state must transition to FINISHED and winner must be BlueTeam
    EXPECT_TRUE(engine.isFinished());
    EXPECT_FALSE(engine.isInProgress());
    ASSERT_NE(engine.getWinnerName(), nullptr);
    EXPECT_EQ(*(engine.getWinnerName()), "BlueTeam"); // TC-14 PASS

    // --- TC-15 Verification ---
    // Performing an action after the battle is finished must be rejected (returns false)
    EXPECT_FALSE(engine.performCurrentAction(3)); // TC-15 PASS
}

TEST(BattleEngineTest, BattleLoggingVerification) {
    CharacterRoster roster;
    auto wA = std::make_unique<Warrior>(1, "Ares", 100, "Warrior", 25);
    auto wB = std::make_unique<Warrior>(2, "Thor", 100, "Warrior", 15);
    roster.add(std::move(wA));
    roster.add(std::move(wB));

    Team teamA(1, "RedTeam");
    teamA.addCharacter(1);
    Team teamB(2, "BlueTeam");
    teamB.addCharacter(2);

    BattleEngine engine;
    ASSERT_TRUE(engine.selectTeams(&teamA, &teamB, roster));
    ASSERT_TRUE(engine.startBattle());

    // Turn 1: Ares (1) attacks Thor (2)
    ASSERT_TRUE(engine.performCurrentAction(2));

    const auto& log = engine.getBattleLog();
    ASSERT_EQ(log.size(), 1);
    EXPECT_EQ(log[0].turnNumber, 1);
    EXPECT_EQ(log[0].actorId, 1);
    EXPECT_EQ(log[0].actorName, "Ares");
    EXPECT_EQ(log[0].targetId, 2);
    EXPECT_EQ(log[0].targetName, "Thor");
    EXPECT_EQ(log[0].actionType, "DAMAGE");
    EXPECT_EQ(log[0].value, 25);
    EXPECT_EQ(log[0].targetHpAfter, 75);
    EXPECT_TRUE(log[0].targetAliveAfter);

    // Reset battle
    engine.reset();
    EXPECT_TRUE(engine.getBattleLog().empty());
}

