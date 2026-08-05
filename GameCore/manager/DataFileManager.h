#pragma once
#include <string>
#include "CharacterRoster.h"
#include "TeamManager.h"

using namespace std;

class DataFileManager
{
public:
    static bool loadTeams(const string& filepath, TeamManager& teamManager, const CharacterRoster& roster);
    static bool saveTeams(const string& filepath, const TeamManager& teamManager);

    //Author: Nguyen Dinh Dung
		static bool loadCharacters(const string& filepath, CharacterRoster& roster);
		static bool saveCharacters(const string& filepath, const CharacterRoster& roster);
};
