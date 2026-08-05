#include "../pch.h"
#include "BattleEngine.h"
#include <iostream>
#include <string>

 BattleEngine::BattleEngine()
    : m_roster(nullptr)
 {}

bool BattleEngine::selectTeams(const Team* teamA,
    const Team* teamB,
    const CharacterRoster& roster) {
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
    m_battle.setCurrentSide(0);
    m_battle.setCurrentIndex(0);
    m_battle.setState(BattleState::IN_PROGRESS);
    m_battle.incrementTurn();
    int side = 0, index = 0;
    if (!m_battle.getSlot(side, index).isAlive()) {
        advanceToNextActor();
    }
    return true;
}

bool BattleEngine::performCurrentAction(int targetCharacterId) {
    if (m_battle.getState() != BattleState::IN_PROGRESS)
        return false;

    int actorSide  = m_battle.getCurrentSide();
    int actorIndex = m_battle.getCurrentIndex();
    CombatantSlot& actorSlot = m_battle.getSlot(actorSide, actorIndex);

    if (!actorSlot.isAlive()) {
        advanceToNextActor();
        return false;
    }

    if (!isValidTarget(targetCharacterId, actorSide))
        return false;

    //Character* actorChar = m_roster->findById(actorSlot.characterId);
    //if (actorChar == nullptr) return false;

    int targetSide = -1, targetIndex = -1;
    if (!findSlot(targetCharacterId, targetSide, targetIndex))
        return false;
    CombatantSlot& targetSlot = m_battle.getSlot(targetSide, targetIndex);

    //actorChar->setCurrentHp(actorSlot.currentHp);
    //actorChar->setCurrentMana(actorSlot.currentMana);

    //Character* targetChar = m_roster->findById(targetCharacterId);
    //if (targetChar == nullptr) return false;
    //targetChar->setCurrentHp(targetSlot.currentHp);
    //targetChar->setCurrentMana(targetSlot.currentMana);

    // *** RUNTIME POLYMORPHISM ***
    //actorChar->performAction(*targetChar);

    //actorSlot.currentMana = actorChar->getCurrentMana();
    //targetSlot.currentHp  = targetChar->getCurrentHp();

    if (targetSlot.currentHp < 0) targetSlot.currentHp = 0;

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

//const Character* BattleEngine::getCurrentActor() const {
//    if (m_battle.getState() != BattleState::IN_PROGRESS) return nullptr;
//    if (m_roster == nullptr) return nullptr;
//    int side  = m_battle.getCurrentSide();
//    int index = m_battle.getCurrentIndex();
//    const CombatantSlot& slot = m_battle.getSlot(side, index);
//    if (!slot.isAlive()) return nullptr;
//    return m_roster->findById(slot.characterId);
//}

const std::string* BattleEngine::getWinnerName() const {
    if (m_battle.getState() != BattleState::FINISHED) return nullptr;
    if (m_battle.hasAlive(0)) return &m_battle.getTeamAName();
    return &m_battle.getTeamBName();
}

//void BattleEngine::printStatus(const CharacterRoster& roster) const { ... }

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
    int nextSide = 1 - m_battle.getCurrentSide();
    for (int attempt = 0; attempt < 2; ++attempt) {
        int size = (nextSide == 0) ? m_battle.getSizeA() : m_battle.getSizeB();
        for (int i = 0; i < size; ++i) {
            if (m_battle.getSlot(nextSide, i).isAlive()) {
                m_battle.setCurrentSide(nextSide);
                m_battle.setCurrentIndex(i);
                return;
            }
        }
        nextSide = 1 - nextSide;
    }
}
