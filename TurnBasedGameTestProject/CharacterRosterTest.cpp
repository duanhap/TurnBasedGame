#include "pch.h"
#include "../GameCore/manager/CharacterRoster.h"
#include "../GameCore/model/Warrior.h"
#include "../GameCore/model/Mage.h"


// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

// Convenience factory so tests stay concise.
static std::unique_ptr<Warrior> makeWarrior(int id, const std::string& name, unsigned int hp = 100)
{
    return std::make_unique<Warrior>(id, name, hp, "Warrior", 20);
}

static std::unique_ptr<Mage> makeMage(int id, const std::string& name, unsigned int hp = 80)
{
    return std::make_unique<Mage>(id, name, hp, "Mage", 50, 30, 10, 5);
}


// ===========================================================================
// add()
// ===========================================================================

TEST(CharacterRosterTest, Add_ValidCharacter_ReturnsId)
{
    // Arrange
    CharacterRoster roster;
    // Act
    int result = roster.add(makeWarrior(1, "Ares"));
    // Assert
    EXPECT_EQ(result, 1);
}

TEST(CharacterRosterTest, Add_ValidCharacter_IncreasesSize)
{
    // Arrange
    CharacterRoster roster;
    // Act
    roster.add(makeWarrior(1, "Ares"));
    // Assert
    EXPECT_EQ(roster.size(), 1);
}

TEST(CharacterRosterTest, Add_MultipleValidCharacters_ReturnsCorrectIds)
{
    // Arrange
    CharacterRoster roster;
    // Act & Assert
    EXPECT_EQ(roster.add(makeWarrior(1, "Ares")),  1);
    EXPECT_EQ(roster.add(makeMage(2,   "Luna")),   2);
    EXPECT_EQ(roster.add(makeWarrior(3, "Thor")),  3);
    EXPECT_EQ(roster.size(), 3);
}

TEST(CharacterRosterTest, Add_Nullptr_ReturnsNegativeOne)
{
    // Arrange
    CharacterRoster roster;
    // Act
    int result = roster.add(nullptr);
    // Assert
    EXPECT_EQ(result, -1);
    EXPECT_EQ(roster.size(), 0);
}

TEST(CharacterRosterTest, Add_ZeroId_ReturnsNegativeOne)
{
    // Arrange
    CharacterRoster roster;
    // Act
    int result = roster.add(makeWarrior(0, "Ares"));
    // Assert
    EXPECT_EQ(result, -1);
    EXPECT_TRUE(roster.isEmpty());
}

TEST(CharacterRosterTest, Add_NegativeId_ReturnsNegativeOne)
{
    // Arrange
    CharacterRoster roster;
    // Act
    int result = roster.add(makeWarrior(-5, "Ares"));
    // Assert
    EXPECT_EQ(result, -1);
    EXPECT_TRUE(roster.isEmpty());
}

TEST(CharacterRosterTest, Add_EmptyName_ReturnsNegativeOne)
{
    // Arrange
    CharacterRoster roster;
    // Act
    int result = roster.add(makeWarrior(1, ""));
    // Assert
    EXPECT_EQ(result, -1);
    EXPECT_TRUE(roster.isEmpty());
}

TEST(CharacterRosterTest, Add_DuplicateId_ReturnsNegativeOne)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    // Act
    int result = roster.add(makeMage(1, "Luna")); // same id
    // Assert
    EXPECT_EQ(result, -1);
    EXPECT_EQ(roster.size(), 1); // original not touched
}

TEST(CharacterRosterTest, Add_DuplicateId_DoesNotReplaceExisting)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    roster.add(makeMage(1, "Luna")); // rejected
    // Assert: the original character with id=1 is still "Ares"
    const Character* c = roster.findById(1);
    ASSERT_NE(c, nullptr);
    EXPECT_EQ(c->getName(), "Ares");
}


// ===========================================================================
// findById() — const overload
// ===========================================================================

TEST(CharacterRosterTest, FindById_ExistingId_ReturnsCorrectCharacter)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(42, "Ares"));
    // Act
    const Character* c = roster.findById(42);
    // Assert
    ASSERT_NE(c, nullptr);
    EXPECT_EQ(c->getId(), 42);
    EXPECT_EQ(c->getName(), "Ares");
}

