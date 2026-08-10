#include "Menu.h"
#include "DataFileManager.h"
#include "Warrior.h"
#include "Mage.h"
#include "Archer.h"
#include "Healer.h"
#include "Battle.h"
#include <iostream>
#include <limits>
#include <string>
#include <memory>
#include <thread>
#include <chrono>

/*
Contributors: Nguyen Dinh Dung, 
Last modified: 2026-08-10
*/
static const int LOADING_DELAY_MS = 500;


Menu::Menu(CharacterRoster& roster,
   TeamManager& teamManager,
   BattleEngine& battleEngine)
   : m_roster(roster)
   , m_teamManager(teamManager)
   , m_battleEngine(battleEngine)
{
}


void Menu::run() {
    bool running = true;
    while (running) {
        showMainMenu();
        int choice = readInt("Chon: ", 1, 4);
        switch (choice) {
        case 1: clearScreenWithLoading(); handleRosterMenu(); break;
        case 2: clearScreenWithLoading(); handleTeamMenu();   break;
        case 3: clearScreenWithLoading(); handleBattleMenu(); break;
        case 4:
            clearScreenWithLoading();
            handleSaveExit();
            running = false;
            break;
        }
    }
}

// ============================================================
//  Menu cấp 1
// ============================================================
void Menu::showMainMenu() const {
    std::cout << "\n========================================\n";
    std::cout << "   TURN-BASED ADVENTURE GAME\n";
    std::cout << "========================================\n";
    std::cout << "  1. Character Roster\n";
    std::cout << "  2. Team\n";
    std::cout << "  3. Battle\n";
    std::cout << "  4. Save & Exit\n";
    std::cout << "----------------------------------------\n";
}

void Menu::handleSaveExit() {
    std::cout << "\n[*] Dang luu du lieu...\n";
    bool okChar = DataFileManager::saveCharacters("data/characters.txt", m_roster);
    bool okTeam = DataFileManager::saveTeams("data/teams.txt", m_teamManager);
    if (okChar && okTeam) {
        std::cout << "[OK] Da luu. Tam biet!\n";
    } else {
        if (!okChar) std::cout << "[LOI] Khong the luu characters.txt. Kiem tra quyen truy cap.\n";
        if (!okTeam) std::cout << "[LOI] Khong the luu teams.txt. Kiem tra quyen truy cap.\n";
    }
}

// ============================================================
//  Sub-menu: Character Roster
// ============================================================
void Menu::showRosterMenu() const {
    std::cout << "\n--- Character Roster ---\n";
    std::cout << "  1. Hien thi tat ca nhan vat\n";
    std::cout << "  2. Them nhan vat moi\n";
    std::cout << "  3. Chinh sua nhan vat\n";
    std::cout << "  4. Xoa nhan vat\n";
    std::cout << "  5. Tim kiem nhan vat\n";
    std::cout << "  0. Quay lai\n";
    std::cout << "------------------------\n";
}

void Menu::handleRosterMenu() {
    bool back = false;
    while (!back) {
        showRosterMenu();
        int choice = readInt("Chon: ", 0, 5);
        switch (choice) {
        case 1: clearScreenWithLoading(); doListCharacters(); break;
        case 2: clearScreenWithLoading(); doAddCharacter();   break;
        case 3: clearScreenWithLoading(); doEditCharacter();  break;
        case 4: clearScreenWithLoading(); doDeleteCharacter(); break;
        case 5: clearScreenWithLoading(); doFindCharacter();  break;
        case 0: clearScreenWithLoading(); back = true;        break;
        }
    }
}

void Menu::doListCharacters() const {
    m_roster.displayAll();
}

