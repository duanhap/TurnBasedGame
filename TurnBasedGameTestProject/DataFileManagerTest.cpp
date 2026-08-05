#include "pch.h"
#include "../GameCore/manager/DataFileManager.h"
#include "../GameCore/manager/CharacterRoster.h"
#include "../GameCore/model/Warrior.h"
#include "../GameCore/model/Mage.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>
#include <filesystem>


// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

// Creates a temporary file with the given content and returns its path.
// The caller is responsible for deleting the file after use.
static std::string createTempFile(const std::string& content) {
    // Use std::filesystem::temp_directory_path for a safe temp location
    std::string filepath = (std::filesystem::temp_directory_path() / "test_characters.txt").string();
    std::ofstream out(filepath);
    out << content;
    out.close();
    return filepath;
}

// Reads the entire content of a file into a string.
static std::string readFileContent(const std::string& filepath) {
    std::ifstream in(filepath);
    std::string content((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
    return content;
}

// Convenience factory so tests stay concise.
static std::unique_ptr<Warrior> makeWarrior(int id, const std::string& name, unsigned int hp, unsigned int atk)
{
    return std::make_unique<Warrior>(id, name, hp, "WARRIOR", atk);
}

static std::unique_ptr<Mage> makeMage(int id, const std::string& name, unsigned int hp,
    unsigned int maxMana, unsigned int spellDmg, unsigned int manaCost, unsigned int fallback)
{
    return std::make_unique<Mage>(id, name, hp, "MAGE", maxMana, spellDmg, manaCost, fallback);
}


// ===========================================================================
// loadCharacters()
// ===========================================================================

TEST(DataFileManagerLoadCharactersTest, Load_ValidWarrior_AddsToRoster)
{
    // Arrange
    std::string filepath = createTempFile("WARRIOR|1|Ares|100|30\n");
    CharacterRoster roster;
    // Act
    bool result = DataFileManager::loadCharacters(filepath, roster);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(roster.size(), 1);
    const Character* c = roster.findById(1);
    ASSERT_NE(c, nullptr);
    EXPECT_EQ(c->getName(), "Ares");
    EXPECT_EQ(c->getMaxHp(), 100);
    EXPECT_EQ(c->getType(), "WARRIOR");
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerLoadCharactersTest, Load_ValidMage_AddsToRoster)
{
    // Arrange
    std::string filepath = createTempFile("MAGE|2|Luna|80|50|40|10|5\n");
    CharacterRoster roster;
    // Act
    bool result = DataFileManager::loadCharacters(filepath, roster);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(roster.size(), 1);
    const Character* c = roster.findById(2);
    ASSERT_NE(c, nullptr);
    EXPECT_EQ(c->getName(), "Luna");
    EXPECT_EQ(c->getMaxHp(), 80);
    EXPECT_EQ(c->getType(), "MAGE");
    const Mage* m = dynamic_cast<const Mage*>(c);
    ASSERT_NE(m, nullptr);
    EXPECT_EQ(m->getMaxMana(), 50u);
    EXPECT_EQ(m->getSpellDamage(), 40u);
    EXPECT_EQ(m->getManaCost(), 10u);
    EXPECT_EQ(m->getFallbackDamage(), 5u);
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerLoadCharactersTest, Load_MixedWarriorAndMage_AddsAll)
{
    // Arrange
    std::string content =
        "WARRIOR|101|Ares|100|30\n"
        "MAGE|102|Luna|80|10|40|10|10\n";
    std::string filepath = createTempFile(content);
    CharacterRoster roster;
    // Act
    bool result = DataFileManager::loadCharacters(filepath, roster);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(roster.size(), 2);
    EXPECT_NE(roster.findById(101), nullptr);
    EXPECT_NE(roster.findById(102), nullptr);
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerLoadCharactersTest, Load_CommentsAndEmptyLines_Skipped)
{
    // Arrange
    std::string content =
        "# This is a comment\n"
        "\n"
        "WARRIOR|1|Ares|100|30\n"
        "# Another comment\n"
        "\n"
        "MAGE|2|Luna|80|50|40|10|5\n";
    std::string filepath = createTempFile(content);
    CharacterRoster roster;
    // Act
    DataFileManager::loadCharacters(filepath, roster);
    // Assert
    EXPECT_EQ(roster.size(), 2);
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerLoadCharactersTest, Load_DuplicateId_SkipsDuplicate)
{
    // Arrange
    std::string content =
        "WARRIOR|1|Ares|100|30\n"
        "WARRIOR|1|Zeus|120|40\n"; // duplicate id
    std::string filepath = createTempFile(content);
    CharacterRoster roster;
    // Act
    DataFileManager::loadCharacters(filepath, roster);
    // Assert
    EXPECT_EQ(roster.size(), 1);
    EXPECT_EQ(roster.findById(1)->getName(), "Ares"); // first one kept
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerLoadCharactersTest, Load_InvalidId_SkipsLine)
{
    // Arrange
    std::string content =
        "WARRIOR|abc|Ares|100|30\n"   // non-numeric id
        "WARRIOR|0|Zeus|100|30\n"      // zero id
        "WARRIOR|-1|Hades|100|30\n"    // negative id
        "WARRIOR|1|Valid|100|30\n";    // valid
    std::string filepath = createTempFile(content);
    CharacterRoster roster;
    // Act
    DataFileManager::loadCharacters(filepath, roster);
    // Assert
    EXPECT_EQ(roster.size(), 1);
    EXPECT_EQ(roster.findById(1)->getName(), "Valid");
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerLoadCharactersTest, Load_EmptyName_SkipsLine)
{
    // Arrange
    std::string content = "WARRIOR|1||100|30\n"; // empty name
    std::string filepath = createTempFile(content);
    CharacterRoster roster;
    // Act
    DataFileManager::loadCharacters(filepath, roster);
    // Assert
    EXPECT_EQ(roster.size(), 0);
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerLoadCharactersTest, Load_InvalidMaxHp_SkipsLine)
{
    // Arrange
    std::string content =
        "WARRIOR|1|Ares|0|30\n"      // maxHp = 0
        "WARRIOR|2|Zeus|abc|30\n";    // non-numeric maxHp
    std::string filepath = createTempFile(content);
    CharacterRoster roster;
    // Act
    DataFileManager::loadCharacters(filepath, roster);
    // Assert
    EXPECT_EQ(roster.size(), 0);
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerLoadCharactersTest, Load_InvalidAttackPower_SkipsLine)
{
    // Arrange
    std::string content = "WARRIOR|1|Ares|100|0\n"; // attackPower = 0
    std::string filepath = createTempFile(content);
    CharacterRoster roster;
    // Act
    DataFileManager::loadCharacters(filepath, roster);
    // Assert
    EXPECT_EQ(roster.size(), 0);
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerLoadCharactersTest, Load_WarriorWrongFieldCount_SkipsLine)
{
    // Arrange
    std::string content =
        "WARRIOR|1|Ares|100\n"             // too few fields (4)
        "WARRIOR|2|Zeus|100|30|extra\n";   // too many fields (6)
    std::string filepath = createTempFile(content);
    CharacterRoster roster;
    // Act
    DataFileManager::loadCharacters(filepath, roster);
    // Assert
    EXPECT_EQ(roster.size(), 0);
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerLoadCharactersTest, Load_MageWrongFieldCount_SkipsLine)
{
    // Arrange
    std::string content =
        "MAGE|1|Luna|80|50|40|10\n"             // too few fields (7)
        "MAGE|2|Iris|80|50|40|10|5|extra\n";    // too many fields (9)
    std::string filepath = createTempFile(content);
    CharacterRoster roster;
    // Act
    DataFileManager::loadCharacters(filepath, roster);
    // Assert
    EXPECT_EQ(roster.size(), 0);
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerLoadCharactersTest, Load_UnknownType_SkipsLine)
{
    // Arrange
    std::string content = "ARCHER|1|Robin|100|50\n";
    std::string filepath = createTempFile(content);
    CharacterRoster roster;
    // Act
    DataFileManager::loadCharacters(filepath, roster);
    // Assert
    EXPECT_EQ(roster.size(), 0);
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerLoadCharactersTest, Load_NonExistentFile_ReturnsTrueEmptyRoster)
{
    // Arrange
    CharacterRoster roster;
    // Act
    bool result = DataFileManager::loadCharacters("nonexistent_path_12345.txt", roster);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(roster.size(), 0);
}

TEST(DataFileManagerLoadCharactersTest, Load_EmptyFile_ReturnsTrueEmptyRoster)
{
    // Arrange
    std::string filepath = createTempFile("");
    CharacterRoster roster;
    // Act
    bool result = DataFileManager::loadCharacters(filepath, roster);
    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(roster.size(), 0);
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerLoadCharactersTest, Load_WhitespaceAroundFields_TrimsCorrectly)
{
    // Arrange
    std::string content = "WARRIOR | 1 | Ares | 100 | 30\n";
    std::string filepath = createTempFile(content);
    CharacterRoster roster;
    // Act
    DataFileManager::loadCharacters(filepath, roster);
    // Assert
    EXPECT_EQ(roster.size(), 1);
    const Character* c = roster.findById(1);
    ASSERT_NE(c, nullptr);
    EXPECT_EQ(c->getName(), "Ares");
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerLoadCharactersTest, Load_MageWithZeroMana_LoadsSuccessfully)
{
    // Arrange: maxMana=0 is valid (non-negative)
    std::string content = "MAGE|3|NoMana|80|0|40|10|5\n";
    std::string filepath = createTempFile(content);
    CharacterRoster roster;
    // Act
    DataFileManager::loadCharacters(filepath, roster);
    // Assert
    EXPECT_EQ(roster.size(), 1);
    const Mage* m = dynamic_cast<const Mage*>(roster.findById(3));
    ASSERT_NE(m, nullptr);
    EXPECT_EQ(m->getMaxMana(), 0u);
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerLoadCharactersTest, Load_MixedValidAndInvalid_OnlyLoadsValid)
{
    // Arrange
    std::string content =
        "WARRIOR|1|Ares|100|30\n"       // valid
        "INVALID LINE\n"                 // invalid
        "MAGE|2|Luna|80|50|40|10|5\n"   // valid
        "WARRIOR|abc|Bad|100|30\n"       // invalid id
        "WARRIOR|3|Thor|200|50\n";      // valid
    std::string filepath = createTempFile(content);
    CharacterRoster roster;
    // Act
    DataFileManager::loadCharacters(filepath, roster);
    // Assert
    EXPECT_EQ(roster.size(), 3);
    EXPECT_NE(roster.findById(1), nullptr);
    EXPECT_NE(roster.findById(2), nullptr);
    EXPECT_NE(roster.findById(3), nullptr);
    // Cleanup
    std::remove(filepath.c_str());
}


// ===========================================================================
// saveCharacters()
// ===========================================================================

TEST(DataFileManagerSaveCharactersTest, Save_SingleWarrior_WritesCorrectFormat)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares", 100, 30));
    std::string filepath = (std::filesystem::temp_directory_path() / "test_save_chars.txt").string();
    // Act
    bool result = DataFileManager::saveCharacters(filepath, roster);
    // Assert
    EXPECT_TRUE(result);
    std::string content = readFileContent(filepath);
    EXPECT_NE(content.find("WARRIOR|1|Ares|100|30"), std::string::npos);
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerSaveCharactersTest, Save_SingleMage_WritesCorrectFormat)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeMage(2, "Luna", 80, 50, 40, 10, 5));
    std::string filepath = (std::filesystem::temp_directory_path() / "test_save_chars.txt").string();
    // Act
    bool result = DataFileManager::saveCharacters(filepath, roster);
    // Assert
    EXPECT_TRUE(result);
    std::string content = readFileContent(filepath);
    EXPECT_NE(content.find("MAGE|2|Luna|80|50|40|10|5"), std::string::npos);
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerSaveCharactersTest, Save_MixedCharacters_WritesAll)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(101, "Ares", 100, 30));
    roster.add(makeMage(102, "Luna", 80, 10, 40, 10, 10));
    std::string filepath = (std::filesystem::temp_directory_path() / "test_save_chars.txt").string();
    // Act
    bool result = DataFileManager::saveCharacters(filepath, roster);
    // Assert
    EXPECT_TRUE(result);
    std::string content = readFileContent(filepath);
    EXPECT_NE(content.find("WARRIOR|101|Ares|100|30"), std::string::npos);
    EXPECT_NE(content.find("MAGE|102|Luna|80|10|40|10|10"), std::string::npos);
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerSaveCharactersTest, Save_EmptyRoster_WritesOnlyComments)
{
    // Arrange
    CharacterRoster roster;
    std::string filepath = (std::filesystem::temp_directory_path() / "test_save_chars.txt").string();
    // Act
    bool result = DataFileManager::saveCharacters(filepath, roster);
    // Assert
    EXPECT_TRUE(result);
    std::string content = readFileContent(filepath);
    // Should contain comment headers
    EXPECT_NE(content.find("#"), std::string::npos);
    // Should NOT contain any actual data lines (lines starting with WARRIOR| or MAGE| without #)
    // Comment headers like "# WARRIOR|id|..." are expected, but no data lines like "WARRIOR|1|..."
    std::istringstream stream(content);
    std::string line;
    while (std::getline(stream, line)) {
        // Skip comment and empty lines
        if (line.empty() || line[0] == '#') continue;
        // Any non-comment line would be character data — there should be none
        FAIL() << "Unexpected data line in empty roster save: " << line;
    }
    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerSaveCharactersTest, Save_IncludesCommentHeaders)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares", 100, 30));
    std::string filepath = (std::filesystem::temp_directory_path() / "test_save_chars.txt").string();
    // Act
    DataFileManager::saveCharacters(filepath, roster);
    // Assert
    std::string content = readFileContent(filepath);
    EXPECT_NE(content.find("# Dinh dang du lieu Nhan vat"), std::string::npos);
    EXPECT_NE(content.find("# WARRIOR|id|name|maxHp|attackPower"), std::string::npos);
    EXPECT_NE(content.find("# MAGE|id|name|maxHp|maxMana|spellDamage|manaCost|fallbackDamage"), std::string::npos);
    // Cleanup
    std::remove(filepath.c_str());
}


