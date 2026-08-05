#include "pch.h"
#include "DataFileManager.h"
#include "../model/Warrior.h"
#include "../model/Mage.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

static string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

static vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    if (!s.empty() && s.back() == delimiter) {
        tokens.push_back("");
    }
    return tokens;
}

static bool tryParsePositiveInt(const string& str, int& result) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    try {
        result = stoi(str);
        return result > 0;
    } catch (...) {
        return false;
    }
}

static bool tryParseNonNegativeInt(const string& str, int& result) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    try {
        result = stoi(str);
        return result >= 0;
    } catch (...) {
        return false;
    }
}

//Author: Nguyen Dinh Dung
bool DataFileManager::loadCharacters(const string& filepath, CharacterRoster& roster)
{
    ifstream infile(filepath);
    if (!infile.is_open()) {
        cout << "Thong bao: File du lieu nhan vat khong ton tai hoac khong the mo: " << filepath << ". Khoi tao danh sach nhan vat rong." << endl;
        return true;
    }

    string line;
    int lineNum = 0;
    while (getline(infile, line)) {
        lineNum++;
        string trimmed = trim(line);
        if (trimmed.empty() || trimmed[0] == '#') {
            continue;
        }

        vector<string> tokens = split(trimmed, '|');
        if (tokens.empty()) {
            cout << "Loi o dong " << lineNum << " trong file " << filepath << ": Dong rong sau khi xu ly. Bo qua dong." << endl;
            continue;
        }

        string type = tokens[0];

        if (type == "WARRIOR") {
            // WARRIOR|id|name|maxHp|attackPower
            if (tokens.size() != 5) {
                cout << "Loi o dong " << lineNum << " trong file " << filepath << ": WARRIOR can dung 5 truong (WARRIOR|id|name|maxHp|attackPower). Bo qua dong." << endl;
                continue;
            }

            int id, maxHp, attackPower;
            if (!tryParsePositiveInt(tokens[1], id)) {
                cout << "Loi o dong " << lineNum << " trong file " << filepath << ": id khong hop le. Bo qua dong." << endl;
                continue;
            }

            string name = tokens[2];
            if (name.empty()) {
                cout << "Loi o dong " << lineNum << " trong file " << filepath << ": Ten nhan vat khong duoc de trong. Bo qua dong." << endl;
                continue;
            }

            if (!tryParsePositiveInt(tokens[3], maxHp)) {
                cout << "Loi o dong " << lineNum << " trong file " << filepath << ": maxHp khong hop le. Bo qua dong." << endl;
                continue;
            }

            if (!tryParsePositiveInt(tokens[4], attackPower)) {
                cout << "Loi o dong " << lineNum << " trong file " << filepath << ": attackPower khong hop le. Bo qua dong." << endl;
                continue;
            }

            auto warrior = make_unique<Warrior>(id, name, static_cast<unsigned int>(maxHp), "WARRIOR", attackPower);
            if (roster.add(move(warrior)) == -1) {
                cout << "Loi o dong " << lineNum << " trong file " << filepath << ": Khong the them Warrior vao Roster (co the trung ID). Bo qua dong." << endl;
            }
        }
        else if (type == "MAGE") {
            // MAGE|id|name|maxHp|maxMana|spellDamage|manaCost|fallbackDamage
            if (tokens.size() != 8) {
                cout << "Loi o dong " << lineNum << " trong file " << filepath << ": MAGE can dung 8 truong (MAGE|id|name|maxHp|maxMana|spellDamage|manaCost|fallbackDamage). Bo qua dong." << endl;
                continue;
            }

            int id, maxHp, maxMana, spellDamage, manaCost, fallbackDamage;
            if (!tryParsePositiveInt(tokens[1], id)) {
                cout << "Loi o dong " << lineNum << " trong file " << filepath << ": id khong hop le. Bo qua dong." << endl;
                continue;
            }

            string name = tokens[2];
            if (name.empty()) {
                cout << "Loi o dong " << lineNum << " trong file " << filepath << ": Ten nhan vat khong duoc de trong. Bo qua dong." << endl;
                continue;
            }

            if (!tryParsePositiveInt(tokens[3], maxHp)) {
                cout << "Loi o dong " << lineNum << " trong file " << filepath << ": maxHp khong hop le. Bo qua dong." << endl;
                continue;
            }

            if (!tryParseNonNegativeInt(tokens[4], maxMana)) {
                cout << "Loi o dong " << lineNum << " trong file " << filepath << ": maxMana khong hop le. Bo qua dong." << endl;
                continue;
            }

            if (!tryParseNonNegativeInt(tokens[5], spellDamage)) {
                cout << "Loi o dong " << lineNum << " trong file " << filepath << ": spellDamage khong hop le. Bo qua dong." << endl;
                continue;
            }

            if (!tryParseNonNegativeInt(tokens[6], manaCost)) {
                cout << "Loi o dong " << lineNum << " trong file " << filepath << ": manaCost khong hop le. Bo qua dong." << endl;
                continue;
            }

            if (!tryParseNonNegativeInt(tokens[7], fallbackDamage)) {
                cout << "Loi o dong " << lineNum << " trong file " << filepath << ": fallbackDamage khong hop le. Bo qua dong." << endl;
                continue;
            }

            auto mage = make_unique<Mage>(id, name, static_cast<unsigned int>(maxHp), "MAGE",
                static_cast<unsigned int>(maxMana), static_cast<unsigned int>(spellDamage),
                static_cast<unsigned int>(manaCost), static_cast<unsigned int>(fallbackDamage));
            if (roster.add(move(mage)) == -1) {
                cout << "Loi o dong " << lineNum << " trong file " << filepath << ": Khong the them Mage vao Roster (co the trung ID). Bo qua dong." << endl;
            }
        }
        else {
            cout << "Loi o dong " << lineNum << " trong file " << filepath << ": Loai nhan vat '" << type << "' khong duoc ho tro. Bo qua dong." << endl;
        }
    }
    infile.close();
    return true;
}

