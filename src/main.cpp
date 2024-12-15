#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

class PlayerStats {
public:
    string playerName;
    string role;
    string champion;
    int kills;
    int deaths;
    int assists;
    int damage;
    int gold;
    int cs;
    int wardsPlaced;
    int wardsCleared;
    int visionScore;
    int level;

    PlayerStats(const string& pName, const string& r, const string& c, int k, int d, int a,
                int dmg, int g, int cs, int wp, int wc, int vs, int lv)
        : playerName(pName), role(r), champion(c), kills(k), deaths(d), assists(a),
          damage(dmg), gold(g), cs(cs), wardsPlaced(wp), wardsCleared(wc),
          visionScore(vs), level(lv) {}
};

class Match {
public:
    string matchID;
    vector<PlayerStats> blueTeam;
    vector<PlayerStats> redTeam;
    int playtime;
    bool blueTeamWin;

    Match(const string& mID, const vector<PlayerStats>& bTeam, const vector<PlayerStats>& rTeam, int pt, bool bWin)
        : matchID(mID), blueTeam(bTeam), redTeam(rTeam), playtime(pt), blueTeamWin(bWin) {}

    string toString() const {
        stringstream ss;
        ss << matchID << " " << playtime << " " << (blueTeamWin ? "1" : "0") << " ";

        auto appendTeamData = [&ss](const vector<PlayerStats>& team) {
            for (const auto& p : team) {
                ss << p.playerName << " " << p.role << " " << p.champion << " "
                   << p.kills << " " << p.deaths << " " << p.assists << " "
                   << p.damage << " " << p.gold << " " << p.cs << " "
                   << p.wardsPlaced << " " << p.wardsCleared << " " << p.visionScore << " " << p.level << " ";
            }
        };

        appendTeamData(blueTeam);
        appendTeamData(redTeam);

        return ss.str();
    }
};

class StatsTracker {
private:
    vector<Match> matches;
    const string dataFilename = "match_data.txt";

public:
    StatsTracker() {
        loadMatches();
    }

    ~StatsTracker() {
        saveMatches();
    }

    void addMatch(const Match& match) {
        matches.push_back(match);
    }

    void saveMatches() const {
        ofstream outFile(dataFilename, ios::trunc);
        for (const auto& match : matches) {
            outFile << match.toString() << endl;
        }
    }

    void loadMatches() {
        ifstream inFile(dataFilename);
        string line;

        while (getline(inFile, line)) {
            stringstream ss(line);
            string mID;
            int pt;
            bool bWin;

            ss >> mID >> pt >> bWin;

            vector<PlayerStats> bTeam;
            vector<PlayerStats> rTeam;

            auto loadTeamData = [&ss]() {
                vector<PlayerStats> team;
                for (int i = 0; i < 5; ++i) {
                    string pName, r, c;
                    int k, d, a, dmg, g, cs, wp, wc, vs, lv;
                    ss >> pName >> r >> c >> k >> d >> a >> dmg >> g >> cs >> wp >> wc >> vs >> lv;
                    team.emplace_back(pName, r, c, k, d, a, dmg, g, cs, wp, wc, vs, lv);
                }
                return team;
            };

            bTeam = loadTeamData();
            rTeam = loadTeamData();

            matches.emplace_back(mID, bTeam, rTeam, pt, bWin);
        }
    }

    void displayAllMatches() const {
        if (matches.empty()) {
            cout << "No matches recorded.\n";
            return;
        }

        cout << "\n[Match History]\n";
        for (const auto& match : matches) {
            cout << "Match ID: " << match.matchID << "\n";
            cout << "Duration: " << match.playtime << " minutes\n";
            cout << "Winner: " << (match.blueTeamWin ? "Blue Team" : "Red Team") << "\n";

            cout << "\nBlue Team:\n";
            for (const auto& p : match.blueTeam) {
                cout << "  Player: " << p.playerName << ", Role: " << p.role << ", Champion: " << p.champion << "\n";
                cout << "  Stats: " << p.kills << "/" << p.deaths << "/" << p.assists << ", Damage: " << p.damage
                     << ", Gold: " << p.gold << ", CS: " << p.cs << "\n";
                cout << "--------------------------------------\n";
            }

            cout << "\nRed Team:\n";
            for (const auto& p : match.redTeam) {
                cout << "  Player: " << p.playerName << ", Role: " << p.role << ", Champion: " << p.champion << "\n";
                cout << "  Stats: " << p.kills << "/" << p.deaths << "/" << p.assists << ", Damage: " << p.damage
                     << ", Gold: " << p.gold << ", CS: " << p.cs << "\n";
                cout << "--------------------------------------\n";
            }
            cout << "============================================================\n";
        }
    }