// ===========================================================================
// Round-trip: save then load
// ===========================================================================

TEST(DataFileManagerRoundTripTest, SaveThenLoad_PreservesWarriorData)
{
    // Arrange: build a roster, save it, then load into a new roster
    CharacterRoster original;
    original.add(makeWarrior(1, "Ares", 100, 30));
    original.add(makeWarrior(2, "Thor", 200, 50));
    std::string filepath = (std::filesystem::temp_directory_path() / "test_roundtrip.txt").string();

    // Act: save then load
    DataFileManager::saveCharacters(filepath, original);
    CharacterRoster loaded;
    DataFileManager::loadCharacters(filepath, loaded);

    // Assert
    EXPECT_EQ(loaded.size(), 2);

    const Character* c1 = loaded.findById(1);
    ASSERT_NE(c1, nullptr);
    EXPECT_EQ(c1->getName(), "Ares");
    EXPECT_EQ(c1->getMaxHp(), 100);
    const Warrior* w1 = dynamic_cast<const Warrior*>(c1);
    ASSERT_NE(w1, nullptr);
    EXPECT_EQ(w1->getAttackPower(), 30u);

    const Character* c2 = loaded.findById(2);
    ASSERT_NE(c2, nullptr);
    EXPECT_EQ(c2->getName(), "Thor");
    EXPECT_EQ(c2->getMaxHp(), 200);
    const Warrior* w2 = dynamic_cast<const Warrior*>(c2);
    ASSERT_NE(w2, nullptr);
    EXPECT_EQ(w2->getAttackPower(), 50u);

    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerRoundTripTest, SaveThenLoad_PreservesMageData)
{
    // Arrange
    CharacterRoster original;
    original.add(makeMage(10, "Luna", 80, 50, 40, 10, 5));
    std::string filepath = (std::filesystem::temp_directory_path() / "test_roundtrip.txt").string();

    // Act
    DataFileManager::saveCharacters(filepath, original);
    CharacterRoster loaded;
    DataFileManager::loadCharacters(filepath, loaded);

    // Assert
    EXPECT_EQ(loaded.size(), 1);
    const Mage* m = dynamic_cast<const Mage*>(loaded.findById(10));
    ASSERT_NE(m, nullptr);
    EXPECT_EQ(m->getName(), "Luna");
    EXPECT_EQ(m->getMaxHp(), 80);
    EXPECT_EQ(m->getMaxMana(), 50u);
    EXPECT_EQ(m->getSpellDamage(), 40u);
    EXPECT_EQ(m->getManaCost(), 10u);
    EXPECT_EQ(m->getFallbackDamage(), 5u);

    // Cleanup
    std::remove(filepath.c_str());
}