void Menu::doAddCharacter() {
    std::cout << "\n-- Them nhan vat moi --\n";
    std::cout << "  1. Warrior\n";
    std::cout << "  2. Mage\n";
    std::cout << "  3. Archer\n";
    std::cout << "  4. Healer\n";
    int typeChoice = readInt("Chon loai: ", 1, 4);

    int id = readInt("Nhap ID (so nguyen duong): ", 1, std::numeric_limits<int>::max());

    std::string name;
    readNonEmptyString("Nhap ten nhan vat: ", name);

    int maxHp = readInt(("Nhap maxHp (" + std::to_string(CHARACTER_MAX_HP_LOWER) + " - " + std::to_string(CHARACTER_MAX_HP_UPPER) + "): ").c_str(),
                              static_cast<int>(CHARACTER_MAX_HP_LOWER), static_cast<int>(CHARACTER_MAX_HP_UPPER));

    if (typeChoice == 1) {
        int attackPower = readInt(("Nhap attackPower (" + std::to_string(WARRIOR_ATTACK_POWER_LOWER) + " - " + std::to_string(WARRIOR_ATTACK_POWER_UPPER) + "): ").c_str(),
                              static_cast<int>(WARRIOR_ATTACK_POWER_LOWER), static_cast<int>(WARRIOR_ATTACK_POWER_UPPER));
        auto warrior = std::make_unique<Warrior>(id, name, (unsigned int)maxHp, "WARRIOR", attackPower);
        int result = m_roster.add(std::move(warrior));
        if (result != -1) {
            std::cout << "[OK] Da them Warrior: " << name << " (ID=" << result << ")\n";
        } else {
            std::cout << "[LOI] Khong the them nhan vat (ID trung hoac chi so khong hop le).\n";
        }
    } else if (typeChoice == 2) {
        int maxMana = readInt(("Nhap maxMana (" + std::to_string(MAGE_MANA_LOWER) + " - " + std::to_string(MAGE_MANA_UPPER) + "): ").c_str(),
                              static_cast<int>(MAGE_MANA_LOWER), static_cast<int>(MAGE_MANA_UPPER));
        int spellDamage = readInt(("Nhap spellDamage (" + std::to_string(MAGE_SPELL_DAMAGE_LOWER) + " - " + std::to_string(MAGE_SPELL_DAMAGE_UPPER) + "): ").c_str(),
                                  static_cast<int>(MAGE_SPELL_DAMAGE_LOWER), static_cast<int>(MAGE_SPELL_DAMAGE_UPPER));
        int manaCost = readInt(("Nhap manaCost (" + std::to_string(MAGE_MANA_COST_LOWER) + " - " + std::to_string(MAGE_MANA_COST_UPPER) + "): ").c_str(),
                               static_cast<int>(MAGE_MANA_COST_LOWER), static_cast<int>(MAGE_MANA_COST_UPPER));
        int fallbackDamage = readInt(("Nhap fallbackDamage (" + std::to_string(MAGE_FALLBACK_DAMAGE_LOWER) + " - " + std::to_string(MAGE_FALLBACK_DAMAGE_UPPER) + "): ").c_str(),
                                     static_cast<int>(MAGE_FALLBACK_DAMAGE_LOWER), static_cast<int>(MAGE_FALLBACK_DAMAGE_UPPER));

        auto mage = std::make_unique<Mage>(id, name, (unsigned int)maxHp, "MAGE",
            (unsigned int)maxMana, (unsigned int)spellDamage,
            (unsigned int)manaCost, (unsigned int)fallbackDamage);
        int result = m_roster.add(std::move(mage));
        if (result != -1) {
            std::cout << "[OK] Da them Mage: " << name << " (ID=" << result << ")\n";
        } else {
            std::cout << "[LOI] Khong the them nhan vat (ID trung hoac chi so khong hop le).\n";
        }
    } else if (typeChoice == 3) {
        // Archer
        int normalDamage = readInt(("Nhap normalDamage (" + std::to_string(ARCHER_NORMAL_DAMAGE_LOWER) + " - " + std::to_string(ARCHER_NORMAL_DAMAGE_UPPER) + "): ").c_str(),
                                   static_cast<int>(ARCHER_NORMAL_DAMAGE_LOWER), static_cast<int>(ARCHER_NORMAL_DAMAGE_UPPER));
        int criticalDamage = readInt(("Nhap criticalDamage (" + std::to_string(ARCHER_CRITICAL_DAMAGE_LOWER) + " - " + std::to_string(ARCHER_CRITICAL_DAMAGE_UPPER) + "): ").c_str(),
                                     static_cast<int>(ARCHER_CRITICAL_DAMAGE_LOWER), static_cast<int>(ARCHER_CRITICAL_DAMAGE_UPPER));

        auto archer = std::make_unique<Archer>(id, name, (unsigned int)maxHp, "ARCHER", normalDamage, criticalDamage);
        int result = m_roster.add(std::move(archer));
        if (result != -1) {
            std::cout << "[OK] Da them Archer: " << name << " (ID=" << result << ")\n";
        } else {
            std::cout << "[LOI] Khong the them nhan vat (ID trung hoac chi so khong hop le).\n";
        }
    } else {
        // Healer
        int healingPower = readInt("Nhap healingPower (> 0): ", 1, std::numeric_limits<int>::max());
        auto healer = std::make_unique<Healer>(id, name, (unsigned int)maxHp, (unsigned int)healingPower);
        int result = m_roster.add(std::move(healer));
        if (result != -1) {
            std::cout << "[OK] Da them Healer: " << name << " (ID=" << result << ")\n";
        } else {
            std::cout << "[LOI] Khong the them nhan vat (ID trung hoac chi so khong hop le).\n";
        }
    }
}

