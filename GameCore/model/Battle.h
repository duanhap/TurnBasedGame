#ifndef BATTLE_H
#define BATTLE_H

#include "CombatantSlot.h"   // tách riêng để tránh circular dependency
#include "Character.h"
#include "Team.h"
#include "../manager/CharacterRoster.h"
#include <string>

enum class BattleState {
    READY,
    IN_PROGRESS,
    FINISHED
};

static const int MAX_TEAM_SIZE = 5;

class Battle {
public:
    Battle();

    // Gán hai Team và khởi tạo mảng slot; trả false nếu không hợp lệ
    bool setup(const Team* teamA, const Team* teamB, const CharacterRoster& roster);

    // Reset currentHp / currentMana về maxHp / maxMana (gọi khi Start)
    void resetCombatants(const CharacterRoster& roster);

    
    BattleState getState() const;
    void        setState(BattleState s);

    
    const std::string& getTeamAName() const;
    const std::string& getTeamBName() const;
    int                getTeamAId()   const;
    int                getTeamBId()   const;

    
    int getSizeA() const;
    int getSizeB() const;

    // Truy cập slot theo bên (side=0 → A, side=1 → B) và chỉ số
    CombatantSlot& getSlot(int side, int index);
    const CombatantSlot& getSlot(int side, int index) const;

    // Kiểm tra còn nhân vật sống trong một bên
    bool hasAlive(int side) const;

    // Lượt hiện tại (tăng sau mỗi hành động hợp lệ)
    int  getTurnNumber() const;
    void incrementTurn();

    int  getCurrentSide()  const;
    int  getCurrentIndex() const;
    void setCurrentSide(int side);

    // Cursor riêng cho từng bên — mỗi bên tự advance độc lập
    int  getCursorA() const;
    int  getCursorB() const;
    void setCursorA(int idx);
    void setCursorB(int idx);

    // getCurrentIndex() trả cursor của bên đang đến lượt
    //int  getCurrentIndex() const;

    bool isSetup() const;

private:
    BattleState   m_state;

    // Thông tin hai Team 
    int         m_teamAId;
    int         m_teamBId;
    std::string m_teamAName;
    std::string m_teamBName;


    CombatantSlot m_slotsA[MAX_TEAM_SIZE];
    CombatantSlot m_slotsB[MAX_TEAM_SIZE];
    int           m_sizeA;
    int           m_sizeB;

    int  m_turnNumber;
    int  m_currentSide;   // 0 = A, 1 = B đang đến lượt
    int  m_cursorA;       // cursor riêng bên A (index NV sẽ hành động tiếp theo)
    int  m_cursorB;       // cursor riêng bên B
    bool m_isSetup;
};

#endif // BATTLE_H