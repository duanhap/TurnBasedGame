#include "../pch.h"
#include "BattleEngine.h"
#include <iostream>
#include <string>

 BattleEngine::BattleEngine()
    : m_roster(nullptr)
 {}

bool BattleEngine::selectTeams(const Team* teamA,
    const Team* teamB,
    CharacterRoster& roster) {
    bool ok = m_battle.setup(teamA, teamB, roster);
    if (ok) {
        m_roster = &roster;
    }
    return ok;
}

bool BattleEngine::startBattle() {
    if (!m_battle.isSetup()) return false;
    if (m_battle.getState() == BattleState::IN_PROGRESS ||
        m_battle.getState() == BattleState::FINISHED)
        return false;
    m_battle.resetCombatants(*m_roster);
    // Reset cursor về 0 cho cả hai bên
    m_battle.setCursorA(0);
    m_battle.setCursorB(0);
    m_battle.setCurrentSide(0);  // Team A đi trước
    m_battle.setState(BattleState::IN_PROGRESS);
    m_battle.incrementTurn();   // lượt 1

    // Nếu NV[0] của A đã chết (hiếm nhưng phòng thủ) thì advance
    if (!m_battle.getSlot(0, m_battle.getCursorA()).isAlive()) {
        advanceCursorForSide(0);
    }
    return true;
}

bool BattleEngine::performCurrentAction(int targetCharacterId) {
    if (m_battle.getState() != BattleState::IN_PROGRESS)
        return false;

    int actorSide  = m_battle.getCurrentSide();
    // Dùng cursor riêng của bên đang đến lượt
    int actorIndex = (actorSide == 0) ? m_battle.getCursorA() : m_battle.getCursorB();
    CombatantSlot& actorSlot = m_battle.getSlot(actorSide, actorIndex);

    if (!actorSlot.isAlive()) {
        advanceToNextActor();
        return false;
    }

    if (!isValidTarget(targetCharacterId, actorSide))
        return false;

    Character* actorChar = m_roster->findById(actorSlot.characterId);
    if (actorChar == nullptr) return false;

    int targetSide = -1, targetIndex = -1;
    if (!findSlot(targetCharacterId, targetSide, targetIndex))
        return false;
    CombatantSlot& targetSlot = m_battle.getSlot(targetSide, targetIndex);

    // *** RUNTIME POLYMORPHISM — không có dynamic_cast, không có if/switch ***
    // Warrior hoặc Mage tự quyết định hành vi dựa trên slot phiên đấu
    actorChar->performActionInBattle(actorSlot, targetSlot);

    if (!m_battle.hasAlive(targetSide)) {
        m_battle.setState(BattleState::FINISHED);
        return true;
    }

    advanceToNextActor();
    m_battle.incrementTurn();
    return true;
}

bool BattleEngine::isInProgress() const {
    return m_battle.getState() == BattleState::IN_PROGRESS;
}

bool BattleEngine::isFinished() const {
    return m_battle.getState() == BattleState::FINISHED;
}

const Character* BattleEngine::getCurrentActor() const {
    if (m_battle.getState() != BattleState::IN_PROGRESS) return nullptr;
    if (m_roster == nullptr) return nullptr;
    int side   = m_battle.getCurrentSide();
    int cursor = (side == 0) ? m_battle.getCursorA() : m_battle.getCursorB();
    const CombatantSlot& slot = m_battle.getSlot(side, cursor);
    if (!slot.isAlive()) return nullptr;
    return m_roster->findById(slot.characterId);
}

const std::string* BattleEngine::getWinnerName() const {
    if (m_battle.getState() != BattleState::FINISHED) return nullptr;
    if (m_battle.hasAlive(0)) return &m_battle.getTeamAName();
    return &m_battle.getTeamBName();
}

