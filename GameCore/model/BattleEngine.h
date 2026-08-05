#pragma once
#ifndef BATTLEENGINE_H
#define BATTLEENGINE_H


#include "Battle.h"
//#include "CharacterRoster.h"
//#include "Team.h"

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

    // Trả con trỏ nhân vật đang đến lượt (nullptr nếu không IN_PROGRESS)
    //const Character* getCurrentActor() const;

    const std::string* getWinnerName() const;

    // In toàn bộ trạng thái trận: lượt, HP/Mana từng nhân vật, trạng thái (FR-06)
    //void printStatus(const CharacterRoster& roster) const;

private:
    Battle           m_battle;    
    //const CharacterRoster* m_roster; // con trỏ quan sát, không sở hữu

    // Tìm slot ứng với characterId trong battle (bên nào cũng được)
    bool findSlot(int characterId, int& outSide, int& outIndex) const;

    // Tìm slot của actor đang đến lượt (không advance)
    bool getCurrentActorSlot(int& outSide, int& outIndex) const;

    void advanceToNextActor();
    bool isValidTarget(int targetId, int actorSide) const;
};

#endif // BATTLEENGINE_H