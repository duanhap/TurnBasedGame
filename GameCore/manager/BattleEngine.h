#pragma once
#ifndef BATTLEENGINE_H
#define BATTLEENGINE_H

#include "../model/Battle.h"
//#include "CharacterRoster.h"
//#include "../model/Team.h"

class BattleEngine {
public:
    BattleEngine();

    //bool selectTeams(const Team* teamA,
    //    const Team* teamB,
    //    const CharacterRoster& roster);

    bool startBattle();

    bool performCurrentAction(int targetCharacterId);

    bool isInProgress() const;
    bool isFinished()   const;

    //const Character* getCurrentActor() const;

    const std::string* getWinnerName() const;

    //void printStatus(const CharacterRoster& roster) const;

private:
    Battle           m_battle;
    //const CharacterRoster* m_roster;

    bool findSlot(int characterId, int& outSide, int& outIndex) const;
    bool getCurrentActorSlot(int& outSide, int& outIndex) const;
    void advanceToNextActor();
    bool isValidTarget(int targetId, int actorSide) const;
};

#endif // BATTLEENGINE_H
