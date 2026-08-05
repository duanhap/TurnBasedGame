#include "BattleEngine.h"
#include <iostream>
#include <string>

// ============================================================
//  Constructor
// ============================================================
//BattleEngine::BattleEngine()
//    : m_roster(nullptr)
//{
//}

// ============================================================
//  selectTeams 
// ============================================================
//bool BattleEngine::selectTeams(const Team* teamA,
//    const Team* teamB,
//    const CharacterRoster& roster) {
//    // Battle::setup() kiểm tra: nullptr, cùng ID, Team rỗng
//    bool ok = m_battle.setup(teamA, teamB, roster);
//    if (ok) {
//        m_roster = &roster;   // lưu tham chiếu để dùng sau
//    }
//    return ok;
//}

// ============================================================
//  startBattle  
// ============================================================
//bool BattleEngine::startBattle() {
//    // Chưa setup hoặc đang/đã diễn ra → từ chối
//    if (!m_battle.isSetup())
//        return false;
//    if (m_battle.getState() == BattleState::IN_PROGRESS ||
//        m_battle.getState() == BattleState::FINISHED)
//        return false;
//
//    // Reset HP/Mana về max 
//    m_battle.resetCombatants(*m_roster);7)
//
//    // Đặt lượt đầu tiên: Team A, nhân vật đầu tiên còn sống
//    m_battle.setCurrentSide(0);
//    m_battle.setCurrentIndex(0);
//    m_battle.setState(BattleState::IN_PROGRESS);
//    m_battle.incrementTurn();   // lượt 1
//
//    // Bỏ qua slot đầu nếu đã chết (trường hợp hiếm, nhưng phòng thủ)
//    // Advance tới đúng actor đầu tiên còn sống
//    int side = 0;
//    int index = 0;
//    if (!m_battle.getSlot(side, index).isAlive()) {
//        advanceToNextActor();
//    }
//
//    return true;
//}

// ============================================================
//  performCurrentAction  
// ============================================================
bool BattleEngine::performCurrentAction(int targetCharacterId) {
    // TC-15: battle chưa IN_PROGRESS → từ chối
    if (m_battle.getState() != BattleState::IN_PROGRESS)
        return false;

    // Xác định actor hiện tại
    int actorSide = m_battle.getCurrentSide();
    int actorIndex = m_battle.getCurrentIndex();
    CombatantSlot& actorSlot = m_battle.getSlot(actorSide, actorIndex);

    // TC-13: actor đã bị hạ (không nên xảy ra nếu advanceToNextActor() đúng, nhưng guard)
    if (!actorSlot.isAlive()) {
        advanceToNextActor();
        return false;
    }

    // TC-12: target phải còn sống và thuộc phe đối lập
    if (!isValidTarget(targetCharacterId, actorSide))
        return false;

    // Lấy Character pointer từ Roster → runtime polymorphism (FR-04)
    //Character* actorChar = m_roster->findById(actorSlot.characterId);
    //if (actorChar == nullptr) return false;

    // Tìm slot của target để cập nhật HP sau hành động
    int targetSide = -1, targetIndex = -1;
    if (!findSlot(targetCharacterId, targetSide, targetIndex))
        return false;
    CombatantSlot& targetSlot = m_battle.getSlot(targetSide, targetIndex);

    // Gắn currentHp / currentMana vào actorChar để performAction() đọc đúng
    // (Character chứa currentHp/currentMana là dữ liệu phiên — do Battle quản lý)
    //actorChar->setCurrentHp(actorSlot.currentHp);
    //actorChar->setCurrentMana(actorSlot.currentMana);

    // Lấy Character của target để performAction() tác động lên
    //Character* targetChar = m_roster->findById(targetCharacterId);
    //if (targetChar == nullptr) return false;
    //targetChar->setCurrentHp(targetSlot.currentHp);
    //targetChar->setCurrentMana(targetSlot.currentMana);

    // *** GỌI POLYMORPHIC — Warrior hoặc Mage tự quyết định implementation ***
    //actorChar->performAction(*targetChar);  // FR-04: runtime polymorphism

    // Đọc lại trạng thái sau hành động và ghi ngược vào slot
    //actorSlot.currentMana = actorChar->getCurrentMana();
    //targetSlot.currentHp = targetChar->getCurrentHp();

    // Clamp HP không âm
    if (targetSlot.currentHp < 0) targetSlot.currentHp = 0;

    // Kiểm tra kết thúc trận: phe đối lập hết nhân vật sống (TC-14)
    if (!m_battle.hasAlive(targetSide)) {
        m_battle.setState(BattleState::FINISHED);
        return true;  // hành động đã thực hiện thành công trước khi kết thúc
    }

    // Advance sang nhân vật sống tiếp theo (TC-13)
    advanceToNextActor();
    m_battle.incrementTurn();

    return true;
}

// ============================================================
//  Query methods
// ============================================================
bool BattleEngine::isInProgress() const {
    return m_battle.getState() == BattleState::IN_PROGRESS;
}

bool BattleEngine::isFinished() const {
    return m_battle.getState() == BattleState::FINISHED;
}

//const Character* BattleEngine::getCurrentActor() const {
//    if (m_battle.getState() != BattleState::IN_PROGRESS) return nullptr;
//    if (m_roster == nullptr)                              return nullptr;
//
//    int side = m_battle.getCurrentSide();
//    int index = m_battle.getCurrentIndex();
//    const CombatantSlot& slot = m_battle.getSlot(side, index);
//    if (!slot.isAlive()) return nullptr;
//
//    return m_roster->findById(slot.characterId);
//}