void Menu::doEditCharacter() {
    std::cout << "\n-- Chinh sua nhan vat --\n";
    int id = readInt("Nhap ID nhan vat can sua: ", 1, std::numeric_limits<int>::max());

     //Kiểm tra tồn tại — roster trả về pointer hoặc nullptr
    const Character* ch = m_roster.findById(id);
    if (ch == nullptr) {
        std::cout << "[LOI] Khong tim thay nhan vat co ID = " << id << "\n";
        return;
    }
    ch->display();

    std::string newName;
    readNonEmptyString("Ten moi (giu nguyen nhap lai ten cu): ", newName);

    int newMaxHp = readInt(("maxHp moi (" + std::to_string(CHARACTER_MAX_HP_LOWER) + " - " + std::to_string(CHARACTER_MAX_HP_UPPER) + "): ").c_str(),
                              static_cast<int>(CHARACTER_MAX_HP_LOWER), static_cast<int>(CHARACTER_MAX_HP_UPPER));

    bool ok = false;

    // Detect concrete class using dynamic_cast
    if (dynamic_cast<const Warrior*>(ch)) {
        int attackPower = readInt(("attackPower moi (" + std::to_string(WARRIOR_ATTACK_POWER_LOWER) + " - " + std::to_string(WARRIOR_ATTACK_POWER_UPPER) + "): ").c_str(),
                              static_cast<int>(WARRIOR_ATTACK_POWER_LOWER), static_cast<int>(WARRIOR_ATTACK_POWER_UPPER));
        ok = m_roster.updateWarrior(id, newName, newMaxHp, attackPower);
    }
    else if (dynamic_cast<const Mage*>(ch)) {
        int maxMana = readInt(("maxMana moi (" + std::to_string(MAGE_MANA_LOWER) + " - " + std::to_string(MAGE_MANA_UPPER) + "): ").c_str(),
                              static_cast<int>(MAGE_MANA_LOWER), static_cast<int>(MAGE_MANA_UPPER));
        int spellDamage = readInt(("spellDamage moi (" + std::to_string(MAGE_SPELL_DAMAGE_LOWER) + " - " + std::to_string(MAGE_SPELL_DAMAGE_UPPER) + "): ").c_str(),
                                  static_cast<int>(MAGE_SPELL_DAMAGE_LOWER), static_cast<int>(MAGE_SPELL_DAMAGE_UPPER));
        int manaCost = readInt(("manaCost moi (" + std::to_string(MAGE_MANA_COST_LOWER) + " - " + std::to_string(MAGE_MANA_COST_UPPER) + "): ").c_str(),
                               static_cast<int>(MAGE_MANA_COST_LOWER), static_cast<int>(MAGE_MANA_COST_UPPER));
        int fallbackDamage = readInt(("fallbackDamage moi (" + std::to_string(MAGE_FALLBACK_DAMAGE_LOWER) + " - " + std::to_string(MAGE_FALLBACK_DAMAGE_UPPER) + "): ").c_str(),
                                     static_cast<int>(MAGE_FALLBACK_DAMAGE_LOWER), static_cast<int>(MAGE_FALLBACK_DAMAGE_UPPER));
        ok = m_roster.updateMage(id, newName, newMaxHp, maxMana, spellDamage, manaCost, fallbackDamage);
    }
    else if (dynamic_cast<const Archer*>(ch)) {
        int normalDamage = readInt(("normalDamage moi (" + std::to_string(ARCHER_NORMAL_DAMAGE_LOWER) + " - " + std::to_string(ARCHER_NORMAL_DAMAGE_UPPER) + "): ").c_str(),
                                   static_cast<int>(ARCHER_NORMAL_DAMAGE_LOWER), static_cast<int>(ARCHER_NORMAL_DAMAGE_UPPER));
        int criticalDamage = readInt(("criticalDamage moi (" + std::to_string(ARCHER_CRITICAL_DAMAGE_LOWER) + " - " + std::to_string(ARCHER_CRITICAL_DAMAGE_UPPER) + "): ").c_str(),
                                     static_cast<int>(ARCHER_CRITICAL_DAMAGE_LOWER), static_cast<int>(ARCHER_CRITICAL_DAMAGE_UPPER));
        ok = m_roster.updateArcher(id, newName, newMaxHp, normalDamage, criticalDamage);
    }
    else if (dynamic_cast<const Healer*>(ch)) {
        int maxMahealingPowerna = readInt(("healingPower moi (" + std::to_string(HEALER_HEALING_POWER_LOWER) + " - " + std::to_string(HEALER_HEALING_POWER_UPPER) + "): ").c_str(),
                              static_cast<int>(HEALER_HEALING_POWER_LOWER), static_cast<int>(HEALER_HEALING_POWER_UPPER));
        int healingPower = readInt("healingPower moi (> 0): ", 1, std::numeric_limits<int>::max());
        ok = m_roster.updateHealer(id, newName, newMaxHp, healingPower);
    }
    else {
        std::cout << "[LOI] Loai nhan vat khong ho tro cho chuc nang chinh sua.\n";
        return;
    }

    if (ok) {
        std::cout << "[OK] Da cap nhat nhan vat ID = " << id << "\n";
    }
    else {
        std::cout << "[LOI] Khong the cap nhat (chi so khong hop le).\n";
    }
}

