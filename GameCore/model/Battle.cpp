#include "../pch.h"
#include "Battle.h"
#include <iostream>


Battle::Battle()
    : m_state(BattleState::READY)
    , m_teamAId(0)
    , m_teamBId(0)
    , m_sizeA(0)
    , m_sizeB(0)
    , m_turnNumber(0)
    , m_currentSide(0)
    , m_cursorA(0)
    , m_cursorB(0)
    , m_isSetup(false)
{}

bool Battle::setup(const Team* teamA, const Team* teamB, const CharacterRoster& roster) {
    if (teamA == nullptr || teamB == nullptr)          return false;
    if (teamA->getId() == teamB->getId())               return false;
    if (teamA->getSize() == 0 || teamB->getSize() == 0) return false;

    m_teamAId   = teamA->getId();
    m_teamBId   = teamB->getId();
    m_teamAName = teamA->getName();
    m_teamBName = teamB->getName();

    m_sizeA = teamA->getSize();
    for (int i = 0; i < m_sizeA; ++i) {
        int cid = teamA->getCharacterIds()[i];
        const Character* ch = roster.findById(cid);
        if (ch == nullptr) {
            std::cout << "[WARN] Character ID=" << cid << " khong ton tai trong Roster, bo qua.\n";
            --m_sizeA; --i; continue;
        }
        m_slotsA[i] = CombatantSlot(cid, ch->getMaxHp(), ch->getMaxMana());
    }

    m_sizeB = teamB->getSize();
    for (int i = 0; i < m_sizeB; ++i) {
        int cid = teamB->getCharacterIds()[i];
        const Character* ch = roster.findById(cid);
        if (ch == nullptr) {
            std::cout << "[WARN] Character ID=" << cid << " khong ton tai trong Roster, bo qua.\n";
            --m_sizeB; --i; continue;
        }
        m_slotsB[i] = CombatantSlot(cid, ch->getMaxHp(), ch->getMaxMana());
    }

    if (m_sizeA == 0 || m_sizeB == 0) return false;

    m_state      = BattleState::READY;
    m_turnNumber = 0;
    m_currentSide = 0;
    m_cursorA    = 0;   // cả hai bắt đầu từ NV[0]
    m_cursorB    = 0;
    m_isSetup    = true;
    return true;
}

void Battle::resetCombatants(const CharacterRoster& roster) {
    for (int i = 0; i < m_sizeA; ++i) {
        const Character* ch = roster.findById(m_slotsA[i].characterId);
        if (ch) { m_slotsA[i].currentHp = ch->getMaxHp(); m_slotsA[i].currentMana = ch->getMaxMana(); }
    }
    for (int i = 0; i < m_sizeB; ++i) {
        const Character* ch = roster.findById(m_slotsB[i].characterId);
        if (ch) { m_slotsB[i].currentHp = ch->getMaxHp(); m_slotsB[i].currentMana = ch->getMaxMana(); }
    }
}

BattleState Battle::getState()          const { return m_state; }
void        Battle::setState(BattleState s)   { m_state = s; }

const std::string& Battle::getTeamAName() const { return m_teamAName; }
const std::string& Battle::getTeamBName() const { return m_teamBName; }
int                Battle::getTeamAId()   const { return m_teamAId; }
int                Battle::getTeamBId()   const { return m_teamBId; }

int Battle::getSizeA() const { return m_sizeA; }
int Battle::getSizeB() const { return m_sizeB; }

CombatantSlot& Battle::getSlot(int side, int index) {
    return (side == 0) ? m_slotsA[index] : m_slotsB[index];
}
const CombatantSlot& Battle::getSlot(int side, int index) const {
    return (side == 0) ? m_slotsA[index] : m_slotsB[index];
}

bool Battle::hasAlive(int side) const {
    int size = (side == 0) ? m_sizeA : m_sizeB;
    for (int i = 0; i < size; ++i)
        if (getSlot(side, i).isAlive()) return true;
    return false;
}

int  Battle::getTurnNumber()  const { return m_turnNumber; }
void Battle::incrementTurn()        { ++m_turnNumber; }

int  Battle::getCurrentSide()  const { return m_currentSide; }
void Battle::setCurrentSide(int s)   { m_currentSide = s; }

// getCurrentIndex trả cursor của bên đang đến lượt
int Battle::getCurrentIndex() const {
    return (m_currentSide == 0) ? m_cursorA : m_cursorB;
}

int  Battle::getCursorA() const { return m_cursorA; }
int  Battle::getCursorB() const { return m_cursorB; }
void Battle::setCursorA(int i)  { m_cursorA = i; }
void Battle::setCursorB(int i)  { m_cursorB = i; }

bool Battle::isSetup() const { return m_isSetup; }

void Battle::reset() {
    m_state       = BattleState::READY;
    m_teamAId     = 0;
    m_teamBId     = 0;
    m_teamAName.clear();
    m_teamBName.clear();
    m_sizeA       = 0;
    m_sizeB       = 0;
    m_turnNumber  = 0;
    m_currentSide = 0;
    m_cursorA     = 0;
    m_cursorB     = 0;
    m_isSetup     = false;
}
