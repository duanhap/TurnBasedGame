#include "pch.h"
#include "../GameCore/manager/CharacterRoster.h"
#include "../GameCore/manager/TeamManager.h"
#include "../GameCore/manager/DataFileManager.h"
#include "../GameCore/model/Warrior.h"
#include "../GameCore/model/Mage.h"
#include <fstream>
#include <sstream>

// Helper to create characters
static std::unique_ptr<Warrior> makeTestWarrior(int id, const std::string& name)
{
    return std::make_unique<Warrior>(id, name, 100, "Warrior", 20);
}

// ===========================================================================
// Team Class Tests
// ===========================================================================

TEST(TeamTest, AddCharacter_ValidId_Succeeds)
{
    Team team(1, "Alpha");
    EXPECT_TRUE(team.addCharacter(10));
    EXPECT_EQ(team.getCharacterIds().size(), 1);
    EXPECT_EQ(team.getCharacterIds()[0], 10);
}

TEST(TeamTest, AddCharacter_NegativeId_Fails)
{
    Team team(1, "Alpha");
    EXPECT_FALSE(team.addCharacter(0));
    EXPECT_FALSE(team.addCharacter(-5));
    EXPECT_TRUE(team.getCharacterIds().empty());
}

TEST(TeamTest, AddCharacter_DuplicateId_Fails)
{
    Team team(1, "Alpha");
    EXPECT_TRUE(team.addCharacter(10));
    EXPECT_FALSE(team.addCharacter(10));
    EXPECT_EQ(team.getCharacterIds().size(), 1);
}

TEST(TeamTest, AddCharacter_ExceedLimit_Fails)
{
    Team team(1, "Alpha");
    for (int i = 1; i <= 5; ++i) {
        EXPECT_TRUE(team.addCharacter(i));
    }
    // 6th character should fail
    EXPECT_FALSE(team.addCharacter(6));
    EXPECT_EQ(team.getCharacterIds().size(), 5);
}

TEST(TeamTest, RemoveCharacter_ExistingId_Succeeds)
{
    Team team(1, "Alpha");
    team.addCharacter(10);
    team.addCharacter(20);
    EXPECT_TRUE(team.removeCharacter(10));
    EXPECT_FALSE(team.hasCharacter(10));
    EXPECT_TRUE(team.hasCharacter(20));
}

TEST(TeamTest, RemoveCharacter_NonExistingId_Fails)
{
    Team team(1, "Alpha");
    team.addCharacter(10);
    EXPECT_FALSE(team.removeCharacter(20));
    EXPECT_EQ(team.getCharacterIds().size(), 1);
}

// ===========================================================================
// TeamManager Class Tests
// ===========================================================================

TEST(TeamManagerTest, CreateTeam_Valid_Succeeds)
{
    TeamManager manager;
    EXPECT_TRUE(manager.createTeam(1, "TeamA"));
    EXPECT_TRUE(manager.hasTeamId(1));
    EXPECT_TRUE(manager.hasTeamName("TeamA"));
    EXPECT_EQ(manager.getTeams().size(), 1);
}

TEST(TeamManagerTest, CreateTeam_DuplicateIdOrName_Fails)
{
    TeamManager manager;
    EXPECT_TRUE(manager.createTeam(1, "TeamA"));
    // Duplicate ID
    EXPECT_FALSE(manager.createTeam(1, "TeamB"));
    // Duplicate Name
    EXPECT_FALSE(manager.createTeam(2, "TeamA"));
}

TEST(TeamManagerTest, AddCharacterToTeam_InRoster_Succeeds)
{
    TeamManager manager;
    manager.createTeam(1, "TeamA");

    CharacterRoster roster;
    roster.add(makeTestWarrior(10, "Ares"));

    EXPECT_TRUE(manager.addCharacterToTeam(1, 10, roster));
    EXPECT_TRUE(manager.getTeam(1)->hasCharacter(10));
}

