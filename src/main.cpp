#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream> // 파일 입출력을 위한 라이브러리

using namespace std;

class Match {
public:
    string playerName;
    string role;
    string champion;
    int kills;
    int deaths;
    int assists;
    int damage;
    int gold;
    int playtime;
    bool win;
    int cs;             
    int wardsPlaced;    
    int wardsCleared;   
    int visionScore;    
    int level;          

    Match(const string& name, const string& r, const string& c, int k, int d, int a, int dmg, int g, int pt, bool w, int cs, int wp, int wc, int vs, int lvl)
        : playerName(name), role(r), champion(c), kills(k), deaths(d), assists(a), damage(dmg), gold(g),
          playtime(pt), win(w), cs(cs), wardsPlaced(wp), wardsCleared(wc), visionScore(vs), level(lvl) {}

    // 데이터를 파일에 저장할 수 있게 문자열로 변환
    string toString() const {
        return playerName + " " + role + " " + champion + " " + 
               to_string(kills) + " " + to_string(deaths) + " " +
               to_string(assists) + " " + to_string(damage) + " " + to_string(gold) + " " +
               to_string(playtime) + " " + (win ? "1" : "0") + " " +
               to_string(cs) + " " + to_string(wardsPlaced) + " " +
               to_string(wardsCleared) + " " + to_string(visionScore) + " " + to_string(level);
    }
};

class StatsTracker {
private:
    vector<Match> matches;
    const string filename = "match_data.txt";

public:
    StatsTracker() {
        loadMatches();
    }

    ~StatsTracker() {
        saveMatches();
    }

    // 경기 데이터 추가
    void addMatch(const Match& match) {
        matches.push_back(match);
    }

    // 모든 경기 기록을 파일에 저장
    void saveMatches() const {
        ofstream outFile(filename, ios::app); // Append mode
        for (const auto& match : matches) {
            outFile << match.toString() << endl;
        }
    }

    // 파일에서 기존 경기 기록을 불러오기
    void loadMatches() {
        ifstream inFile(filename);
        string line;

        while (getline(inFile, line)) {
            stringstream ss(line);
            string playerName, role, champion;
            int kills, deaths, assists, damage, gold, playtime, cs, wardsPlaced, wardsCleared, visionScore, level;
            bool win;

            ss >> playerName >> role >> champion >> kills >> deaths >> assists >> damage >> gold 
               >> playtime >> win >> cs >> wardsPlaced >> wardsCleared >> visionScore >> level;

            matches.push_back(Match(playerName, role, champion, kills, deaths, assists, damage, gold, playtime, win, cs, wardsPlaced, wardsCleared, visionScore, level));
        }
    }

    void displayAllMatches() const {
        for (const auto& match : matches) {
            cout << match.toString() << endl;
        }
    }

    void displayPlayerStats(const string& playerName) const {
        // 구현 필요
        cout << "[Feature under development] Displaying stats for player: " << playerName << endl;
    }
};

void showMenu() {
    cout << "\n===== Match Tracker Menu =====" << endl;
    cout << "1. Enter Match Data" << endl;
    cout << "2. View Player Stats" << endl;
    cout << "3. Exit Program" << endl;
    cout << "=============================" << endl;
    cout << "Select an option: ";
}

int main() {
    StatsTracker tracker;
    bool running = true;

    while (running) {
        showMenu();
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: {
            string matchID;
            cout << "Enter match ID (e.g., match1, match2, etc.): ";
            cin >> matchID;
            cin.ignore();

            int playtime;
            cout << "Enter game duration (in minutes): ";
            cin >> playtime;
            cin.ignore();

            bool blueTeamWin;
            cout << "Did the Blue Team win? (1 for win, 0 for loss): ";
            cin >> blueTeamWin;
            cin.ignore();

        cout << "Entering data for Blue Team:\n";
        cout << "Entering data for Blue Team:\n";
        // 블루팀 데이터 입력
            cout << "Entering data for Blue Team:\n";
        // 블루팀 데이터 입력
            for (int i = 1; i <= 5; ++i) {
                string inputLine, playerName, role, champion;
                int kills, deaths, assists, damage, gold, cs, wardsPlaced, wardsCleared, visionScore, level;

                cout << "Enter data for Blue Team Player " << i 
                     << " (format: Name Role Champion Kills Deaths Assists Damage Gold CS WardsPlaced WardsCleared VisionScore Level): ";
                getline(cin, inputLine);

                stringstream ss(inputLine);
                ss >> playerName >> role >> champion >> kills >> deaths >> assists >> damage >> gold 
                   >> cs >> wardsPlaced >> wardsCleared >> visionScore >> level;

                tracker.addMatch(Match(playerName, role, champion, kills, deaths, assists, damage, gold, playtime, blueTeamWin, cs, wardsPlaced, wardsCleared, visionScore, level));
            }

        cout << "Entering data for Red Team:\n";
        cout << "Entering data for Red Team:\n";
        // 레드팀 데이터 입력
            cout << "Entering data for Red Team:\n";
        // 레드팀 데이터 입력
            for (int i = 1; i <= 5; ++i) {
                string inputLine, playerName, role, champion;
                int kills, deaths, assists, damage, gold, cs, wardsPlaced, wardsCleared, visionScore, level;

                cout << "Enter data for Red Team Player " << i 
                     << " (format: Name Role Champion Kills Deaths Assists Damage Gold CS WardsPlaced WardsCleared VisionScore Level): ";
                getline(cin, inputLine);

                stringstream ss(inputLine);
                ss >> playerName >> role >> champion >> kills >> deaths >> assists >> damage >> gold 
                   >> cs >> wardsPlaced >> wardsCleared >> visionScore >> level;

                tracker.addMatch(Match(playerName, role, champion, kills, deaths, assists, damage, gold, playtime, !blueTeamWin, cs, wardsPlaced, wardsCleared, visionScore, level));
            }
            break;
        }
        case 2: {
            string playerName;
            cout << "Enter player name to view stats: ";
            cin >> playerName;
            tracker.displayPlayerStats(playerName);
            break;
        }
        case 3: {
            cout << "Exiting program. Goodbye!" << endl;
            running = false;
            break;
        }
        default:
            cout << "Invalid choice. Please select a valid option." << endl;
        }
    }

    return 0;
}
