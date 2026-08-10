#include "../pch.h"
#include "Battle.h"
#include "Warrior.h"
#include "Mage.h"
#include "Archer.h"
#include "Healer.h"
#include <iostream>
#include <unordered_set>


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

bool Battle::setup(const Team* teamA, const Team* teamB, CharacterRoster& roster) {
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
        Character* src = roster.findById(cid);
        if (src == nullptr) {
            std::cout << "[WARN] Character ID=" << cid << " khong ton tai trong Roster, bo qua.\n";
            --m_sizeA; --i; continue;
        }
        // Clone a concrete instance so each team has independent battle state
        Character* clone = nullptr;
        if (const Warrior* w = dynamic_cast<const Warrior*>(src)) {
            clone = new Warrior(w->getId(), w->getName(), w->getMaxHp(), w->getType(), static_cast<int>(w->getAttackPower()));
        }
        else if (const Mage* m = dynamic_cast<const Mage*>(src)) {
            clone = new Mage(m->getId(), m->getName(), m->getMaxHp(), m->getType(), m->getMaxMana(), m->getSpellDamage(), m->getManaCost(), m->getFallbackDamage());
        }
        else if (const Archer* a = dynamic_cast<const Archer*>(src)) {
            clone = new Archer(a->getId(), a->getName(), a->getMaxHp(), a->getType(), static_cast<int>(a->getNormalDamage()), static_cast<int>(a->getCriticalDamage()));
        }
        else if (const Healer* h = dynamic_cast<const Healer*>(src)) {
            clone = new Healer(h->getId(), h->getName(), h->getMaxHp(), h->getHealingPower());
        }
        if (clone == nullptr) {
            std::cout << "[WARN] Loai nhan vat ID=" << cid << " khong ho tro cloning, bo qua.\n";
            --m_sizeA; --i; continue;
        }
        // Preserve current battle state from roster for the clone
        // Reset will be called before battle start, but copy currentHp/currentMana if any
        // Use available public methods by resetting then applying differences if needed
        m_slotsA[i] = clone;
        m_ownedA[i] = true;
    }

    m_sizeB = teamB->getSize();
    for (int i = 0; i < m_sizeB; ++i) {
        int cid = teamB->getCharacterIds()[i];
        Character* src = roster.findById(cid);
        if (src == nullptr) {
            std::cout << "[WARN] Character ID=" << cid << " khong ton tai trong Roster, bo qua.\n";
            --m_sizeB; --i; continue;
        }
        Character* clone = nullptr;
        if (const Warrior* w = dynamic_cast<const Warrior*>(src)) {
            clone = new Warrior(w->getId(), w->getName(), w->getMaxHp(), w->getType(), static_cast<int>(w->getAttackPower()));
        }
        else if (const Mage* m = dynamic_cast<const Mage*>(src)) {
            clone = new Mage(m->getId(), m->getName(), m->getMaxHp(), m->getType(), m->getMaxMana(), m->getSpellDamage(), m->getManaCost(), m->getFallbackDamage());
        }
        else if (const Archer* a = dynamic_cast<const Archer*>(src)) {
            clone = new Archer(a->getId(), a->getName(), a->getMaxHp(), a->getType(), static_cast<int>(a->getNormalDamage()), static_cast<int>(a->getCriticalDamage()));
        }
        else if (const Healer* h = dynamic_cast<const Healer*>(src)) {
            clone = new Healer(h->getId(), h->getName(), h->getMaxHp(), h->getHealingPower());
        }
        if (clone == nullptr) {
            std::cout << "[WARN] Loai nhan vat ID=" << cid << " khong ho tro cloning, bo qua.\n";
            --m_sizeB; --i; continue;
        }
        m_slotsB[i] = clone;
        m_ownedB[i] = true;
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

void Battle::resetCombatants() {
    for (int i = 0; i < m_sizeA; ++i) {
        if (m_slotsA[i]) m_slotsA[i]->resetForBattle();
    }
    for (int i = 0; i < m_sizeB; ++i) {
        if (m_slotsB[i]) m_slotsB[i]->resetForBattle();
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

Character* Battle::getSlot(int side, int index) {
    return (side == 0) ? m_slotsA[index] : m_slotsB[index];
}
const Character* Battle::getSlot(int side, int index) const {
    return (side == 0) ? m_slotsA[index] : m_slotsB[index];
}

bool Battle::hasAlive(int side) const {
    int size = (side == 0) ? m_sizeA : m_sizeB;
    for (int i = 0; i < size; ++i)
        if (getSlot(side, i)->isAlive()) return true;
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
    // free cloned slots
    for (int i = 0; i < m_sizeA; ++i) {
        if (m_slotsA[i] && m_ownedA[i]) delete m_slotsA[i];
        m_slotsA[i] = nullptr; m_ownedA[i] = false;
    }
    for (int i = 0; i < m_sizeB; ++i) {
        if (m_slotsB[i] && m_ownedB[i]) delete m_slotsB[i];
        m_slotsB[i] = nullptr; m_ownedB[i] = false;
    }
    m_sizeA       = 0;
    m_sizeB       = 0;
    m_turnNumber  = 0;
    m_currentSide = 0;
    m_cursorA     = 0;
    m_cursorB     = 0;
    m_isSetup     = false;
}