TEST(CharacterRosterTest, FindById_NonExistingId_ReturnsNull)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    // Act
    const Character* c = roster.findById(999);
    // Assert
    EXPECT_EQ(c, nullptr);
}

TEST(CharacterRosterTest, FindById_EmptyRoster_ReturnsNull)
{
    // Arrange
    CharacterRoster roster;
    // Act
    const Character* c = roster.findById(1);
    // Assert
    EXPECT_EQ(c, nullptr);
}

TEST(CharacterRosterTest, FindById_CorrectCharacterAmongMany)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    roster.add(makeMage(2,   "Luna"));
    roster.add(makeWarrior(3, "Thor"));
    // Act
    const Character* c = roster.findById(2);
    // Assert
    ASSERT_NE(c, nullptr);
    EXPECT_EQ(c->getName(), "Luna");
}


// ===========================================================================
// findById() — mutable overload
// ===========================================================================

TEST(CharacterRosterTest, FindByIdMutable_ExistingId_ReturnsNonConstPointer)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares", 100));
    // Act
    Character* c = roster.findById(1);
    // Assert
    ASSERT_NE(c, nullptr);
    // Verify we can mutate through it (e.g., reduce hp)
    c->reduceHp(30);
    EXPECT_EQ(c->getMaxHp(), 70);
}

TEST(CharacterRosterTest, FindByIdMutable_NonExistingId_ReturnsNull)
{
    // Arrange
    CharacterRoster roster;
    // Act
    Character* c = roster.findById(7);
    // Assert
    EXPECT_EQ(c, nullptr);
}


// ===========================================================================
// remove()
// ===========================================================================

TEST(CharacterRosterTest, Remove_ExistingId_ReturnsTrue)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    // Act
    bool result = roster.remove(1);
    // Assert
    EXPECT_TRUE(result);
}

TEST(CharacterRosterTest, Remove_ExistingId_DecreasesSize)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    roster.add(makeMage(2, "Luna"));
    // Act
    roster.remove(1);
    // Assert
    EXPECT_EQ(roster.size(), 1);
}

TEST(CharacterRosterTest, Remove_ExistingId_CharacterNoLongerFindable)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    // Act
    roster.remove(1);
    // Assert
    EXPECT_EQ(roster.findById(1), nullptr);
}

TEST(CharacterRosterTest, Remove_NonExistingId_ReturnsFalse)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    // Act
    bool result = roster.remove(999);
    // Assert
    EXPECT_FALSE(result);
    EXPECT_EQ(roster.size(), 1); // unchanged
}

TEST(CharacterRosterTest, Remove_EmptyRoster_ReturnsFalse)
{
    // Arrange
    CharacterRoster roster;
    // Act
    bool result = roster.remove(1);
    // Assert
    EXPECT_FALSE(result);
}

TEST(CharacterRosterTest, Remove_OnlyRemovesTargetCharacter)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    roster.add(makeMage(2,   "Luna"));
    roster.add(makeWarrior(3, "Thor"));
    // Act
    roster.remove(2);
    // Assert
    EXPECT_EQ(roster.size(), 2);
    EXPECT_NE(roster.findById(1), nullptr);
    EXPECT_EQ(roster.findById(2), nullptr);
    EXPECT_NE(roster.findById(3), nullptr);
}

TEST(CharacterRosterTest, Remove_AllCharacters_RosterBecomesEmpty)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    roster.add(makeMage(2, "Luna"));
    // Act
    roster.remove(1);
    roster.remove(2);
    // Assert
    EXPECT_TRUE(roster.isEmpty());
    EXPECT_EQ(roster.size(), 0);
}

TEST(CharacterRosterTest, Remove_ThenAddSameId_Succeeds)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    roster.remove(1);
    // Act: re-add with the same id should now succeed
    int result = roster.add(makeWarrior(1, "NewAres"));
    // Assert
    EXPECT_EQ(result, 1);
    EXPECT_EQ(roster.size(), 1);
}


// ===========================================================================
// isEmpty()
// ===========================================================================

