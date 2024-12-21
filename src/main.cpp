#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <map>
#include <iomanip> // std::setw 등을 사용하기 위함
#include <limits>  // numeric_limits

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
                cout << "  Stats: " << p.kills << "/" << p.deaths << "/" << p.assists
                     << ", Damage: " << p.damage
                     << ", Gold: " << p.gold << ", CS: " << p.cs << "\n";
                cout << "--------------------------------------\n";
            }

            cout << "\nRed Team:\n";
            for (const auto& p : match.redTeam) {
                cout << "  Player: " << p.playerName << ", Role: " << p.role << ", Champion: " << p.champion << "\n";
                cout << "  Stats: " << p.kills << "/" << p.deaths << "/" << p.assists
                     << ", Damage: " << p.damage
                     << ", Gold: " << p.gold << ", CS: " << p.cs << "\n";
                cout << "--------------------------------------\n";
            }
            cout << "============================================================\n";
        }
    }

    void displayPlayerStats(const string& playerName) const {
    // 누적 스탯 구조체 (랭킹에서 썼던 것과 유사)
    struct AccumulatedStats {
        int kills = 0;
        int deaths = 0;
        int assists = 0;
        int damage = 0;
        int cs = 0;
        int gold = 0;
        int visionScore = 0;
        int playtime = 0;       // 전체 플레이 시간(분)
        int matchesPlayed = 0;  // 경기 수
        int totalTeamKills = 0; // 킬 관여율 계산을 위해 소속 팀의 총 킬
    };
    
    AccumulatedStats st;  // 플레이어 하나에 대한 누적 스탯

    // 모든 매치를 순회하며, 해당 플레이어의 스탯을 누적
    for (const auto& match : matches) {
        // 팀 킬 계산
        int blueTeamKills = 0;
        for (const auto& p : match.blueTeam) {
            blueTeamKills += p.kills;
        }
        int redTeamKills = 0;
        for (const auto& p : match.redTeam) {
            redTeamKills += p.kills;
        }

        // 블루 팀에 해당 플레이어가 있는지 체크
        bool foundOnBlue = false;
        for (const auto& player : match.blueTeam) {
            if (player.playerName == playerName) {
                foundOnBlue = true;
                // 누적
                st.kills += player.kills;
                st.deaths += player.deaths;
                st.assists += player.assists;
                st.damage += player.damage;
                st.cs += player.cs;
                st.gold += player.gold;
                st.visionScore += player.visionScore;
                st.playtime += match.playtime;
                st.matchesPlayed += 1;
                // 해당 경기 팀 킬
                st.totalTeamKills += blueTeamKills; 
                break; // 이미 찾았으면 빠져나와도 됨
            }
        }

        // 레드 팀에 해당 플레이어가 있는지 체크
        if (!foundOnBlue) {
            for (const auto& player : match.redTeam) {
                if (player.playerName == playerName) {
                    st.kills += player.kills;
                    st.deaths += player.deaths;
                    st.assists += player.assists;
                    st.damage += player.damage;
                    st.cs += player.cs;
                    st.gold += player.gold;
                    st.visionScore += player.visionScore;
                    st.playtime += match.playtime;
                    st.matchesPlayed += 1;
                    // 해당 경기 팀 킬
                    st.totalTeamKills += redTeamKills;
                    break;
                }
            }
        }
    }

    // 플레이어가 아예 없었다면 matchCount==0
    if (st.matchesPlayed == 0) {
        cout << "No stats found for player: " << playerName << "\n";
        return;
    }

    // 이제 누적된 스탯 기반으로 평균 지표 계산
    // (분당 데미지, 분당 CS, 분당 골드, 분당 시야점수, KDA, 킬관여율 등)
    double avgDamagePerMinute   = (st.playtime > 0) ? static_cast<double>(st.damage) / st.playtime : 0.0;
    double avgCSPerMinute       = (st.playtime > 0) ? static_cast<double>(st.cs)     / st.playtime : 0.0;
    double avgGoldPerMinute     = (st.playtime > 0) ? static_cast<double>(st.gold)   / st.playtime : 0.0;
    double avgVisionScorePerMin = (st.playtime > 0) ? static_cast<double>(st.visionScore) / st.playtime : 0.0;

    // KDA
    double avgKDA = (st.deaths == 0)
        ? (st.kills + st.assists)
        : static_cast<double>(st.kills + st.assists) / st.deaths;

    // Kill Participation
    double killParticipation = 0.0;
    if (st.totalTeamKills > 0) {
        killParticipation = (static_cast<double>(st.kills + st.assists) / st.totalTeamKills) * 100.0;
    }

    // 출력
    cout << "\n[Player Statistics: " << playerName << "]\n";
    cout << "Matches Played: " << st.matchesPlayed << "\n";
    cout << "Total Kills/Deaths/Assists: " << st.kills << "/" << st.deaths << "/" << st.assists << "\n";
    cout << "Total Damage: " << st.damage << "\n";
    cout << "Total Gold: " << st.gold << "\n";
    cout << "Total CS: " << st.cs << "\n";
    cout << "Total Vision Score: " << st.visionScore << "\n";
    cout << "Total Playtime: " << st.playtime << " minutes\n";
    cout << "\n-- Averages --\n";
    cout << "Avg Damage/Min: " << avgDamagePerMinute << "\n";
    cout << "Avg CS/Min: " << avgCSPerMinute << "\n";
    cout << "Avg Gold/Min: " << avgGoldPerMinute << "\n";
    cout << "Avg VisionScore/Min: " << avgVisionScorePerMin << "\n";
    cout << "Average KDA: " << avgKDA << "\n";
    cout << "Kill Participation: " << killParticipation << "%\n";
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

    
    void displayRanking() const {
        if (matches.empty()) {
            cout << "No matches recorded.\n";
            return;
        }

        // 순위를 매길 지표를 선택
        cout << "\n[Ranking Menu]\n";
        cout << "1. Total Kills\n";
        cout << "2. Total Damage\n";
        cout << "3. Average Damage per Minute\n";
        cout << "4. Average KDA\n";
        cout << "5. Average CS per Minute\n";
        cout << "6. Average Gold per Minute\n";
        cout << "7. Average Vision Score per Minute\n";
        cout << "8. Kill Participation (%)\n";
        cout << "Choose a metric for ranking: ";
        int metricChoice;
        cin >> metricChoice;

        // 누적 스탯 구조체
        struct AccumulatedStats {
            int kills = 0;
            int deaths = 0;
            int assists = 0;
            int damage = 0;
            int cs = 0;
            int gold = 0;
            int visionScore = 0;
            int playtime = 0;
            int matchesPlayed = 0;
            // 킬 관여율 계산을 위해 팀 킬을 누적
            int totalTeamKills = 0;
        };

        map<string, AccumulatedStats> statMap; // <플레이어명, 누적스탯>

        // 모든 Match를 순회하며 정보를 수집
        for (const auto& match : matches) {
            int blueTeamKills = 0;
            for (auto &p : match.blueTeam) {
                blueTeamKills += p.kills;
            }
            int redTeamKills = 0;
            for (auto &p : match.redTeam) {
                redTeamKills += p.kills;
            }

            // Blue Team 누적
            for (const auto& player : match.blueTeam) {
                auto& entry = statMap[player.playerName];
                entry.kills       += player.kills;
                entry.deaths      += player.deaths;
                entry.assists     += player.assists;
                entry.damage      += player.damage;
                entry.cs          += player.cs;
                entry.gold        += player.gold;
                entry.visionScore += player.visionScore;
                entry.playtime    += match.playtime;
                entry.matchesPlayed += 1;
                entry.totalTeamKills += blueTeamKills;
            }

            // Red Team 누적
            for (const auto& player : match.redTeam) {
                auto& entry = statMap[player.playerName];
                entry.kills       += player.kills;
                entry.deaths      += player.deaths;
                entry.assists     += player.assists;
                entry.damage      += player.damage;
                entry.cs          += player.cs;
                entry.gold        += player.gold;
                entry.visionScore += player.visionScore;
                entry.playtime    += match.playtime;
                entry.matchesPlayed += 1;
                entry.totalTeamKills += redTeamKills;
            }
        }

       
        vector<pair<string, AccumulatedStats>> playerStatsVec(statMap.begin(), statMap.end());

        auto getValueForMetric = [&](const AccumulatedStats& st) {
            switch (metricChoice) {
            case 1: // Total Kills
                return static_cast<double>(st.kills);
            case 2: // Total Damage
                return static_cast<double>(st.damage);
            case 3: // Average Damage per Minute
                if (st.playtime == 0) return 0.0;
                return static_cast<double>(st.damage) / st.playtime;
            case 4: // Average KDA
                if (st.deaths == 0) {
                    return static_cast<double>(st.kills + st.assists);
                }
                return static_cast<double>(st.kills + st.assists) / st.deaths;
            case 5: // Average CS per Minute
                if (st.playtime == 0) return 0.0;
                return static_cast<double>(st.cs) / st.playtime;
            case 6: // Average Gold per Minute
                if (st.playtime == 0) return 0.0;
                return static_cast<double>(st.gold) / st.playtime;
            case 7: // Average Vision Score per Minute
                if (st.playtime == 0) return 0.0;
                return static_cast<double>(st.visionScore) / st.playtime;
            case 8: // Kill Participation (%)
                if (st.totalTeamKills == 0) return 0.0;
                return (static_cast<double>(st.kills + st.assists) / st.totalTeamKills) * 100.0;
            default: // 잘못된 입력 시 kills 기준
                return static_cast<double>(st.kills);
            }
        };

        // 내림차순 정렬(높은 값이 1등)
        sort(playerStatsVec.begin(), playerStatsVec.end(),
             [&](auto& a, auto& b) {
                 return getValueForMetric(a.second) > getValueForMetric(b.second);
             });

        // 출력
        cout << "\n[Player Ranking]\n";
        cout << left << setw(5) << "Rank"
             << left << setw(15) << "Player"
             << left << setw(10) << "Matches"
             << left << setw(15) << "MetricValue"
             << endl;
        cout << "-----------------------------------------------------------\n";

        int rank = 1;
        for (auto& [playerName, st] : playerStatsVec) {
            double val = getValueForMetric(st);
            cout << left << setw(5) << rank
                 << left << setw(15) << playerName
                 << left << setw(10) << st.matchesPlayed
                 << left << setw(15) << val
                 << endl;
            rank++;
        }
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
    cout << "6. Display Ranking" << endl;
    cout << "========================\n" << endl;
    cout << "Choose an option: ";
}

void EnterMatchData(StatsTracker& tracker) {
    // match ID (공백 없이 받는 예시)
    cout << "Enter match ID: ";
    string mID;
    cin >> mID;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter game duration (in minutes): ";
    int pt;
    cin >> pt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Did the Blue Team win? (1 for Yes, 0 for No): ";
    bool bWin;
    cin >> bWin;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<PlayerStats> bTeam, rTeam;

    cout << "Enter data for Blue Team:\n";
    for (int i = 1; i <= 5; ++i) {
        string pName, r, c;
        int k, d, a, dmg, g, cs, wp, wc, vs, lv;

        cout << "Player " << i
             << " (Name Role Champ Kills Deaths Assists Damage Gold CS WardsPlaced WardsCleared VisionScore Level): ";
        cin >> pName >> r >> c >> k >> d >> a >> dmg >> g >> cs >> wp >> wc >> vs >> lv;
        bTeam.emplace_back(pName, r, c, k, d, a, dmg, g, cs, wp, wc, vs, lv);
    }

    cout << "Enter data for Red Team:\n";
    for (int i = 1; i <= 5; ++i) {
        string pName, r, c;
        int k, d, a, dmg, g, cs, wp, wc, vs, lv;

        cout << "Player " << i
             << " (Name Role Champ Kills Deaths Assists Damage Gold CS WardsPlaced WardsCleared VisionScore Level): ";
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
        // 메뉴 숫자 받기
        cin >> choice;
        // 버퍼 비우기 (개행문자 등 처리)
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            EnterMatchData(tracker);
            break;
        case 2: {
            // 플레이어 이름(공백 포함 가능)을 받기
            cout << "Enter player name: ";
            string pName;
            getline(cin, pName);
            tracker.displayPlayerStats(pName);
            break;
        }
        case 3: {
            // 챔피언 이름(공백 포함 가능)을 받기
            cout << "Enter champion name: ";
            string champName;
            getline(cin, champName);
            tracker.displayChampionWinRate(champName);
            break;
        }
        case 4:
            tracker.displayAllMatches();
            break;
        case 5:
            cout << "Exiting program. Goodbye!\n";
            break;
        case 6:
            tracker.displayRanking();
            break;
        default:
            cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
