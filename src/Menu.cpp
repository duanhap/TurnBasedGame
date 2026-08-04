#include "Menu.h"
#include <iostream>
#include <limits>
#include <string>

// ============================================================
//  Constructor
// ============================================================
//Menu::Menu(CharacterRoster& roster,
//    TeamManager& teamManager,
//    BattleEngine& battleEngine,
//    DataFileManager& fileManager)
//    : m_roster(roster)
//    , m_teamManager(teamManager)
//    , m_battleEngine(battleEngine)
//    , m_fileManager(fileManager)
//{
//}

// ============================================================
//  Vòng lặp chính
// ============================================================
void Menu::run() {
    bool running = true;
    while (running) {
        showMainMenu();
        int choice = readInt("Chon: ", 1, 4);
        switch (choice) {
        case 1: handleRosterMenu(); break;
        case 2: handleTeamMenu();   break;
        case 3: handleBattleMenu(); break;
        case 4:
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
    //bool ok = m_fileManager.saveAll(m_roster, m_teamManager);
    //if (ok) {
    //    std::cout << "[OK] Da luu. Tam biet!\n";
    //}
    //else {
    //    std::cout << "[LOI] Khong the luu file. Kiem tra quyen truy cap.\n";
    //}
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
        case 1: doListCharacters(); break;
        case 2: doAddCharacter();   break;
        case 3: doEditCharacter();  break;
        case 4: doDeleteCharacter(); break;
        case 5: doFindCharacter();  break;
        case 0: back = true;        break;
        }
    }
}

void Menu::doListCharacters() const {
    //m_roster.displayAll();
}

void Menu::doAddCharacter() {
    std::cout << "\n-- Them nhan vat moi --\n";
    std::cout << "  1. Warrior\n";
    std::cout << "  2. Mage\n";
    int typeChoice = readInt("Chon loai: ", 1, 2);

    int id;
    std::cout << "Nhap ID (so nguyen duong): ";
    id = readInt("", 1, std::numeric_limits<int>::max());

    std::string name;
    readNonEmptyString("Nhap ten nhan vat: ", name);

    int maxHp;
    std::cout << "Nhap maxHp (> 0): ";
    maxHp = readInt("", 1, std::numeric_limits<int>::max());

    if (typeChoice == 1) {
        // Warrior
        int attackPower;
        std::cout << "Nhap attackPower (> 0): ";
        attackPower = readInt("", 1, std::numeric_limits<int>::max());

        //bool ok = m_roster.addWarrior(id, name, maxHp, attackPower);
        //if (ok) {
        //    std::cout << "[OK] Da them Warrior: " << name << "\n";
        //}
        //else {
        //    std::cout << "[LOI] Khong the them nhan vat (ID trung hoac chi so khong hop le).\n";
        //}
    }
    else {
        // Mage
        int maxMana, spellDamage, manaCost, fallbackDamage;
        std::cout << "Nhap maxMana (> 0): ";
        maxMana = readInt("", 1, std::numeric_limits<int>::max());
        std::cout << "Nhap spellDamage (> 0): ";
        spellDamage = readInt("", 1, std::numeric_limits<int>::max());
        std::cout << "Nhap manaCost (> 0): ";
        manaCost = readInt("", 1, std::numeric_limits<int>::max());
        std::cout << "Nhap fallbackDamage (> 0): ";
        fallbackDamage = readInt("", 1, std::numeric_limits<int>::max());

        //bool ok = m_roster.addMage(id, name, maxHp, maxMana, spellDamage, manaCost, fallbackDamage);
        //if (ok) {
        //    std::cout << "[OK] Da them Mage: " << name << "\n";
        //}
        //else {
        //    std::cout << "[LOI] Khong the them nhan vat (ID trung hoac chi so khong hop le).\n";
        //}
    }
}

