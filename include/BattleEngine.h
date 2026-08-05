#ifndef BATTLEENGINE_H
#define BATTLEENGINE_H

// BattleEngine.h — Điều khiển luồng trận đấu


#include "Battle.h"
//#include "CharacterRoster.h"
//#include "Team.h"

class BattleEngine {
public:
    BattleEngine();


    // Chọn hai Team; trả false nếu cùng một Team, rỗng, hoặc không hợp lệ (TC-06)
    //bool selectTeams(const Team* teamA,
    //    const Team* teamB,
    //    const CharacterRoster& roster);

    // Bắt đầu trận: reset HP/Mana, trạng thái READY → IN_PROGRESS (TC-07)
    bool startBattle();

    // Thực hiện hành động của actor hiện tại lên target (chỉ định bằng ID)
    bool performCurrentAction(int targetCharacterId);

    bool isInProgress() const;
    bool isFinished()   const;

    // Trả con trỏ nhân vật đang đến lượt (nullptr nếu không IN_PROGRESS)
    //const Character* getCurrentActor() const;

    // Trả Team thắng (nullptr nếu chưa FINISHED)
    const std::string* getWinnerName() const;

    // In toàn bộ trạng thái trận: lượt, HP/Mana từng nhân vật, trạng thái (FR-06)
    //void printStatus(const CharacterRoster& roster) const;

private:
    Battle           m_battle;      // data model — Battle sở hữu slot
    //const CharacterRoster* m_roster; // con trỏ quan sát, không sở hữu

    // Tìm slot ứng với characterId trong battle (bên nào cũng được)
    bool findSlot(int characterId, int& outSide, int& outIndex) const;

    // Tìm slot của actor đang đến lượt (không advance)
    bool getCurrentActorSlot(int& outSide, int& outIndex) const;

    // Advance sang nhân vật sống tiếp theo (sau khi đã thực hiện lượt)
    // Cập nhật m_battle.currentSide / currentIndex
    void advanceToNextActor();

    // Kiểm tra target còn sống và thuộc phe đối lập
    bool isValidTarget(int targetId, int actorSide) const;
};

#endif // BATTLEENGINE_H
