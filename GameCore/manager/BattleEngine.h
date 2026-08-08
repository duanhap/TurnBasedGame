#pragma once
#ifndef BATTLEENGINE_H
#define BATTLEENGINE_H

#include "../model/Battle.h"
#include "CharacterRoster.h"
#include "../model/Team.h"

class BattleEngine {
public:
    BattleEngine();

    bool selectTeams(const Team*    teamA,
                     const Team*    teamB,
                     CharacterRoster& roster);

    bool startBattle();

    bool performCurrentAction(int targetCharacterId);

    bool isInProgress() const;
    bool isFinished()   const;

    const Character* getCurrentActor() const;

    const std::string* getWinnerName() const;

    void printStatus(const CharacterRoster& roster) const;

    // Reset trận đấu về trạng thái ban đầu (cho phép chơi trận mới)
    void reset();

    // Getter cho Menu truy cập trạng thái trận đấu để hiển thị
    const Battle& getBattle() const;

private:
    Battle           m_battle;
    CharacterRoster* m_roster; // non-const để gọi findById() mutable

    bool findSlot(int characterId, int& outSide, int& outIndex) const;
    //bool getCurrentActorSlot(int& outSide, int& outIndex) const;
    void advanceToNextActor();
    void advanceCursorForSide(int side); // advance cursor riêng của một bên
    bool isValidTarget(int targetId, int actorSide) const;
};

#endif // BATTLEENGINE_H