void Menu::doEditCharacter() {
    std::cout << "\n-- Chinh sua nhan vat --\n";
    int id;
    std::cout << "Nhap ID nhan vat can sua: ";
    id = readInt("", 1, std::numeric_limits<int>::max());

    // Kiểm tra tồn tại — roster trả về pointer hoặc nullptr
    //const Character* ch = m_roster.findById(id);
    //if (ch == nullptr) {
    //    std::cout << "[LOI] Khong tim thay nhan vat co ID = " << id << "\n";
    //    return;
    //}
    //ch->display();

    std::string newName;
    readNonEmptyString("Ten moi (giu nguyen nhap lai ten cu): ", newName);

    int newMaxHp;
    std::cout << "maxHp moi (> 0): ";
    newMaxHp = readInt("", 1, std::numeric_limits<int>::max());

    bool ok = false;
    // Phân nhánh theo loại để nhập đúng chỉ số — việc quyết định loại chỉ xảy ra tại đây (UI)
    //if (ch->getType() == CharacterType::WARRIOR) {
    //    int attackPower;
    //    std::cout << "attackPower moi (> 0): ";
    //    attackPower = readInt("", 1, std::numeric_limits<int>::max());
    //    ok = m_roster.updateWarrior(id, newName, newMaxHp, attackPower);
    //}
    //else {
    //    int maxMana, spellDamage, manaCost, fallbackDamage;
    //    std::cout << "maxMana moi (> 0): ";
    //    maxMana = readInt("", 1, std::numeric_limits<int>::max());
    //    std::cout << "spellDamage moi (> 0): ";
    //    spellDamage = readInt("", 1, std::numeric_limits<int>::max());
    //    std::cout << "manaCost moi (> 0): ";
    //    manaCost = readInt("", 1, std::numeric_limits<int>::max());
    //    std::cout << "fallbackDamage moi (> 0): ";
    //    fallbackDamage = readInt("", 1, std::numeric_limits<int>::max());
    //    ok = m_roster.updateMage(id, newName, newMaxHp, maxMana, spellDamage, manaCost, fallbackDamage);
    //}

    if (ok) {
        std::cout << "[OK] Da cap nhat nhan vat ID = " << id << "\n";
    }
    else {
        std::cout << "[LOI] Khong the cap nhat (chi so khong hop le).\n";
    }
}

void Menu::doDeleteCharacter() {
    std::cout << "\n-- Xoa nhan vat --\n";
    int id;
    std::cout << "Nhap ID nhan vat can xoa: ";
    id = readInt("", 1, std::numeric_limits<int>::max());

    // Xóa khỏi tất cả Team trước (FR-01: khi xóa phải loại khỏi mọi Team)
    // m_teamManager.removeCharacterFromAllTeams(id);

    //bool ok = m_roster.removeById(id);
    //if (ok) {
    //    std::cout << "[OK] Da xoa nhan vat ID = " << id << " va loai khoi moi Team.\n";
    //}
    //else {
    //    std::cout << "[LOI] Khong tim thay nhan vat co ID = " << id << "\n";
    //}
}

void Menu::doFindCharacter() const {
    std::cout << "\n-- Tim kiem nhan vat --\n";
    std::cout << "  1. Tim theo ID\n";
    std::cout << "  2. Tim theo ten (khong phan biet hoa/thuong)\n";
    int choice = readInt("Chon: ", 1, 2);

    if (choice == 1) {
        int id;
        std::cout << "Nhap ID: ";
        id = readInt("", 1, std::numeric_limits<int>::max());
        //const Character* ch = m_roster.findById(id);
        //if (ch != nullptr) {
        //    ch->display();
        //}
        //else {
        //    std::cout << "[THONG BAO] Khong tim thay nhan vat co ID = " << id << "\n";
        //}
    }
    else {
        std::string keyword;
        readNonEmptyString("Nhap mot phan ten: ", keyword);
        //int found = m_roster.findByName(keyword); // trả về số kết quả và tự in
        //if (found == 0) {
        //    std::cout << "[THONG BAO] Khong co nhan vat phu hop voi tu khoa \"" << keyword << "\"\n";
        //}
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
        case 1: doListTeams();               break;
        case 2: doCreateTeam();              break;
        case 3: doRenameTeam();              break;
        case 4: doDeleteTeam();              break;
        case 5: doAddCharacterToTeam();      break;
        case 6: doRemoveCharacterFromTeam(); break;
        case 0: back = true;                 break;
        }
    }
}

void Menu::doListTeams() const {
    //m_teamManager.displayAll(m_roster);
}

void Menu::doCreateTeam() {
    std::cout << "\n-- Tao Team moi --\n";
    int teamId;
    std::cout << "Nhap Team ID (so nguyen duong): ";
    teamId = readInt("", 1, std::numeric_limits<int>::max());

    std::string teamName;
    readNonEmptyString("Nhap ten Team: ", teamName);

    //bool ok = m_teamManager.createTeam(teamId, teamName);
    //if (ok) {
    //    std::cout << "[OK] Da tao Team \"" << teamName << "\" (ID=" << teamId << ")\n";
    //}
    //else {
    //    std::cout << "[LOI] Khong the tao Team (ID trung hoac ten rong/trung).\n";
    //}
}