void Menu::doDeleteCharacter() {
    std::cout << "\n-- Xoa nhan vat --\n";
    int id = readInt("Nhap ID nhan vat can xoa: ", 1, std::numeric_limits<int>::max());

    m_teamManager.removeCharacterFromAllTeams(id);

    bool ok = m_roster.remove(id);
    if (ok) {
        std::cout << "[OK] Da xoa nhan vat ID = " << id << " va loai khoi moi Team.\n";
    }
    else {
        std::cout << "[LOI] Khong tim thay nhan vat co ID = " << id << "\n";
    }
}

void Menu::doFindCharacter() const {
    std::cout << "\n-- Tim kiem nhan vat --\n";
    std::cout << "  1. Tim theo ID\n";
    std::cout << "  2. Tim theo ten (khong phan biet hoa/thuong)\n";
    int choice = readInt("Chon: ", 1, 2);

    if (choice == 1) {
        int id = readInt("Nhap ID: ", 1, std::numeric_limits<int>::max());
        const Character* ch = m_roster.findById(id);
        if (ch != nullptr) {
            ch->display();
        }
        else {
            std::cout << "[THONG BAO] Khong tim thay nhan vat co ID = " << id << "\n";
        }
    }
    else {
        std::string keyword;
        readNonEmptyString("Nhap mot phan ten: ", keyword);
        const std::vector<Character*> found = m_roster.findByName(keyword); // trả về số kết quả và tự in
        if (found.size()  == 0) {
            std::cout << "[THONG BAO] Khong co nhan vat phu hop voi tu khoa \"" << keyword << "\"\n";
        }
        else {
            std::cout << "[THONG BAO] Tim thay " << found.size() << " nhan vat phu hop voi tu khoa \"" << keyword << "\":\n";
            for (const Character* ch : found) {
                ch->display();
			}
        }
    }
}

