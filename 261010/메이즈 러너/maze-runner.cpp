#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
#include <cmath>

using namespace std;

struct People {
    int id;
    int r;
    int c;
    int move=0;
    bool exited=false;

};

struct Wall {
    int r;
    int c;
    int hp;
    bool broken=false;
};

struct rotateHubo {
    int peopleid;
    int size;
    int l;
    int u;
};

int N, M, K;
int board[10][10];
pair<int, int> direction[] = { {1,0},{-1,0},{0,1},{0,-1} };
vector<People> peoples;
vector<Wall> walls;
pair<int, int> exitRC;
int result = 0;
int exitBoard[10][10]; //출구 위치 저장
int exitCnt = 0;

void movePeoples(void) {
    for (auto &it : peoples) {
        if (it.exited) continue;

        int currdist = abs(it.r - exitRC.first) + abs(it.c - exitRC.second);
        int cr = it.r;
        int cc = it.c;
        for (int i = 0; i < 4; i++) {
            int nr = cr + direction[i].first;
            int nc = cc + direction[i].second;
            if (0 <= nr && nr < N && 0 <= nc && nc < N && board[nr][nc]==0) {
                int nextdist = abs(nr - exitRC.first) + abs(nc - exitRC.second);
                if (nextdist < currdist) {
                    it.r = nr;
                    it.c = nc;
                    result++;
                    break;
                }
            }
        }
        


        if (it.r == exitRC.first && it.c == exitRC.second) {
            it.exited = true;
            exitCnt++;
        }
    }
}

void rotateBoard90(void){
    vector<rotateHubo> hubos;
    for (const auto &it : peoples) {
        if (it.exited) continue;

        rotateHubo temp;
        rotateHubo temphubo[8];
        temp.peopleid = it.id;

        temp.size = max(abs(exitRC.first - it.r), abs(exitRC.second - it.c));
        
        temphubo[0].l = it.c;
        temphubo[0].u = it.r;

        temphubo[1].l = it.c;
        temphubo[1].u = max(0,it.r - temp.size);

        temphubo[2].l = max(0,it.c - temp.size);
        temphubo[2].u = max(0,it.r - temp.size);

        temphubo[3].l = max(0,it.c - temp.size);
        temphubo[3].u = it.r;

        temphubo[4].l = exitRC.second;
        temphubo[4].u = exitRC.first;

        temphubo[5].l = exitRC.second;
        temphubo[5].u = max(0,exitRC.first - temp.size);

        temphubo[6].l = max(0,exitRC.second - temp.size);
        temphubo[6].u = max(0,exitRC.first - temp.size);
        
        temphubo[7].l = max(0,exitRC.second - temp.size);
        temphubo[7].u = exitRC.first;

        sort(&temphubo[0], &temphubo[0] + 8, [](const rotateHubo &a , const rotateHubo &b) {
            if (a.u == b.u) {
                return a.l < b.l;
            }
            return a.u < b.u;
        });

        for (const auto &tempit : temphubo) {
            if (tempit.l <= it.c && it.c <= tempit.l + temp.size &&
                tempit.l <= exitRC.second && exitRC.second <= tempit.l + temp.size &&
                tempit.u <= it.r && it.r <= tempit.u + temp.size &&
                tempit.u <= exitRC.first && exitRC.first <= tempit.u + temp.size) {
                temp.l = tempit.l;
                temp.u = tempit.u;
                break;
            }
        }

        hubos.push_back(temp);
        
    }
    sort(hubos.begin(), hubos.end(), [](const rotateHubo &a, const rotateHubo &b) {
        if (a.size == b.size) {
            if (a.u == b.u) {
                return a.l < b.l;
            }
            return a.u < b.u;
        }
        return a.size < b.size;
    });

    int tempBoard[10][10];
    //int tempPeopleBoard[10][10];
    int tempExitBoard[10][10];

    rotateHubo &target = hubos[0];
    for (int i = 0; i <= target.size; i++) {
        for (int j = 0; j <= target.size; j++) {
            tempBoard[i][j] = board[target.u + i][target.l + j];
            tempExitBoard[i][j] = exitBoard[target.u + i][target.l + j];
        }
    }

    for (int i = 0; i <= target.size; i++) {
        for (int j = 0; j <= target.size; j++) {
            board[target.u + i][target.l + j] = tempBoard[target.size - j][i];
            exitBoard[target.u + i][target.l + j] = tempExitBoard[target.size - j][i];

            if (board[target.u + i][target.l + j] > 0) board[target.u + i][target.l + j]--;
            if (exitBoard[target.u + i][target.l + j] > 0) {
                exitRC.first = target.u + i;
                exitRC.second = target.l + j;
            }
        }
    }
    for (auto &it : peoples) {
        if (it.exited) continue;

        if (target.l <= it.c && it.c <= target.l + target.size &&
            target.u <= it.r && it.r <= target.u + target.size) {

            int cr = it.r - target.u;
            int cc = it.c - target.l;

            it.r = target.u + cc;
            it.c = target.l + target.size - cr;
        }
    }
    
}


int main(void) {
    cin >> N >> M >> K;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> board[i][j];
            exitBoard[i][j] = 0;
        }
    }
    
    for (int i = 0; i < M; i++) {
        People temp;
        cin >> temp.r >> temp.c;
        temp.r -= 1;
        temp.c -= 1;
        temp.id = peoples.size();
        peoples.push_back(temp);
    }
    cin >> exitRC.first >> exitRC.second;
    exitRC.first -= 1;
    exitRC.second -= 1;
    exitBoard[exitRC.first][exitRC.second] = 1;

    for (int i = 0; i < K; i++) {
         movePeoples();
        if (exitCnt == peoples.size()) break;
        rotateBoard90();
    }
    
    cout << result << endl;
    cout << exitRC.first + 1 << " " << exitRC.second + 1;
}