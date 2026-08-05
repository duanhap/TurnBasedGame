#include "pch.h"
#include "TeamManager.h"
#include <algorithm>
#include <iostream>

using namespace std;

TeamManager::TeamManager() {}

TeamManager::~TeamManager() {}

void TeamManager::clear()
{
    teams.clear();
}

bool TeamManager::createTeam(int teamId, string teamName)
{
    if (teamId <= 0) {
        cout << "Loi: Team ID phai la so nguyen duong!" << endl;
        return false;
    }
    if (teamName.empty()) {
        cout << "Loi: Ten Team khong duoc de trong!" << endl;
        return false;
    }
    if (hasTeamId(teamId)) {
        cout << "Loi: Team ID " << teamId << " da ton tai!" << endl;
        return false;
    }
    if (hasTeamName(teamName)) {
        cout << "Loi: Ten Team '" << teamName << "' da ton tai!" << endl;
        return false;
    }
    if (teams.size() >= 10) {
        cout << "Loi: Khong the tao them Team. Da dat gioi han toi da 10 Teams!" << endl;
        return false;
    }
    teams.push_back(Team(teamId, teamName));
    return true;
}

bool TeamManager::renameTeam(int teamId, string newName)
{
    if (newName.empty()) {
        cout << "Loi: Ten Team khong duoc de trong!" << endl;
        return false;
    }
    Team* t = getTeamMutable(teamId);
    if (t == nullptr) {
        cout << "Loi: Khong tim thay Team voi ID " << teamId << "!" << endl;
        return false;
    }
    for (const auto& other : teams) {
        if (other.getId() != teamId && other.getName() == newName) {
            cout << "Loi: Ten Team '" << newName << "' da duoc su dung boi Team khac!" << endl;
            return false;
        }
    }
    t->setName(newName);
    return true;
}

bool TeamManager::deleteTeam(int teamId)
{
    for (auto it = teams.begin(); it != teams.end(); ++it) {
        if (it->getId() == teamId) {
            teams.erase(it);
            return true;
        }
    }
    return false;
}

bool TeamManager::addCharacterToTeam(int teamId, int characterId, const CharacterRoster& roster)
{
    Team* t = getTeamMutable(teamId);
    if (t == nullptr) {
        cout << "Loi: Khong tim thay Team voi ID " << teamId << "!" << endl;
        return false;
    }
    if (!roster.hasCharacter(characterId)) {
        cout << "Loi: ID nhan vat " << characterId << " khong ton tai trong Character Roster!" << endl;
        return false;
    }
    return t->addCharacter(characterId);
}

bool TeamManager::removeCharacterFromTeam(int teamId, int characterId)
{
    Team* t = getTeamMutable(teamId);
    if (t == nullptr) {
        cout << "Loi: Khong tim thay Team voi ID " << teamId << "!" << endl;
        return false;
    }
    return t->removeCharacter(characterId);
}

void TeamManager::removeCharacterFromAllTeams(int characterId)
{
    for (auto& team : teams) {
        team.removeCharacter(characterId);
    }
}

const vector<Team>& TeamManager::getTeams() const
{
    return teams;
}

vector<Team>& TeamManager::getTeamsMutable()
{
    return teams;
}

const Team* TeamManager::getTeam(int teamId) const
{
    for (const auto& team : teams) {
        if (team.getId() == teamId) {
            return &team;
        }
    }
    return nullptr;
}

Team* TeamManager::getTeamMutable(int teamId)
{
    for (auto& team : teams) {
        if (team.getId() == teamId) {
            return &team;
        }
    }
    return nullptr;
}

bool TeamManager::hasTeamId(int teamId) const
{
    return getTeam(teamId) != nullptr;
}

bool TeamManager::hasTeamName(string teamName) const
{
    for (const auto& team : teams) {
        if (team.getName() == teamName) {
            return true;
        }
    }
    return false;
}

void TeamManager::displayAllTeams(const CharacterRoster& roster) const
{
    if (teams.empty()) {
        cout << "(Chua co Team nao)" << endl;
        return;
    }
    cout << "\n===== DANH SACH TEAM =====" << endl;
    for (const auto& team : teams) {
        cout << "[ID: " << team.getId() << "] " << team.getName() << endl;
        const vector<int>& ids = team.getCharacterIds();
        if (ids.empty()) {
            cout << "  (Chua co nhan vat)" << endl;
            continue;
        }
        for (int i = 0; i < (int)ids.size(); ++i) {
            int charId = ids[i];
            const Character* ch = roster.findById(charId);
            cout << "  " << (i + 1) << ". ID=" << charId;
            if (ch != nullptr) {
                cout << " | " << ch->getName()
                     << " [" << ch->getType() << "]"
                     << " HP=" << ch->getMaxHp();
            } else {
                cout << " (khong tim thay trong Roster)";
            }
            cout << endl;
        }
    }
    cout << "==========================" << endl;
}