TEST(TeamManagerTest, AddCharacterToTeam_NotInRoster_Fails)
{
    TeamManager manager;
    manager.createTeam(1, "TeamA");

    CharacterRoster roster;
    // Roster is empty, character 10 doesn't exist in roster

    EXPECT_FALSE(manager.addCharacterToTeam(1, 10, roster));
    EXPECT_FALSE(manager.getTeam(1)->hasCharacter(10));
}

TEST(TeamManagerTest, RemoveCharacterFromAllTeams_RemovesFromAll)
{
    TeamManager manager;
    manager.createTeam(1, "TeamA");
    manager.createTeam(2, "TeamB");

    CharacterRoster roster;
    roster.add(makeTestWarrior(10, "Ares"));

    manager.addCharacterToTeam(1, 10, roster);
    manager.addCharacterToTeam(2, 10, roster);

    manager.removeCharacterFromAllTeams(10);
    EXPECT_FALSE(manager.getTeam(1)->hasCharacter(10));
    EXPECT_FALSE(manager.getTeam(2)->hasCharacter(10));
}

// ===========================================================================
// DataFileManager Class Tests
// ===========================================================================

TEST(DataFileManagerTest, LoadAndSaveTeams_ValidData_Succeeds)
{
    std::string testFile = "test_teams.txt";
    
    // Setup roster
    CharacterRoster roster;
    roster.add(makeTestWarrior(10, "Ares"));
    roster.add(makeTestWarrior(20, "Thor"));

    // Write manual test data
    std::ofstream outfile(testFile);
    outfile << "# teamId|teamName|characterId1,characterId2\n";
    outfile << "1|TeamAlpha|10,20\n";
    outfile << "2|TeamBeta|10\n";
    outfile.close();

    TeamManager manager;
    EXPECT_TRUE(DataFileManager::loadTeams(testFile, manager, roster));

    // Verify loaded data
    ASSERT_EQ(manager.getTeams().size(), 2);
    const Team* t1 = manager.getTeam(1);
    ASSERT_NE(t1, nullptr);
    EXPECT_EQ(t1->getName(), "TeamAlpha");
    EXPECT_TRUE(t1->hasCharacter(10));
    EXPECT_TRUE(t1->hasCharacter(20));

    const Team* t2 = manager.getTeam(2);
    ASSERT_NE(t2, nullptr);
    EXPECT_EQ(t2->getName(), "TeamBeta");
    EXPECT_TRUE(t2->hasCharacter(10));
    EXPECT_FALSE(t2->hasCharacter(20));

    // Save teams back
    std::string testSaveFile = "test_save_teams.txt";
    EXPECT_TRUE(DataFileManager::saveTeams(testSaveFile, manager));

    // Load back saved teams to verify saving format
    TeamManager manager2;
    EXPECT_TRUE(DataFileManager::loadTeams(testSaveFile, manager2, roster));
    ASSERT_EQ(manager2.getTeams().size(), 2);
    EXPECT_EQ(manager2.getTeam(1)->getName(), "TeamAlpha");
    EXPECT_TRUE(manager2.getTeam(1)->hasCharacter(10));
    EXPECT_TRUE(manager2.getTeam(1)->hasCharacter(20));

    // Clean up files
    std::remove(testFile.c_str());
    std::remove(testSaveFile.c_str());
}

TEST(DataFileManagerTest, LoadTeams_InvalidCharacters_SkipsInvalidAndContinues)
{
    std::string testFile = "test_invalid_teams.txt";

    CharacterRoster roster;
    roster.add(makeTestWarrior(10, "Ares")); // only ID 10 exists in roster

    std::ofstream outfile(testFile);
    outfile << "1|TeamAlpha|10,99\n"; // 99 does not exist in roster
    outfile.close();

    TeamManager manager;
    EXPECT_TRUE(DataFileManager::loadTeams(testFile, manager, roster));

    ASSERT_EQ(manager.getTeams().size(), 1);
    const Team* t1 = manager.getTeam(1);
    ASSERT_NE(t1, nullptr);
    EXPECT_TRUE(t1->hasCharacter(10));
    EXPECT_FALSE(t1->hasCharacter(99)); // 99 should have been skipped/ignored

    std::remove(testFile.c_str());
}
