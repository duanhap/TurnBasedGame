#include "pch.h"
#include "DataFileManager.h"
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
