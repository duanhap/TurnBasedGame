#include <string>
#ifndef MENU_H
#define MENU_H

//#include "CharacterRoster.h"
//#include "TeamManager.h"
//#include "BattleEngine.h"
//#include "DataFileManager.h"

class Menu {
public:
    
    //Menu(CharacterRoster& roster,
    //    TeamManager& teamManager,
    //    BattleEngine& battleEngine,
    //    DataFileManager& fileManager);

    void run();

private:
    //CharacterRoster& m_roster;
    //TeamManager& m_teamManager;
    //BattleEngine& m_battleEngine;
    //DataFileManager& m_fileManager;

    // ---------------------------------------------------------------
    // Menu cấp 1
    // ---------------------------------------------------------------
    void showMainMenu()        const;
    void handleRosterMenu();
    void handleTeamMenu();
    void handleBattleMenu();
    void handleSaveExit();

    // ---------------------------------------------------------------
    // Sub-menu: Character Roster  
    // ---------------------------------------------------------------
    void showRosterMenu()      const;
    void doListCharacters()    const;
    void doAddCharacter();
    void doEditCharacter();
    void doDeleteCharacter();
    void doFindCharacter()     const;

    // ---------------------------------------------------------------
    // Sub-menu: Team  (FR-02)
    // ---------------------------------------------------------------
    void showTeamMenu()        const;
    void doListTeams()         const;
    void doCreateTeam();
    void doRenameTeam();
    void doDeleteTeam();
    void doAddCharacterToTeam();
    void doRemoveCharacterFromTeam();

    // ---------------------------------------------------------------
    // Sub-menu: Battle  
    // ---------------------------------------------------------------
    void showBattleMenu()      const;
    void doSelectTeams();
    void doStartBattle();
    void doPerformAction();
    void doPrintBattleStatus() const;

    // ---------------------------------------------------------------
    // Tiện ích nhập liệu — xử lý input không hợp lệ (TC-18)
    // ---------------------------------------------------------------

    // Đọc một số nguyên; lặp lại nếu input không phải số hoặc ngoài [min, max]
    int  readInt(const char* prompt, int min, int max) const;

    // Đọc một dòng chuỗi không rỗng
    void readNonEmptyString(const char* prompt, std::string& out) const;

    // Xóa trạng thái lỗi và ký tự thừa của cin
    void clearInputStream() const;
};

#endif // MENU_H