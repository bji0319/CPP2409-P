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

    // 생성자
    Match(const string& name, const string& r, const string& c, int k, int d, int a)
        : playerName(name), role(r), champion(c), kills(k), deaths(d), assists(a) {}

    // 데이터를 파일에 저장할 수 있게 문자열로 변환
    string toString() const {
        return playerName + " " + role + " " + champion + " " + 
               to_string(kills) + " " + to_string(deaths) + " " + to_string(assists);
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
            int kills, deaths, assists;

            ss >> playerName >> role >> champion >> kills >> deaths >> assists;
            matches.push_back(Match(playerName, role, champion, kills, deaths, assists));
        }
    }
};

int main() {
    StatsTracker tracker;
    char moreData;

    do {
        // 한 줄로 모든 데이터 입력받기
        string inputLine;
        cout << "Enter match ID (e.g., match1, match2, etc.): ";
        string matchID;
        cin >> matchID;
        cin.ignore();  

        // 10명의 데이터 입력
        for (int i = 1; i <= 10; ++i) {
            string playerName, role, champion;
            int kills, deaths, assists;

            cout << "Enter data for Player " << i << " (format: Name Role Champion Kills Deaths Assists): ";
            getline(cin, inputLine);

            stringstream ss(inputLine);
            ss >> playerName >> role >> champion >> kills >> deaths >> assists;

            // 매치 데이터 추가
            tracker.addMatch(Match(playerName, role, champion, kills, deaths, assists));
        }

        // 새 매치를 더 입력할지 여부 묻기
        cout << "Do you want to enter another match? (y/n): ";
        cin >> moreData;
        cin.ignore();  

    } while (moreData == 'y');

    return 0;
}
