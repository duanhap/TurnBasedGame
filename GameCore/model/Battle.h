#ifndef BATTLE_H
#define BATTLE_H

//#include "Character.h"
//#include "Team.h"
//#include "CharacterRoster.h"
#include <string>

// ----------------------------------------------------------------
// Enum trạng thái trận đấu  (FR-03)
// ----------------------------------------------------------------
enum class BattleState {
    READY,       // Đã chọn Team, chưa bắt đầu
    IN_PROGRESS, // Đang diễn ra
    FINISHED     // Đã kết thúc
};

// ----------------------------------------------------------------
// Lưu HP / Mana hiện tại của MỘT nhân vật trong phiên chiến đấu.
// Tách khỏi Character để không làm bẩn dữ liệu gốc (roster).
// ----------------------------------------------------------------
struct CombatantSlot {
    int  characterId;   // tham chiếu ngược tới Roster
    int  currentHp;
    int  currentMana;   // = 0 với Warrior (không dùng mana)

    CombatantSlot();
    CombatantSlot(int id, int hp, int mana);

    bool isAlive() const;
};

// ----------------------------------------------------------------
// MAX sizes — số lượng nhân vật trong team
// ----------------------------------------------------------------
static const int MAX_TEAM_SIZE = 5;

// ----------------------------------------------------------------
// Battle — data container cho một phiên đấu
// ----------------------------------------------------------------
class Battle {
public:
    Battle();

    // --- Setup (gọi bởi BattleEngine::selectTeams) ---

    // Gán hai Team và khởi tạo mảng slot; trả false nếu không hợp lệ
    //bool setup(const Team* teamA, const Team* teamB, const CharacterRoster& roster);

    // Reset currentHp / currentMana về maxHp / maxMana (gọi khi Start)
    //void resetCombatants(const CharacterRoster& roster);

    // --- Truy vấn trạng thái ---
    BattleState getState() const;
    void        setState(BattleState s);

    // Tên hai Team để hiển thị
    const std::string& getTeamAName() const;
    const std::string& getTeamBName() const;
    int                getTeamAId()   const;
    int                getTeamBId()   const;

    // Số nhân vật thực sự trong mỗi bên (≤ MAX_TEAM_SIZE)
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

    // Bên đến lượt (0 = Team A, 1 = Team B), và chỉ số trong bên đó
    int  getCurrentSide()  const;
    int  getCurrentIndex() const;
    void setCurrentSide(int side);
    void setCurrentIndex(int idx);

    // Kiểm tra setup đã được gọi chưa
    bool isSetup() const;

private:
    BattleState   m_state;

    // Thông tin hai Team (chỉ tên + ID, không sở hữu Team object)
    int         m_teamAId;
    int         m_teamBId;
    std::string m_teamAName;
    std::string m_teamBName;

    // Mảng slot cố định cho hai bên
    CombatantSlot m_slotsA[MAX_TEAM_SIZE];
    CombatantSlot m_slotsB[MAX_TEAM_SIZE];
    int           m_sizeA;
    int           m_sizeB;

    int  m_turnNumber;    // lượt tổng (bắt đầu từ 1)
    int  m_currentSide;   // 0 = A, 1 = B
    int  m_currentIndex;  // chỉ số nhân vật trong bên đang đến lượt

    bool m_isSetup;
};

#endif // BATTLE_H