    void displayPlayerStats(const string& playerName) const {
        int totalKills = 0, totalDeaths = 0, totalAssists = 0;
        int totalDamage = 0, totalPlaytime = 0;
        int matchCount = 0;

        for (const auto& match : matches) {
            auto processTeam = [&](const vector<PlayerStats>& team) {
                for (const auto& player : team) {
                    if (player.playerName == playerName) {
                        totalKills += player.kills;
                        totalDeaths += player.deaths;
                        totalAssists += player.assists;
                        totalDamage += player.damage;
                        totalPlaytime += match.playtime;
                        matchCount++;
                    }
                }
            };

            processTeam(match.blueTeam);
            processTeam(match.redTeam);
        }

        if (matchCount == 0) {
            cout << "No stats found for player: " << playerName << "\n";
            return;
        }

        double avgDamagePerMinute = totalPlaytime > 0 ? static_cast<double>(totalDamage) / totalPlaytime : 0.0;
        double avgKDA = totalDeaths == 0 ? totalKills + totalAssists : static_cast<double>(totalKills + totalAssists) / totalDeaths;

        cout << "\n[Player Statistics: " << playerName << "]\n";
        cout << "Matches Played: " << matchCount << "\n";
        cout << "Average Damage per Minute: " << avgDamagePerMinute << "\n";
        cout << "Average KDA: " << avgKDA << "\n";
    }

    void displayChampionWinRate(const string& championName) const {
        int totalGames = 0, totalWins = 0;

        for (const auto& match : matches) {
            auto processTeam = [&](const vector<PlayerStats>& team, bool isBlueWin) {
                for (const auto& player : team) {
                    if (player.champion == championName) {
                        totalGames++;
                        if (isBlueWin) totalWins++;
                    }
                }
            };

            processTeam(match.blueTeam, match.blueTeamWin);
            processTeam(match.redTeam, !match.blueTeamWin);
        }

        if (totalGames == 0) {
            cout << "No stats found for champion: " << championName << "\n";
            return;
        }

        double winRate = static_cast<double>(totalWins) / totalGames * 100.0;
        cout << "\n[Champion Statistics: " << championName << "]\n";
        cout << "Win Rate: " << winRate << "%\n";
    }
};

void displayMenu() {
    cout << "\n========================" << endl;
    cout << "  Match History Menu" << endl;
    cout << "========================" << endl;
    cout << "1. Enter Match Data" << endl;
    cout << "2. View Player Stats" << endl;
    cout << "3. View Champion Win Rate" << endl;
    cout << "4. Display All Matches" << endl;
    cout << "5. Exit" << endl;
    cout << "========================\n" << endl;
    cout << "Choose an option: ";
}

void EnterMatchData(StatsTracker& tracker) {
    string mID;
    cout << "Enter match ID: ";
    cin >> mID;
    cin.ignore();

    int pt;
    cout << "Enter game duration (in minutes): ";
    cin >> pt;
    cin.ignore();

    bool bWin;
    cout << "Did the Blue Team win? (1 for Yes, 0 for No): ";
    cin >> bWin;
    cin.ignore();

    vector<PlayerStats> bTeam, rTeam;

    cout << "Enter data for Blue Team:\n";
    for (int i = 1; i <= 5; ++i) {
        string pName, r, c;
        int k, d, a, dmg, g, cs, wp, wc, vs, lv;

        cout << "Player " << i << " (Name Role Champ Kills Deaths Assists Damage Gold CS WardsPlaced WardsCleared VisionScore Level): ";
        cin >> pName >> r >> c >> k >> d >> a >> dmg >> g >> cs >> wp >> wc >> vs >> lv;

        bTeam.emplace_back(pName, r, c, k, d, a, dmg, g, cs, wp, wc, vs, lv);
    }

    cout << "Enter data for Red Team:\n";
    for (int i = 1; i <= 5; ++i) {
        string pName, r, c;
        int k, d, a, dmg, g, cs, wp, wc, vs, lv;

        cout << "Player " << i << " (Name Role Champ Kills Deaths Assists Damage Gold CS WardsPlaced WardsCleared VisionScore Level): ";
        cin >> pName >> r >> c >> k >> d >> a >> dmg >> g >> cs >> wp >> wc >> vs >> lv;

        rTeam.emplace_back(pName, r, c, k, d, a, dmg, g, cs, wp, wc, vs, lv);
    }

    tracker.addMatch(Match(mID, bTeam, rTeam, pt, bWin));
}

int main() {
    StatsTracker tracker;
    int choice;

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            EnterMatchData(tracker);
            break;
        case 2: {
            string pName;
            cout << "Enter player name: ";
            cin >> pName;
            tracker.displayPlayerStats(pName);
            break;
        }
        case 3: {
            string champName;
            cout << "Enter champion name: ";
            cin >> champName;
            tracker.displayChampionWinRate(champName);
            break;
        }
        case 4:
            tracker.displayAllMatches();
            break;
        case 5:
            cout << "Exiting program. Goodbye!\n";
            break;
        default:
            cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
