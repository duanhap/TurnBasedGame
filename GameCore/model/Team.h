#pragma once
#include <string>
#include <vector>

using namespace std;

class Team
{
private:
    int teamId;
    string teamName;
    vector<int> characterIds;

public:
    Team();
    Team(int id, string name);
    ~Team();

    int getId() const;
    string getName() const;
    int getSize() const;
    void setName(string name);
    const vector<int>& getCharacterIds() const;

    bool addCharacter(int charId);
    bool removeCharacter(int charId);
    bool hasCharacter(int charId) const;
};