TEST(CharacterRosterTest, IsEmpty_DefaultConstructed_ReturnsTrue)
{
    CharacterRoster roster;
    EXPECT_TRUE(roster.isEmpty());
}

TEST(CharacterRosterTest, IsEmpty_AfterAddingCharacter_ReturnsFalse)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    // Assert
    EXPECT_FALSE(roster.isEmpty());
}

TEST(CharacterRosterTest, IsEmpty_AfterAddAndRemove_ReturnsTrue)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    roster.remove(1);
    // Assert
    EXPECT_TRUE(roster.isEmpty());
}


// ===========================================================================
// size()
// ===========================================================================

TEST(CharacterRosterTest, Size_EmptyRoster_ReturnsZero)
{
    CharacterRoster roster;
    EXPECT_EQ(roster.size(), 0);
}

TEST(CharacterRosterTest, Size_AfterAddingThree_ReturnsThree)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    roster.add(makeMage(2,   "Luna"));
    roster.add(makeWarrior(3, "Thor"));
    // Assert
    EXPECT_EQ(roster.size(), 3);
}

TEST(CharacterRosterTest, Size_AfterFailedAdd_Unchanged)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    roster.add(nullptr);        // rejected
    roster.add(makeWarrior(1, "Duplicate")); // rejected
    // Assert
    EXPECT_EQ(roster.size(), 1);
}


// ===========================================================================
// getAllCharacters()
// ===========================================================================

TEST(CharacterRosterTest, GetAllCharacters_EmptyRoster_ReturnsEmptyVector)
{
    CharacterRoster roster;
    EXPECT_TRUE(roster.getAllCharacters().empty());
}

TEST(CharacterRosterTest, GetAllCharacters_ReturnsAllInsertedCharacters)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    roster.add(makeMage(2,   "Luna"));
    roster.add(makeWarrior(3, "Thor"));
    // Act
    auto all = roster.getAllCharacters();
    // Assert
    EXPECT_EQ(all.size(), 3u);
}

TEST(CharacterRosterTest, GetAllCharacters_PointersAreNonNull)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    roster.add(makeMage(2,   "Luna"));
    // Act
    auto all = roster.getAllCharacters();
    // Assert
    for (const Character* c : all) {
        EXPECT_NE(c, nullptr);
    }
}

TEST(CharacterRosterTest, GetAllCharacters_ContainsCorrectIds)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(10, "Ares"));
    roster.add(makeMage(20,   "Luna"));
    // Act
    auto all = roster.getAllCharacters();
    // Assert: collect ids and check both are present
    std::vector<int> ids;
    for (const Character* c : all) {
        ids.push_back(c->getId());
    }
    EXPECT_NE(std::find(ids.begin(), ids.end(), 10), ids.end());
    EXPECT_NE(std::find(ids.begin(), ids.end(), 20), ids.end());
}

TEST(CharacterRosterTest, GetAllCharacters_AfterRemove_DoesNotContainRemovedCharacter)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    roster.add(makeMage(2,   "Luna"));
    roster.remove(1);
    // Act
    auto all = roster.getAllCharacters();
    // Assert
    EXPECT_EQ(all.size(), 1u);
    EXPECT_EQ(all[0]->getId(), 2);
}


// ===========================================================================
// hasCharacter()
// ===========================================================================

TEST(CharacterRosterTest, HasCharacter_ExistingId_ReturnsTrue)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(5, "Ares"));
    // Assert
    EXPECT_TRUE(roster.hasCharacter(5));
}

TEST(CharacterRosterTest, HasCharacter_NonExistingId_ReturnsFalse)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(5, "Ares"));
    // Assert
    EXPECT_FALSE(roster.hasCharacter(99));
}

TEST(CharacterRosterTest, HasCharacter_EmptyRoster_ReturnsFalse)
{
    CharacterRoster roster;
    EXPECT_FALSE(roster.hasCharacter(1));
}

TEST(CharacterRosterTest, HasCharacter_AfterRemove_ReturnsFalse)
{
    // Arrange
    CharacterRoster roster;
    roster.add(makeWarrior(1, "Ares"));
    roster.remove(1);
    // Assert
    EXPECT_FALSE(roster.hasCharacter(1));
}