void Menu::doRenameTeam() {
    std::cout << "\n-- Doi ten Team --\n";
    int teamId;
    std::cout << "Nhap Team ID: ";
    teamId = readInt("", 1, std::numeric_limits<int>::max());

    std::string newName;
    readNonEmptyString("Ten moi: ", newName);

    //bool ok = m_teamManager.renameTeam(teamId, newName);
    //if (ok) {
    //    std::cout << "[OK] Da doi ten Team ID=" << teamId << " thanh \"" << newName << "\"\n";
    //}
    //else {
    //    std::cout << "[LOI] Khong tim thay Team hoac ten moi bi trung.\n";
    //}
}

void Menu::doDeleteTeam() {
    std::cout << "\n-- Xoa Team --\n";
    int teamId;
    std::cout << "Nhap Team ID: ";
    teamId = readInt("", 1, std::numeric_limits<int>::max());

    //bool ok = m_teamManager.deleteTeam(teamId);
    //if (ok) {
    //    std::cout << "[OK] Da xoa Team ID=" << teamId << "\n";
    //}
    //else {
    //    std::cout << "[LOI] Khong tim thay Team ID=" << teamId << "\n";
    //}
}

void Menu::doAddCharacterToTeam() {
    std::cout << "\n-- Them nhan vat vao Team --\n";
    int teamId;
    std::cout << "Nhap Team ID: ";
    teamId = readInt("", 1, std::numeric_limits<int>::max());

    int charId;
    std::cout << "Nhap Character ID: ";
    charId = readInt("", 1, std::numeric_limits<int>::max());

    // Kiểm tra Character tồn tại trong Roster trước khi chuyển cho TeamManager
    //if (m_roster.findById(charId) == nullptr) {
    //    std::cout << "[LOI] Character ID=" << charId << " khong ton tai trong Roster.\n";
    //    return;
    //}

    //bool ok = m_teamManager.addCharacterToTeam(teamId, charId);
    //if (ok) {
    //    std::cout << "[OK] Da them Character ID=" << charId << " vao Team ID=" << teamId << "\n";
    //}
    //else {
    //    std::cout << "[LOI] Khong the them (Team day, ID trung, hoac Team khong ton tai).\n";
    //}
}

void Menu::doRemoveCharacterFromTeam() {
    std::cout << "\n-- Xoa nhan vat khoi Team --\n";
    int teamId;
    std::cout << "Nhap Team ID: ";
    teamId = readInt("", 1, std::numeric_limits<int>::max());

    int charId;
    std::cout << "Nhap Character ID: ";
    charId = readInt("", 1, std::numeric_limits<int>::max());

    //bool ok = m_teamManager.removeCharacterFromTeam(teamId, charId);
    //if (ok) {
    //    std::cout << "[OK] Da xoa Character ID=" << charId << " khoi Team ID=" << teamId << "\n";
    //}
    //else {
    //    std::cout << "[LOI] Khong tim thay Character hoac Team tuong ung.\n";
    //}
}

// ============================================================
//  Sub-menu: Battle
// ============================================================
void Menu::showBattleMenu() const {
    std::cout << "\n--- Battle ---\n";
    std::cout << "  1. Chon hai Team tham chien\n";
    std::cout << "  2. Bat dau tran dau (Start)\n";
    std::cout << "  3. Thuc hien hanh dong luot hien tai\n";
    std::cout << "  4. Xem trang thai tran dau\n";
    std::cout << "  0. Quay lai\n";
    std::cout << "--------------\n";
}

void Menu::handleBattleMenu() {
    bool back = false;
    while (!back) {
        showBattleMenu();
        int choice = readInt("Chon: ", 0, 4);
        switch (choice) {
        case 1: doSelectTeams();      break;
        case 2: doStartBattle();      break;
        case 3: doPerformAction();    break;
        case 4: doPrintBattleStatus(); break;
        case 0: back = true;          break;
        }
    }
}

