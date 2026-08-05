#include "pch.h"
#include "Team.h"
#include <algorithm>
#include <iostream>

using namespace std;

Team::Team() : teamId(0), teamName("") {}

Team::Team(int id, string name) : teamId(id), teamName(name) {}

Team::~Team() {}

int Team::getId() const
{
    return teamId;
}

string Team::getName() const
{
    return teamName;
}

void Team::setName(string name)
{
    teamName = name;
}

const vector<int>& Team::getCharacterIds() const
{
    return characterIds;
}

bool Team::addCharacter(int charId)
{
    if (charId <= 0) {
        cout << "Loi: ID nhan vat phai la so nguyen duong!" << endl;
        return false;
    }
    if (hasCharacter(charId)) {
        cout << "Loi: Nhan vat voi ID " << charId << " da ton tai trong Team " << teamName << "." << endl;
        return false;
    }
    if (characterIds.size() >= 5) {
        cout << "Loi: Team " << teamName << " da dat gioi han toi da 5 nhan vat!" << endl;
        return false;
    }
    characterIds.push_back(charId);
    return true;
}

bool Team::removeCharacter(int charId)
{
    auto it = find(characterIds.begin(), characterIds.end(), charId);
    if (it != characterIds.end()) {
        characterIds.erase(it);
        return true;
    }
    return false;
}

bool Team::hasCharacter(int charId) const
{
    return find(characterIds.begin(), characterIds.end(), charId) != characterIds.end();
}
