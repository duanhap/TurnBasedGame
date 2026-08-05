#include "CharacterRoster.h"
#include "TeamManager.h"
#include "BattleEngine.h"
#include "DataFileManager.h"
#include "Menu.h"

int main() {
    // 1. Khởi tạo domain objects
    CharacterRoster roster;
    TeamManager     teamManager;

    // 2. Load dữ liệu từ file (DataFileManager dùng static methods)
    DataFileManager::loadCharacters("data/characters.txt", roster);
    DataFileManager::loadTeams("data/teams.txt", teamManager, roster);

    // 3. Khởi tạo engine
    BattleEngine engine;

    // 4. Khởi tạo và chạy app
    Menu menu(roster, teamManager, engine);
    menu.run();

    return 0;
}