void Menu::doSelectTeams() {
    std::cout << "\n-- Chon hai Team tham chien --\n";
    std::cout << "Danh sach Team hien co:\n";
    //m_teamManager.displayAll(m_roster);

    int idA, idB;
    std::cout << "Nhap Team ID (doi A): ";
    idA = readInt("", 1, std::numeric_limits<int>::max());
    std::cout << "Nhap Team ID (doi B): ";
    idB = readInt("", 1, std::numeric_limits<int>::max());

    // Lấy pointer tới Team từ TeamManager
    //const Team* teamA = m_teamManager.findById(idA);
    //const Team* teamB = m_teamManager.findById(idB);

    //if (teamA == nullptr) {
    //    std::cout << "[LOI] Khong tim thay Team ID=" << idA << "\n";
    //    return;
    //}
    //if (teamB == nullptr) {
    //    std::cout << "[LOI] Khong tim thay Team ID=" << idB << "\n";
    //    return;
    //}

    //// Validation: hai Team khác nhau, không rỗng (FR-03 / TC-06)
    //bool ok = m_battleEngine.selectTeams(teamA, teamB, m_roster);
    //if (ok) {
    //    std::cout << "[OK] Da chon doi A: \"" << teamA->getName()
    //        << "\" vs doi B: \"" << teamB->getName() << "\"\n";
    //}
    //else {
    //    std::cout << "[LOI] Khong the chon (Team rong, cung mot Team, hoac nhan vat trung nhau).\n";
    //}
}

void Menu::doStartBattle() {
    std::cout << "\n-- Bat dau tran dau --\n";
    //bool ok = m_battleEngine.startBattle();
    //if (ok) {
    //    std::cout << "[OK] Tran dau bat dau! Trang thai: IN_PROGRESS\n";
    //    doPrintBattleStatus();
    //}
    //else {
    //    std::cout << "[LOI] Chua chon du hai Team hoac tran dau da dang dien ra.\n";
    //}
}

void Menu::doPerformAction() {
    // BattleEngine tự xác định actor hiện tại và yêu cầu chọn target
    //if (!m_battleEngine.isInProgress()) {
    //    std::cout << "[LOI] Tran dau chua bat dau hoac da ket thuc (TC-15).\n";
    //    return;
    //}

    // Hiển thị trạng thái trước khi hành động
    doPrintBattleStatus();

    // BattleEngine cho biết nhân vật đang đến lượt
    //const Character* actor = m_battleEngine.getCurrentActor();
    //if (actor == nullptr) {
    //    std::cout << "[LOI] Khong co nhan vat nao den luot.\n";
    //    return;
    //}
    //std::cout << "\nDen luot: [" << actor->getId() << "] " << actor->getName() << "\n";

    //// Hiển thị danh sách đối thủ còn sống để chọn target
    //std::cout << "Chon ID nhan vat doi thu (con song) de tan cong: ";
    //int targetId = readInt("", 1, std::numeric_limits<int>::max());

    //bool ok = m_battleEngine.performCurrentAction(targetId);
    //if (ok) {
    //    std::cout << "[OK] Hanh dong thanh cong.\n";
    //    doPrintBattleStatus();

    //    // Kiểm tra kết thúc trận
    //    if (m_battleEngine.isFinished()) {
    //        const Team* winner = m_battleEngine.getWinner();
    //        std::cout << "\n*** TRAN DAU KET THUC! ***\n";
    //        std::cout << "*** DOI THANG: " << winner->getName() << " ***\n";
    //    }
    //}
    //else {
    //    std::cout << "[LOI] Hanh dong khong hop le (sai luot, target het HP, hoac target khong ton tai).\n";
    //}
}

void Menu::doPrintBattleStatus() const {
    //m_battleEngine.printStatus(m_roster);
}

// ============================================================
//  Tiện ích nhập liệu  (TC-18: xử lý input sai)
// ============================================================

int Menu::readInt(const char* prompt, int minVal, int maxVal) const {
    int value = 0;
    while (true) {
        if (prompt && prompt[0] != '\0') {
            std::cout << prompt;
        }
        if (std::cin >> value) {
            // Đọc thành công — kiểm tra phạm vi
            clearInputStream(); // bỏ ký tự '\n' còn lại
            if (value >= minVal && value <= maxVal) {
                return value;
            }
            std::cout << "[LOI] Vui long nhap so trong khoang [" << minVal << ", " << maxVal << "]: ";
        }
        else {
            // Không phải số nguyên
            clearInputStream();
            std::cout << "[LOI] Input khong hop le. Vui long nhap lai: ";
        }
    }
}

void Menu::readNonEmptyString(const char* prompt, std::string& out) const {
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, out);
        // Trim khoảng trắng đầu cuối đơn giản
        size_t start = out.find_first_not_of(" \t\r\n");
        if (start != std::string::npos) {
            size_t end = out.find_last_not_of(" \t\r\n");
            out = out.substr(start, end - start + 1);
            return;
        }
        std::cout << "[LOI] Ten khong duoc rong. Vui long nhap lai.\n";
    }
}

void Menu::clearInputStream() const {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}
