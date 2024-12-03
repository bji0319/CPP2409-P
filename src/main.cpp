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
    const string filename = "match_data.txt"; // 데이터 파일 이름

public:
    StatsTracker() {
        loadMatches(); // 프로그램 시작 시 데이터 파일에서 기록 불러오기
    }

    ~StatsTracker() {
        saveMatches(); // 프로그램 종료 시 데이터 파일에 기록 저장
    }

    // 경기 데이터 추가
    void addMatch(const Match& match) {
        matches.push_back(match);
    }

    // 모든 경기 기록을 파일에 저장
    void saveMatches() const {
        ofstream outFile(filename);
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

        // 파일에서 모든 데이터를 순서대로 읽어옴
        ss >> playerName >> role >> champion >> kills >> deaths >> assists >> damage >> gold 
           >> playtime >> win >> cs >> wardsPlaced >> wardsCleared >> visionScore >> level;

        // Match 객체를 생성하여 벡터에 추가
        matches.push_back(Match(playerName, role, champion, kills, deaths, assists, damage, gold, playtime, win, cs, wardsPlaced, wardsCleared, visionScore, level));
    }
}

   
   // 특정 플레이어의 평균 분당 데미지 계산
    double calculateAverageDamagePerMinute(const string& playerName) {
    double totalDamage = 0;
    int totalPlaytime = 0;  // 총 플레이 시간
    int matchCount = 0;

    for (const auto& match : matches) {
        if (match.playerName == playerName) {
            totalDamage += match.damage;
            totalPlaytime += match.playtime;  // 모든 경기의 총 플레이 시간을 더함
            matchCount++;
        }
    }

    if (matchCount == 0) return 0.0; // 해당 플레이어의 경기 기록이 없으면 0 반환
    return totalPlaytime == 0 ? 0.0 : static_cast<double>(totalDamage) / totalPlaytime; // 분당 데미지 계산
}


    // 특정 플레이어의 평균 KDA 계산
    double calculateAverageKDA(const string& playerName) {
        double totalKDA = 0.0;
        int matchCount = 0;

        for (const auto& match : matches) {
            if (match.playerName == playerName) {
                double kda = match.deaths == 0 ? match.kills + match.assists
                                               : static_cast<double>(match.kills + match.assists) / match.deaths;
                totalKDA += kda;
                matchCount++;
            }
        }

        if (matchCount == 0) return 0.0; // 플레이어 기록이 없으면 0 반환
        return totalKDA / matchCount;
    }

      // 챔피언별 승률 계산
    double calculateWinRateByChampion(const string& championName) {
        int totalMatches = 0;
        int totalWins = 0;

        for (const auto& match : matches) {
            if (match.champion == championName) {
                totalMatches++;
                if (match.win) {
                    totalWins++;
                }
            }
        }

        if (totalMatches == 0) return 0.0; // 해당 챔피언의 경기 기록이 없을 때
        return static_cast<double>(totalWins) / totalMatches * 100.0; // 승률(%)
    }
};

int main() {
    StatsTracker tracker;
    char moreData;

    do {
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

            // 블루 팀 데이터 추가
            tracker.addMatch(Match(playerName, role, champion, kills, deaths, assists, damage, gold, playtime, blueTeamWin, cs, wardsPlaced, wardsCleared, visionScore, level));
        }

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

            // 레드 팀 데이터 추가 (레드팀은 반대 승리 조건)
            tracker.addMatch(Match(playerName, role, champion, kills, deaths, assists, damage, gold, playtime, !blueTeamWin, cs, wardsPlaced, wardsCleared, visionScore, level));
        }

        // 새 매치를 더 입력할지 여부 묻기
        cout << "Do you want to enter another match? (y/n): ";
        cin >> moreData;
        cin.ignore();

    } while (moreData == 'y');

    return 0;
}