// ============================================================
//  Sub-menu: Team
// ============================================================
void Menu::showTeamMenu() const {
    std::cout << "\n--- Team ---\n";
    std::cout << "  1. Hien thi danh sach Team\n";
    std::cout << "  2. Tao Team moi\n";
    std::cout << "  3. Doi ten Team\n";
    std::cout << "  4. Xoa Team\n";
    std::cout << "  5. Them nhan vat vao Team\n";
    std::cout << "  6. Xoa nhan vat khoi Team\n";
    std::cout << "  0. Quay lai\n";
    std::cout << "------------\n";
}

void Menu::handleTeamMenu() {
    bool back = false;
    while (!back) {
        showTeamMenu();
        int choice = readInt("Chon: ", 0, 6);
        switch (choice) {
        case 1: clearScreenWithLoading(); doListTeams();               break;
        case 2: clearScreenWithLoading(); doCreateTeam();              break;
        case 3: clearScreenWithLoading(); doRenameTeam();              break;
        case 4: clearScreenWithLoading(); doDeleteTeam();              break;
        case 5: clearScreenWithLoading(); doAddCharacterToTeam();      break;
        case 6: clearScreenWithLoading(); doRemoveCharacterFromTeam(); break;
        case 0: clearScreenWithLoading(); back = true;                 break;
        }
    }
}

void Menu::doListTeams() const {
    m_teamManager.displayAllTeams(m_roster);
}

void Menu::doCreateTeam() {
    std::cout << "\n-- Tao Team moi --\n";
    int teamId = readInt("Nhap Team ID (so nguyen duong): ", 1, std::numeric_limits<int>::max());

    std::string teamName;
    readNonEmptyString("Nhap ten Team: ", teamName);

    bool ok = m_teamManager.createTeam(teamId, teamName);
    if (ok) {
       std::cout << "[OK] Da tao Team \"" << teamName << "\" (ID=" << teamId << ")\n";
    }
    else {
       std::cout << "[LOI] Khong the tao Team (ID trung hoac ten rong/trung).\n";
    }
}

void Menu::doRenameTeam() {
    std::cout << "\n-- Doi ten Team --\n";
    int teamId = readInt("Nhap Team ID: ", 1, std::numeric_limits<int>::max());

    std::string newName;
    readNonEmptyString("Ten moi: ", newName);

    bool ok = m_teamManager.renameTeam(teamId, newName);
    if (ok) {
       std::cout << "[OK] Da doi ten Team ID=" << teamId << " thanh \"" << newName << "\"\n";
    }
    else {
       std::cout << "[LOI] Khong tim thay Team hoac ten moi bi trung.\n";
    }
}

void Menu::doDeleteTeam() {
    std::cout << "\n-- Xoa Team --\n";
    int teamId = readInt("Nhap Team ID: ", 1, std::numeric_limits<int>::max());

    bool ok = m_teamManager.deleteTeam(teamId);
    if (ok) {
       std::cout << "[OK] Da xoa Team ID=" << teamId << "\n";
    }
    else {
       std::cout << "[LOI] Khong tim thay Team ID=" << teamId << "\n";
    }
}

void Menu::doAddCharacterToTeam() {
    std::cout << "\n-- Them nhan vat vao Team --\n";
    int teamId = readInt("Nhap Team ID: ", 1, std::numeric_limits<int>::max());
    int charId = readInt("Nhap Character ID: ", 1, std::numeric_limits<int>::max());

    bool ok = m_teamManager.addCharacterToTeam(teamId, charId, m_roster);

    if (ok) {
       std::cout << "[OK] Da them Character ID=" << charId << " vao Team ID=" << teamId << "\n";
    }
    else {
       std::cout << "[LOI] Khong the them (Team day, ID trung, hoac Team khong ton tai).\n";
    }
}

