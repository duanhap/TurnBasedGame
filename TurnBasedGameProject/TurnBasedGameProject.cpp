#include "CharacterRoster.h"
#include "TeamManager.h"
#include "BattleEngine.h"
#include "DataFileManager.h"
#include "Menu.h"

int main() {
    // 1. Khởi tạo data layer
    DataFileManager dataManager;

    // 2. Khởi tạo domain objects
    CharacterRoster roster;
    TeamManager     teamManager;

    // 3. Load dữ liệu từ file
    //dataManager.loadCharacters(roster);
    //dataManager.loadTeams(teamManager);

    // 4. Khởi tạo engine
    BattleEngine engine;

    // 5. Khởi tạo và chạy app — main() không biết gì thêm
    Menu menu(roster, teamManager, engine, dataManager);
    menu.run();

    return 0;
}
