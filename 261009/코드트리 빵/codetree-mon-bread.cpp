#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>

using namespace std;

struct People {
    int r;
    int c;
    int targetR;
    int targetC;
    bool arrive = false;
    bool moveStart = false;
};

int N, M;
int board[15][15]; //0은 이동 가능, 1은 베이스캠프 이용전, 2는 이용전 편의점, 11은 이용중 베캠, 12는 이용 편의점
pair<int, int> directions[] = { {-1,0},{0,-1},{0,1},{1,0} };
vector<pair<int, int>> basecamps;
vector<pair<int, int>> convinis;
vector<People> peoples;

int arrivePeople;
int currTime;


void movePeople(int t) {//동시에 움직이는거임
    int movec = min(t, M);
    for (int i = 0; i < movec; i++) {
        if (peoples[i].arrive) continue;

        pair<int, int> backtrack[15][15];
        fill(&backtrack[0][0], &backtrack[0][0] + (15 * 15), pair<int, int>{-1, -1});
        bool visited[15][15]={0};
        queue<pair<int, int>> q;
        q.push({ peoples[i].r, peoples[i].c });

        visited[peoples[i].r][peoples[i].c] = true;

        while (!q.empty()) {
            pair<int, int> curr = q.front();
            q.pop();

            if (curr.first == peoples[i].targetR && curr.second == peoples[i].targetC) {
                break;
            }

            for (int j = 0; j < 4; j++) {
                int nr = curr.first + directions[j].first;
                int nc = curr.second + directions[j].second;
                if (0 <= nr && nr < N && 0 <= nc && nc < N && !visited[nr][nc] && board[nr][nc] <= 2) {
                    visited[nr][nc] = true;
                    q.push({ nr,nc });
                    backtrack[nr][nc] = { curr.first,curr.second };
                }
            }
        }


        int cr=peoples[i].targetR;
        int cc=peoples[i].targetC;

        if (backtrack[cr][cc] == pair<int, int>{ -1,-1 }) continue;

        while (!(backtrack[cr][cc].first == peoples[i].r && backtrack[cr][cc].second == peoples[i].c))
        {
            int nr = backtrack[cr][cc].first;
            int nc = backtrack[cr][cc].second;
            cr = nr;
            cc = nc;
        }

        peoples[i].r = cr;
        peoples[i].c = cc;

    }
}

void finishMove(int t) {
    int movec = min(t, M);
    for (int i = 0; i < movec; i++) {
        auto &curr = peoples[i];
        if (curr.arrive) continue;
        if (curr.r == curr.targetR && curr.c == curr.targetC) {
            curr.arrive = true;
            board[curr.r][curr.c] += 10;
            arrivePeople++;
        }
    }
}

void selectBase(int t) {
    int movec = min(t + 1, M);
    
    for (int j = 0; j < movec; j++) {
        if (peoples[j].moveStart) continue;
        auto &people = peoples[j];

        vector<pair<int,pair<int,int>>> hubos;

        queue<pair<int,pair<int, int>>> q;
        q.push({ 0,{ people.targetR,people.targetC } });
        bool visited[15][15] = { 0 };
        visited[people.targetR][people.targetC] = true;

        while (!q.empty())
        {
            pair<int, pair<int, int>> curr = q.front();
            q.pop();
            for (int i = 0; i < 4; i++) {
                int nr = curr.second.first + directions[i].first;
                int nc = curr.second.second + directions[i].second;
                if (0 <= nr && nr < N && 0 <= nc && nc < N && !visited[nr][nc] && board[nr][nc] <= 2) {
                    visited[nr][nc] = true;
                    q.push({ curr.first + 1,{nr,nc} });
                    if (board[nr][nc] == 1) {
                        hubos.push_back({ curr.first + 1,{nr,nc} });
                    }
                }
            }
        }
        

        sort(hubos.begin(), hubos.end(), [](const pair<int, pair<int, int>> &a, const pair<int, pair<int, int>> &b) {
            if (a.first == b.first) {
                if (a.second.first == b.second.first) {
                    return a.second.second < b.second.second;
                }
                return a.second.first < b.second.first;
            }
            return a.first < b.first;
        });


        for (auto &it : hubos) {
            
            if (board[it.second.first][it.second.second] > 10) {
                continue;
            }
            board[it.second.first][it.second.second] += 10;
            peoples[j].r = it.second.first;
            peoples[j].c = it.second.second;
            peoples[j].moveStart = true;
            break;
        }

        
    }
}

void simulation() {
    arrivePeople = 0;
    currTime = 0;
    while (arrivePeople != M) {
        movePeople(currTime);
        finishMove(currTime);
        selectBase(currTime);
        

        currTime++;
    }
}

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> N >> M;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> board[i][j];
            if (board[i][j] == 1) {
                basecamps.push_back({ i,j });
            }
        }
    }
    for (int i = 0; i < M; i++) {
        int r, c;
        cin >> r >> c;
        r -= 1;
        c -= 1;
        board[r][c] = 2;

        People temp;
        temp.targetR = r;
        temp.targetC = c;
        peoples.push_back(temp);

    }
    simulation();

    cout << currTime << endl;

}