void Menu::doRemoveCharacterFromTeam() {
    std::cout << "\n-- Xoa nhan vat khoi Team --\n";
    int teamId = readInt("Nhap Team ID: ", 1, std::numeric_limits<int>::max());
    int charId = readInt("Nhap Character ID: ", 1, std::numeric_limits<int>::max());

    bool ok = m_teamManager.removeCharacterFromTeam(teamId, charId);
    if (ok) {
       std::cout << "[OK] Da xoa Character ID=" << charId << " khoi Team ID=" << teamId << "\n";
    }
    else {
       std::cout << "[LOI] Khong tim thay Character hoac Team tuong ung.\n";
    }
}

// ============================================================
//  Battle: guided step-by-step flow
// ============================================================
void Menu::handleBattleMenu() {
    // Reset trận cũ (nếu có) để cho phép chơi trận mới
    m_battleEngine.reset();

    // Bước 1: Chọn hai Team
    if (!doSelectTeams()) {
        return; // Quay về menu chính nếu chọn thất bại
    }

    // Bước 2: Bắt đầu trận đấu
    if (!doStartBattle()) {
        return;
    }

    // Bước 3: Vòng lặp lượt — chạy cho đến khi trận đấu kết thúc
    while (m_battleEngine.isInProgress()) {
        clearScreenWithLoading();

        // Hiển thị header lượt
        const Battle& battle = m_battleEngine.getBattle();
        std::cout << "\n============== LUOT " << battle.getTurnNumber() << " ==============\n";

        // Hiển thị bảng trạng thái
        doPrintBattleStatus();

        // Hiển thị nhân vật đang đến lượt
        const Character* actor = m_battleEngine.getCurrentActor();
        if (actor == nullptr) {
            std::cout << "[LOI] Khong co nhan vat nao den luot.\n";
            break;
        }

        int actorSide = battle.getCurrentSide();
        const std::string& teamName = (actorSide == 0)
            ? battle.getTeamAName() : battle.getTeamBName();

        std::cout << "\n>> Doi: " << teamName
                  << " | Den luot: [" << actor->getId() << "] "
                  << actor->getName() << " (" << actor->getType() << ")\n";

        // Hiển thị danh sách đối thủ còn sống
        doPrintAliveEnemies(actorSide);

        // Nhập target — lặp cho đến khi hợp lệ
        bool actionOk = false;
        while (!actionOk) {
            int targetId = readInt("Chon ID nhan vat doi thu de tan cong: ",
                                   1, std::numeric_limits<int>::max());
            actionOk = m_battleEngine.performCurrentAction(targetId);
            if (!actionOk) {
                std::cout << "[LOI] Muc tieu khong hop le (sai doi, da chet, hoac khong ton tai). Chon lai.\n";
            }
        }

        std::cout << "\n";

        // Kiểm tra kết thúc trận
        if (m_battleEngine.isFinished()) {
            break;
        }

        // Chờ người chơi xác nhận trước khi chuyển lượt
        doWaitForEnter();
    }

    // Bước 4: Kết quả trận đấu
    clearScreenWithLoading();
    std::cout << "\n============== KET QUA ==============\n";
    doPrintBattleStatus();

    const std::string* winnerName = m_battleEngine.getWinnerName();
    if (winnerName != nullptr) {
        std::cout << "\n*** TRAN DAU KET THUC! ***\n";
        std::cout << "*** DOI THANG: " << *winnerName << " ***\n";
    }
    std::cout << "=====================================\n";

    doWaitForEnter();
}

bool Menu::doSelectTeams() {
    std::cout << "\n-- Chon hai Team tham chien --\n";
    std::cout << "Danh sach Team hien co:\n";
    m_teamManager.displayAllTeams(m_roster);

    int idA = readInt("Nhap Team ID (doi A): ", 1, std::numeric_limits<int>::max());
    int idB = readInt("Nhap Team ID (doi B): ", 1, std::numeric_limits<int>::max());

    // Lấy pointer tới Team từ TeamManager
    const Team* teamA = m_teamManager.getTeam(idA);
    const Team* teamB = m_teamManager.getTeam(idB);

    if (teamA == nullptr) {
       std::cout << "[LOI] Khong tim thay Team ID=" << idA << "\n";
       return false;
    }
    if (teamB == nullptr) {
       std::cout << "[LOI] Khong tim thay Team ID=" << idB << "\n";
       return false;
    }

    // Validation: hai Team khác nhau, không rỗng (FR-03 / TC-06)
    bool ok = m_battleEngine.selectTeams(teamA, teamB, m_roster);
    if (ok) {
       std::cout << "[OK] Da chon doi A: \"" << teamA->getName()
           << "\" vs doi B: \"" << teamB->getName() << "\"\n";
    }
    else {
       std::cout << "[LOI] Khong the chon (Team rong, cung mot Team, hoac nhan vat trung nhau).\n";
    }
    return ok;
}

