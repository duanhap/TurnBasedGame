#pragma once
#include "../model/Team.h"
#include "CharacterRoster.h"
#include <vector>
#include <string>

using namespace std;

class TeamManager
{
private:
    vector<Team> teams;

public:
    TeamManager();
    ~TeamManager();

    bool createTeam(int teamId, string teamName);
    bool renameTeam(int teamId, string newName);
    bool deleteTeam(int teamId);
    
    bool addCharacterToTeam(int teamId, int characterId, const CharacterRoster& roster);
    bool removeCharacterFromTeam(int teamId, int characterId);
    void removeCharacterFromAllTeams(int characterId);

    const vector<Team>& getTeams() const;
    vector<Team>& getTeamsMutable();
    const Team* getTeam(int teamId) const;
    Team* getTeamMutable(int teamId);
    bool hasTeamId(int teamId) const;
    bool hasTeamName(string teamName) const;
    void clear();
    void displayAllTeams(const CharacterRoster& roster) const;
};