//Author: Nguyen Dinh Dung
bool DataFileManager::saveCharacters(const string& filepath, const CharacterRoster& roster)
{
    ofstream outfile(filepath);
    if (!outfile.is_open()) {
        cout << "Loi: Khong the mo file de ghi: " << filepath << endl;
        return false;
    }

    outfile << "# Dinh dang du lieu Nhan vat (characters.txt)" << endl;
    outfile << "# WARRIOR|id|name|maxHp|attackPower" << endl;
    outfile << "# MAGE|id|name|maxHp|maxMana|spellDamage|manaCost|fallbackDamage" << endl;

    for (const auto* character : roster.getAllCharacters()) {
        if (!character) continue;

        string type = character->getType();
        if (type == "WARRIOR") {
            const Warrior* w = dynamic_cast<const Warrior*>(character);
            if (w) {
                outfile << "WARRIOR|" << w->getId() << "|" << w->getName() << "|"
                        << w->getMaxHp() << "|" << w->getAttackPower() << endl;
            }
        }
        else if (type == "MAGE") {
            const Mage* m = dynamic_cast<const Mage*>(character);
            if (m) {
                outfile << "MAGE|" << m->getId() << "|" << m->getName() << "|"
                        << m->getMaxHp() << "|" << m->getMaxMana() << "|"
                        << m->getSpellDamage() << "|" << m->getManaCost() << "|"
                        << m->getFallbackDamage() << endl;
            }
        }
    }
    outfile.close();
    return true;
}

bool DataFileManager::loadTeams(const string& filepath, TeamManager& teamManager, const CharacterRoster& roster)
{
    ifstream infile(filepath);
    if (!infile.is_open()) {
        cout << "Thong bao: File du lieu doi hinh khong ton tai hoac khong the mo: " << filepath << ". Khoi tao danh sach team rong." << endl;
        return true;
    }

    string line;
    int lineNum = 0;
    while (getline(infile, line)) {
        lineNum++;
        string trimmed = trim(line);
        if (trimmed.empty() || trimmed[0] == '#') {
            continue;
        }

        vector<string> tokens = split(trimmed, '|');
        if (tokens.size() < 2) {
            cout << "Loi o dong " << lineNum << " trong file " << filepath << ": Thieu truong teamId hoac teamName. Bo qua dong." << endl;
            continue;
        }

        int teamId;
        if (!tryParsePositiveInt(tokens[0], teamId)) {
            cout << "Loi o dong " << lineNum << " trong file " << filepath << ": teamId khong hop le. Bo qua dong." << endl;
            continue;
        }

        string teamName = tokens[1];
        if (teamName.empty()) {
            cout << "Loi o dong " << lineNum << " trong file " << filepath << ": Ten Team khong duoc de trong. Bo qua dong." << endl;
            continue;
        }

        if (teamManager.hasTeamId(teamId)) {
            cout << "Loi o dong " << lineNum << " trong file " << filepath << ": teamId " << teamId << " da ton tai. Bo qua dong." << endl;
            continue;
        }

        if (teamManager.hasTeamName(teamName)) {
            cout << "Loi o dong " << lineNum << " trong file " << filepath << ": Ten Team '" << teamName << "' da ton tai. Bo qua dong." << endl;
            continue;
        }

        if (teamManager.getTeams().size() >= 10) {
            cout << "Loi o dong " << lineNum << " trong file " << filepath << ": Khong the tao them Team. Da dat gioi han toi da 10 Teams! Bo qua dong." << endl;
            continue;
        }

        if (!teamManager.createTeam(teamId, teamName)) {
            cout << "Loi o dong " << lineNum << " trong file " << filepath << ": Khong the khoi tao Team. Bo qua dong." << endl;
            continue;
        }

        if (tokens.size() >= 3 && !tokens[2].empty()) {
            vector<string> charIds = split(tokens[2], ',');
            for (const string& charIdStr : charIds) {
                if (charIdStr.empty()) continue;
                int charId;
                if (!tryParsePositiveInt(charIdStr, charId)) {
                    cout << "Loi o dong " << lineNum << " trong file " << filepath << ": ID nhan vat '" << charIdStr << "' khong hop le. Bo qua nhan vat nay." << endl;
                    continue;
                }

                if (!teamManager.addCharacterToTeam(teamId, charId, roster)) {
                    cout << "Loi o dong " << lineNum << " trong file " << filepath << ": Khong the them nhan vat " << charId << " vao Team. Bo qua nhan vat nay." << endl;
                }
            }
        }
    }
    infile.close();
    return true;
}

bool DataFileManager::saveTeams(const string& filepath, const TeamManager& teamManager)
{
    ofstream outfile(filepath);
    if (!outfile.is_open()) {
        cout << "Loi: Khong the mo file de ghi: " << filepath << endl;
        return false;
    }

    outfile << "# Dinh dang du lieu Doi hinh (teams.txt)" << endl;
    outfile << "# teamId|teamName|characterId1,characterId2,..." << endl;

    for (const auto& team : teamManager.getTeams()) {
        outfile << team.getId() << "|" << team.getName() << "|";
        const auto& charIds = team.getCharacterIds();
        for (size_t i = 0; i < charIds.size(); ++i) {
            outfile << charIds[i];
            if (i + 1 < charIds.size()) {
                outfile << ",";
            }
        }
        outfile << endl;
    }
    outfile.close();
    return true;
}