bool Menu::doStartBattle() {
    std::cout << "\n-- Bat dau tran dau --\n";
    bool ok = m_battleEngine.startBattle();
    if (ok) {
        std::cout << "[OK] Tran dau bat dau! Trang thai: IN_PROGRESS\n";
    }
    else {
        std::cout << "[LOI] Chua chon du hai Team hoac tran dau da dang dien ra.\n";
    }
    return ok;
}

void Menu::doPrintBattleStatus() const {
    m_battleEngine.printStatus(m_roster);
}

void Menu::doPrintAliveEnemies(int actorSide) const {
    int enemySide = 1 - actorSide;
    const Battle& battle = m_battleEngine.getBattle();
    int size = (enemySide == 0) ? battle.getSizeA() : battle.getSizeB();

    std::cout << "\nMuc tieu kha dung:\n";
    for (int i = 0; i < size; ++i) {
        const Character* ch = battle.getSlot(enemySide, i);
        if (ch == nullptr || !ch->isAlive()) continue;
        std::cout << "  [" << ch->getId() << "] " << ch->getName()
                  << " - HP: " << ch->getCurrentHp() << "/" << ch->getMaxHp() << "\n";
    }
    std::cout << "\n";
}

void Menu::doWaitForEnter() const {
    std::cout << "[Nhan Enter de tiep tuc...]";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ============================================================
//  Tiện ích nhập liệu  
// ============================================================

int Menu::readInt(const char* prompt, int minVal, int maxVal) const {
    int value = 0;
    while (true) {
        if (prompt && prompt[0] != '\0') {
            std::cout << prompt;
        }
        if (std::cin >> value) {
            clearInputStream();
            if (value >= minVal && value <= maxVal) {
                return value;
            }
            std::cout << "[LOI] Vui long nhap so trong khoang [" << minVal << ", " << maxVal << "].\n";
        }
        else {
            if (std::cin.eof()) {
                std::cout << "\n[LOI] Phat hien ket thuc luong nhap (EOF). Thoat chuong trinh.\n";
                exit(0);
            }
            clearInputStream();
            std::cout << "[LOI] Input khong hop le (phai la so nguyen). Vui long nhap lai.\n";
        }
    }
}

void Menu::readNonEmptyString(const char* prompt, std::string& out) const {
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, out)) {
            if (std::cin.eof()) {
                std::cout << "\n[LOI] Phat hien ket thuc luong nhap (EOF). Thoat chuong trinh.\n";
                exit(0);
            }
            std::cin.clear();
            //Bỏ qua tất cả ký tự cho đến khi gặp Ente - dọn dẹp buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        // Trim khoảng trắng đầu cuối đơn giản
        size_t start = out.find_first_not_of(" \t\r\n");
        if (start != std::string::npos) {
            size_t end = out.find_last_not_of(" \t\r\n");
            out = out.substr(start, end - start + 1);
            return;
        }
        std::cout << "[LOI] Noi dung nhap vao khong duoc de trong. Vui long nhap lai.\n";
    }
}

void Menu::clearInputStream() const {
    if (std::cin.fail()) {
        std::cin.clear();
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Menu::clearScreenWithLoading() const {
    const int dotCount = 3;
    const int delayPerDot = LOADING_DELAY_MS / dotCount;

    std::cout << "Loading";
    std::cout.flush();

    for (int i = 0; i < dotCount; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delayPerDot));
        std::cout << ".";
        std::cout.flush();
    }

    // Clear screen and reset cursor
    std::cout << "\033[2J\033[1;1H";
}