TEST(DataFileManagerRoundTripTest, SaveThenLoad_MixedCharacters_FullRoundTrip)
{
    // Arrange
    CharacterRoster original;
    original.add(makeWarrior(101, "Ares", 100, 30));
    original.add(makeMage(102, "Luna", 80, 10, 40, 10, 10));
    original.add(makeWarrior(103, "Thor", 150, 45));
    std::string filepath = (std::filesystem::temp_directory_path() / "test_roundtrip.txt").string();

    // Act
    DataFileManager::saveCharacters(filepath, original);
    CharacterRoster loaded;
    DataFileManager::loadCharacters(filepath, loaded);

    // Assert
    EXPECT_EQ(loaded.size(), 3);
    EXPECT_NE(loaded.findById(101), nullptr);
    EXPECT_NE(loaded.findById(102), nullptr);
    EXPECT_NE(loaded.findById(103), nullptr);

    // Verify types are correct
    EXPECT_NE(dynamic_cast<const Warrior*>(loaded.findById(101)), nullptr);
    EXPECT_NE(dynamic_cast<const Mage*>(loaded.findById(102)), nullptr);
    EXPECT_NE(dynamic_cast<const Warrior*>(loaded.findById(103)), nullptr);

    // Cleanup
    std::remove(filepath.c_str());
}
