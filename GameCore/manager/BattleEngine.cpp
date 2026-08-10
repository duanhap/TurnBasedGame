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
    m_battle.resetCombatants();
    // Reset cursor về 0 cho cả hai bên
    m_battle.setCursorA(0);
    m_battle.setCursorB(0);
    m_battle.setCurrentSide(0);  // Team A đi trước
    m_battle.setState(BattleState::IN_PROGRESS);
    m_battle.incrementTurn();   // lượt 1

    // Nếu NV[0] của A đã chết (hiếm nhưng phòng thủ) thì advance
    if (!m_battle.getSlot(0, m_battle.getCursorA())->isAlive()) {
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
    Character* actorChar = m_battle.getSlot(actorSide, actorIndex);

    if (actorChar == nullptr || !actorChar->isAlive()) {
        advanceToNextActor();
        return false;
    }

    if (!isValidTarget(targetCharacterId, actorSide, actorChar))
        return false;

    int targetSide = -1, targetIndex = -1;
    if (!findSlot(targetCharacterId, targetSide, targetIndex))
        return false;
    Character* targetChar = m_battle.getSlot(targetSide, targetIndex);
    if (targetChar == nullptr) return false;

    int targetHpBefore = targetChar->getCurrentHp();

    // *** RUNTIME POLYMORPHISM — không có dynamic_cast, không có if/switch ***
    // Warrior hoặc Mage tự quyết định hành vi trên battle state của chính nó
    actorChar->performAction(*targetChar);

    int targetHpAfter = targetChar->getCurrentHp();
    int hpDiff = (int)targetHpBefore - (int)targetHpAfter;
    std::string actionType = "DAMAGE";
    int val = hpDiff;
    if (hpDiff < 0) {
        actionType = "HEAL";
        val = -hpDiff;
    }

    BattleLogEntry entry;
    entry.turnNumber = m_battle.getTurnNumber();
    entry.actorId = actorChar->getId();
    entry.actorName = actorChar->getName();
    entry.targetId = targetChar->getId();
    entry.targetName = targetChar->getName();
    entry.actionType = actionType;
    entry.value = val;
    entry.targetHpAfter = targetHpAfter;
    entry.targetMaxHp = targetChar->getMaxHp();
    entry.targetAliveAfter = targetChar->isAlive();

    m_battleLog.push_back(entry);

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
    int side   = m_battle.getCurrentSide();
    int cursor = (side == 0) ? m_battle.getCursorA() : m_battle.getCursorB();
    const Character* ch = m_battle.getSlot(side, cursor);
    if (ch == nullptr || !ch->isAlive()) return nullptr;
    return ch;
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
            const Character* ch = m_battle.getSlot(side, i);
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
            std::cout << " | " << ch->getCurrentHp() << "/" << ch->getMaxHp();
            if (ch->getMaxMana() > 0)
                std::cout << " | " << ch->getCurrentMana() << "/" << ch->getMaxMana();
            else
                std::cout << " | -      ";
            std::cout << " | " << (ch->isAlive() ? "Alive" : "KO") << "\n";
        }
    }
    std::cout << "======================================\n";
    if (!m_battleLog.empty()) {
        std::cout << "\n--- BATTLE LOG ---\n";
        for (const auto& entry : m_battleLog) {
            std::cout << "[Luot " << entry.turnNumber << "] " 
                      << entry.actorName << " (ID: " << entry.actorId << ") ";
            if (entry.actionType == "DAMAGE") {
                std::cout << "tan cong " << entry.targetName << " (ID: " << entry.targetId << ") ";
                std::cout << "gay " << entry.value << " sat thuong. ";
            } else if (entry.actionType == "HEAL") {
                std::cout << "hoi HP cho " << entry.targetName << " (ID: " << entry.targetId << ") ";
                std::cout << "them " << entry.value << " HP. ";
            }
            std::cout << "-> Trang thai sau hanh dong: " << entry.targetName
                      << " (HP: " << entry.targetHpAfter << "/" << entry.targetMaxHp
                      << ", Status: " << (entry.targetAliveAfter ? "Alive" : "KO") << ")\n";
        }
        std::cout << "--------------------------------------\n";
    }
}

bool BattleEngine::findSlot(int characterId, int& outSide, int& outIndex) const {
    for (int side = 0; side < 2; ++side) {
        int size = (side == 0) ? m_battle.getSizeA() : m_battle.getSizeB();
        for (int i = 0; i < size; ++i) {
            const Character* ch = m_battle.getSlot(side, i);
            if (ch != nullptr && ch->getId() == characterId) {
                outSide = side;
                outIndex = i;
                return true;
            }
        }
    }
    return false;
}

bool BattleEngine::isValidTarget(int targetId, int actorSide, const Character* actor) const {
    int tSide = -1, tIndex = -1;
    if (!findSlot(targetId, tSide, tIndex)) return false;
    const Character* ch = m_battle.getSlot(tSide, tIndex);
    if (ch == nullptr || !ch->isAlive()) return false;
    // Default: cannot target same side (cannot attack ally)
        // Healer can target allies (to heal) but should not heal opponents
        if (actor == nullptr) {
            // conservative default
            return tSide != actorSide;
        }
    std::string actorType = actor->getType();
    if (actorType == "HEALER" || actorType == "Healer") {
        // Healer may target allies (same side) but not opponents
         return tSide == actorSide;
    }
    // Other classes: must target opponent*
    return tSide != actorSide;
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
        const Character* ch = m_battle.getSlot(nextSide, cursor);
        if (ch == nullptr || !ch->isAlive()) {
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
        const Character* ch = m_battle.getSlot(side, next);
        if (ch != nullptr && ch->isAlive()) {
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
    clearBattleLog();
}

const Battle& BattleEngine::getBattle() const {
    return m_battle;
}

const std::vector<BattleLogEntry>& BattleEngine::getBattleLog() const {
    return m_battleLog;
}

void BattleEngine::clearBattleLog() {
    m_battleLog.clear();
}