//void BattleEngine::printStatus(const CharacterRoster& roster) const { ... }
void BattleEngine::printStatus(const CharacterRoster& roster) const {
    std::cout << "\n======================================\n";
    std::cout << "Trang thai: ";
    switch (m_battle.getState()) {
        case BattleState::READY:       std::cout << "READY\n";       break;
        case BattleState::IN_PROGRESS: std::cout << "IN_PROGRESS\n"; break;
        case BattleState::FINISHED:    std::cout << "FINISHED\n";    break;
    }
    if (!m_battle.isSetup()) {
        std::cout << "(Chua chon Team)\n======================================\n";
        return;
    }
    std::cout << "Luot: " << m_battle.getTurnNumber() << "\n";
    for (int side = 0; side < 2; ++side) {
        const std::string& teamName = (side == 0)
            ? m_battle.getTeamAName() : m_battle.getTeamBName();
        int size = (side == 0) ? m_battle.getSizeA() : m_battle.getSizeB();
        std::cout << "\n[" << teamName << "]\n";
        std::cout << "  ID  | Ten             | HP      | Mana   | Status\n";
        std::cout << "------|-----------------|---------|--------|-------\n";
        for (int i = 0; i < size; ++i) {
            const CombatantSlot& slot = m_battle.getSlot(side, i);
            const Character* ch = roster.findById(slot.characterId);
            if (ch == nullptr) continue;
            int currentCursor = (side == 0) ? m_battle.getCursorA() : m_battle.getCursorB();
            bool isCurrent = (m_battle.getState() == BattleState::IN_PROGRESS
                              && side == m_battle.getCurrentSide()
                              && i    == currentCursor);
            std::cout << (isCurrent ? ">>>" : "   ");
            std::cout << " " << ch->getId() << " | ";
            std::string nm = ch->getName();
            if (nm.size() > 15) nm = nm.substr(0, 15);
            std::cout << nm;
            for (int p = (int)nm.size(); p < 15; ++p) std::cout << ' ';
            std::cout << " | " << slot.currentHp << "/" << ch->getMaxHp();
            if (ch->getMaxMana() > 0)
                std::cout << " | " << slot.currentMana << "/" << ch->getMaxMana();
            else
                std::cout << " | -      ";
            std::cout << " | " << (slot.isAlive() ? "Alive" : "KO") << "\n";
        }
    }
    std::cout << "======================================\n";
}

bool BattleEngine::findSlot(int characterId, int& outSide, int& outIndex) const {
    for (int side = 0; side < 2; ++side) {
        int size = (side == 0) ? m_battle.getSizeA() : m_battle.getSizeB();
        for (int i = 0; i < size; ++i) {
            if (m_battle.getSlot(side, i).characterId == characterId) {
                outSide  = side;
                outIndex = i;
                return true;
            }
        }
    }
    return false;
}

bool BattleEngine::isValidTarget(int targetId, int actorSide) const {
    int tSide = -1, tIndex = -1;
    if (!findSlot(targetId, tSide, tIndex)) return false;
    if (tSide == actorSide) return false;
    return m_battle.getSlot(tSide, tIndex).isAlive();
}

void BattleEngine::advanceToNextActor() {
    // Sau khi bên doneSide vừa hành động:
    // - Advance cursor của doneSide sang NV kế (chuẩn bị lượt sau)
    // - Chuyển sang nextSide, cursor nextSide giữ nguyên (đang trỏ đúng NV của họ)
    int doneSide = m_battle.getCurrentSide();
    int nextSide = 1 - doneSide;

    // Advance cursor bên vừa đánh
    advanceCursorForSide(doneSide);

    // Chuyển sang bên kia
    if (m_battle.hasAlive(nextSide)) {
        m_battle.setCurrentSide(nextSide);
        // Nếu cursor bên kia đang trỏ vào NV chết (ví dụ vừa bị kill),
        // thì advance cursor của họ luôn
        int cursor = (nextSide == 0) ? m_battle.getCursorA() : m_battle.getCursorB();
        if (!m_battle.getSlot(nextSide, cursor).isAlive()) {
            advanceCursorForSide(nextSide);
        }
    }
    // Nếu nextSide không còn ai sống: không đổi — performCurrentAction đã set FINISHED rồi
}

// Advance cursor của một bên sang NV còn sống tiếp theo (vòng tròn)
void BattleEngine::advanceCursorForSide(int side) {
    int size   = (side == 0) ? m_battle.getSizeA() : m_battle.getSizeB();
    int cursor = (side == 0) ? m_battle.getCursorA() : m_battle.getCursorB();

    // Tìm NV còn sống tiếp theo theo thứ tự vòng tròn
    for (int offset = 1; offset <= size; ++offset) {
        int next = (cursor + offset) % size;
        if (m_battle.getSlot(side, next).isAlive()) {
            if (side == 0) m_battle.setCursorA(next);
            else           m_battle.setCursorB(next);
            return;
        }
    }
    // Không còn ai sống — giữ nguyên cursor (battle sẽ FINISHED)
}

void BattleEngine::reset() {
    m_battle.reset();
    m_roster = nullptr;
}

const Battle& BattleEngine::getBattle() const {
    return m_battle;
}