const std::string* BattleEngine::getWinnerName() const {
    if (m_battle.getState() != BattleState::FINISHED) return nullptr;
    // Bên nào còn sống là bên thắng
    if (m_battle.hasAlive(0)) return &m_battle.getTeamAName();
    return &m_battle.getTeamBName();
}

// ============================================================
//  printStatus  (FR-06, TC-07, TC-09...)
// ============================================================
//void BattleEngine::printStatus(const CharacterRoster& roster) const {
//    std::cout << "\n======================================\n";
//
//    // Trạng thái
//    std::cout << "Trang thai: ";
//    switch (m_battle.getState()) {
//    case BattleState::READY:       std::cout << "READY\n";       break;
//    case BattleState::IN_PROGRESS: std::cout << "IN_PROGRESS\n"; break;
//    case BattleState::FINISHED:    std::cout << "FINISHED\n";    break;
//    }
//
//    if (!m_battle.isSetup()) {
//        std::cout << "(Chua chon Team)\n";
//        std::cout << "======================================\n";
//        return;
//    }
//
//    std::cout << "Luot: " << m_battle.getTurnNumber() << "\n";
//
//    // In từng bên
//    for (int side = 0; side < 2; ++side) {
//        const std::string& teamName = (side == 0)
//            ? m_battle.getTeamAName()
//            : m_battle.getTeamBName();
//        int size = (side == 0) ? m_battle.getSizeA() : m_battle.getSizeB();
//
//        std::cout << "\n[" << teamName << "]\n";
//        std::cout << "  ID   | Ten             | HP       | Mana   | Trang thai\n";
//        std::cout << "  -----|-----------------|----------|--------|----------\n";
//
//        for (int i = 0; i < size; ++i) {
//            const CombatantSlot& slot = m_battle.getSlot(side, i);
//            const Character* ch = roster.findById(slot.characterId);
//            if (ch == nullptr) continue;
//
//            // Đánh dấu actor đang đến lượt
//            bool isCurrent = (m_battle.getState() == BattleState::IN_PROGRESS &&
//                side == m_battle.getCurrentSide() &&
//                i == m_battle.getCurrentIndex());
//
//            std::cout << (isCurrent ? "  >>>" : "     ");
//            std::cout << " " << ch->getId();
//            // Padding tên
//            std::string name = ch->getName();
//            if (name.size() > 15) name = name.substr(0, 15);
//            std::cout << " | " << name;
//            for (int p = (int)name.size(); p < 15; ++p) std::cout << ' ';
//
//            std::cout << " | " << slot.currentHp << "/" << ch->getMaxHp();
//
//            // Mana chỉ in cho Mage (Warrior mana = 0)
//            if (ch->getMaxMana() > 0) {
//                std::cout << "  | " << slot.currentMana << "/" << ch->getMaxMana();
//            }
//            else {
//                std::cout << "  | -      ";
//            }
//
//            std::cout << " | " << (slot.isAlive() ? "Alive" : "KO") << "\n";
//        }
//    }
//    std::cout << "======================================\n";
//}

// ============================================================
//  Helpers
// ============================================================

// Tìm (side, index) của một characterId trong battle
bool BattleEngine::findSlot(int characterId, int& outSide, int& outIndex) const {
    for (int side = 0; side < 2; ++side) {
        int size = (side == 0) ? m_battle.getSizeA() : m_battle.getSizeB();
        for (int i = 0; i < size; ++i) {
            if (m_battle.getSlot(side, i).characterId == characterId) {
                outSide = side;
                outIndex = i;
                return true;
            }
        }
    }
    return false;
}

// Target hợp lệ: thuộc phe đối lập VÀ còn sống (TC-12)
bool BattleEngine::isValidTarget(int targetId, int actorSide) const {
    int tSide = -1, tIndex = -1;
    if (!findSlot(targetId, tSide, tIndex)) return false;

    // Không được cùng phe
    if (tSide == actorSide) return false;

    // Phải còn sống (TC-12)
    return m_battle.getSlot(tSide, tIndex).isAlive();
}

// Advance sang nhân vật sống tiếp theo theo thứ tự luân phiên A/B (TC-13)
void BattleEngine::advanceToNextActor() {
    int startSide = m_battle.getCurrentSide();
    int startIndex = m_battle.getCurrentIndex();

    // Đổi bên trước (thứ tự luân phiên A↔B)
    int nextSide = 1 - startSide;

    // Thử lần lượt: bên kia → bên này, mỗi bên duyệt từ đầu theo thứ tự đã thêm
    for (int attempt = 0; attempt < 2; ++attempt) {
        int size = (nextSide == 0) ? m_battle.getSizeA() : m_battle.getSizeB();
        for (int i = 0; i < size; ++i) {
            if (m_battle.getSlot(nextSide, i).isAlive()) {
                m_battle.setCurrentSide(nextSide);
                m_battle.setCurrentIndex(i);
                return;
            }
        }
        // Bên này không có ai sống, thử bên kia
        nextSide = 1 - nextSide;
    }

    // Không tìm thấy ai (không nên xảy ra — battle sẽ FINISHED trước)
}
