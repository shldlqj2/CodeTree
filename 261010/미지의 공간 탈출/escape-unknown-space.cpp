#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>

using namespace std;

int N, M, F;

struct TimeHole {
    int r;
    int c;
    int direction;
    int moveTime;
};

struct Timewall {
    int l=2e9;
    int d=-2e9;
    int r=-2e9;
    int u=2e9;
    int h;
    int w;
};

int board[20][20];//0 빈공간 1 장애물 3시간의벽 4탈출구 5시간구멍
pair<int, int>directions[] = { {0,1},{0,-1},{1,0},{-1,0} };
int mboard[5][10][10];
pair<int, int> exitRC;
pair<int, int> exitTwRC;
pair<int, int> startRC;
pair<int, int> startBoardRC;
vector<TimeHole> timeholes;
vector<TimeHole> twTimeHoles;
Timewall tw;
int mboardOpen[32][32];
int T;
bool canExit = true;


void rotateBoard(int num,int times) {
    for (int i = 0; i < times; i++) {
        int tempBoard[10][10];
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < M; j++) {
                tempBoard[i][j] = mboard[num][i][j];
            }
        }

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < M; j++) {
                mboard[num][i][j] = tempBoard[M - 1 - j][i];
            }
        }
    }
}

void makeLeft() {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            mboardOpen[1 + tw.h + i][1 + j] = mboard[1][i][j];
        }
    }
    for (int j = 0; j < M; j++) {
        mboardOpen[tw.h][1 + j] = 10;
        mboardOpen[tw.h + 1 + M][1 + j] = 10;
    }
    for (int i = 0; i < M; i++) {
        mboardOpen[tw.h + 1 + i][0] = board[tw.u + i][tw.l - 1];
        if (mboardOpen[tw.h + 1 + i][0] == 0) exitTwRC = { tw.h + 1 + i,0 };
    }
}
void makeRight() {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            mboardOpen[1 + tw.h + i][(1 + (2*tw.w)) + j] = mboard[0][i][j];
        }
    }
    for (int j = 0; j < M; j++) {
        mboardOpen[tw.h][1 + 2*tw.w + j] = 10;
        mboardOpen[tw.h + 1 + M][1 + 2*tw.w + j] = 10;
    }
    for (int i = 0; i < M; i++) {
        mboardOpen[tw.h + 1 + i][1 + 2*tw.w + M] = board[tw.u + i][tw.r + 1];
        if (mboardOpen[tw.h + 1 + i][1 + 2 * tw.w + M] == 0) exitTwRC = { tw.h + 1 + i,1 + 2 * tw.w + M };
    }
}
void makeUp() {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            mboardOpen[1 + i][1 + tw.w + j] = mboard[3][i][j];
        }
    }
    for (int i = 0; i < M; i++) {
        mboardOpen[1 + i][tw.w] = 10;
        mboardOpen[1 + i][1 + M + tw.w] = 10;
    }
    for (int i = 0; i < M; i++) {
        mboardOpen[0][1 + tw.w + i] = board[tw.u - 1][tw.l + i];
        if (mboardOpen[0][1 + tw.w + i] == 0) exitTwRC = { 0,1 + tw.w + i };
    }
}
void makeDown() {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            mboardOpen[1 + (2 * tw.h) + i][1 + tw.w + j] = mboard[2][i][j];
        }
    }
    for (int i = 0; i < M; i++) {
        mboardOpen[1 + (2*tw.h)+i][tw.w] = 10;
        mboardOpen[1 + (2*tw.h)+i][1 + M + tw.w] = 10;
    }
    for (int i = 0; i < M; i++) {
        mboardOpen[1+ 2*tw.h + M][1 + tw.w + i] = board[tw.d + 1][tw.l + i];
        if (mboardOpen[1 + 2 * tw.h + M][1 + tw.w + i] == 0) exitTwRC = { 1 + 2 * tw.h + M,1 + tw.w + i };
    }
}
void makeMiddle() {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            mboardOpen[1 + tw.h + i][1 + tw.w + j] = mboard[4][i][j];
        }
    }
    
}

void makeMap() {
    rotateBoard(0, 3);
    rotateBoard(1, 1);
    rotateBoard(3, 2);

    makeLeft();
    makeRight();
    makeUp();
    makeDown();
    makeMiddle();
}
void expandTH(int time) {
    if (time == 0) return;
    for (auto &it : timeholes) {
        if (time % it.moveTime == 0) {
            int nr = it.r + directions[it.direction].first;
            int nc = it.c + directions[it.direction].second;
            it.r = nr;
            it.c = nc;
            if (0 <= nr && nr < N && 0 <= nc && nc < N && board[nr][nc] == 0) board[nr][nc] = 5;
        }
    }
}

bool exitTW(int time) {

    queue<pair<int,pair<int, int>>> q;
    q.push({ time,startRC });
    bool visited[32][32] = { 0 };
    pair<int, int> backtrack[32][32];
    fill(&backtrack[0][0], &backtrack[0][0] + (32 * 32), pair<int, int>{-1, -1});
    visited[startRC.first][startRC.second] = true;
    while (!q.empty()) {
        pair<int,pair<int, int>> curr = q.front();
        q.pop();
        int cr = curr.second.first;
        int cc = curr.second.second;

        if (cr == 0 || cr == 3 * tw.h + 1 || cc == 0 || cc == 3 * tw.w + 1) {
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nr = cr + directions[i].first;
            int nc = cc + directions[i].second;
            if (0 <= nr && nc < 3 * tw.h + 2 && 0 <= nc && nc < 3 * tw.w + 2 && !visited[nr][nc]) {
                if (mboardOpen[nr][nc] == 0) {
                    visited[nr][nc] = true;
                    q.push({ curr.first + 1,{ nr,nc } });
                    backtrack[nr][nc] = { cr,cc };
                }
                else if (mboardOpen[nr][nc] == 10) {
                    if (cr < 1 + tw.h) {
                        int offset = 1 + tw.h - cr;
                        if (i == 0) {
                            nr = cr + offset;
                            nc = cc + offset;
                        }
                        else {
                            nr = cr + offset;
                            nc = cc - offset;
                        }
                    }
                    else if (cr >= 1 + 2 * tw.h) {
                        int offset = cr - (1 + 2 * tw.h) + 1;
                        if (i == 0) {
                            nr = cr - offset;
                            nc = cc + offset;
                        }
                        else {
                            nr = cr - offset;
                            nc = cc - offset;
                        }
                    }
                    else if (cc < 1 + tw.w) {
                        int offset = 1 + tw.w - cc;
                        if (i == 3) {
                            nr = cr - offset;
                            nc = cc + offset;
                        }
                        else {
                            nr = cr + offset;
                            nc = cc + offset;
                        }
                    }
                    else if (cc >= 1 + 2 * tw.w) {
                        int offset = cc - (1 + 2 * tw.w) + 1;
                        if (i == 3) {
                            nr = cr - offset;
                            nc = cc - offset;
                        }
                        else {
                            nr = cr + offset;
                            nc = cc - offset;
                        }
                    }
                    if (0 <= nr && nc < 3 * tw.h + 2 && 0 <= nc && nc < 3 * tw.w + 2 && 
                        !visited[nr][nc] && mboardOpen[nr][nc] == 0) {
                        visited[nr][nc] = true;
                        q.push({ curr.first + 1,{ nr,nc } });
                        backtrack[nr][nc] = { cr,cc };
                    }
                    
                }
            }
        }
    }

    int cr = exitTwRC.first;
    int cc = exitTwRC.second;

    while (backtrack[cr][cc] != startRC) {
        int nr = backtrack[cr][cc].first;
        int nc = backtrack[cr][cc].second;
        cr = nr;
        cc = nc;
    }

    startRC = { cr,cc };
    if (startRC == exitTwRC) return true;
    
    return false;
}

void exitMap(int time) {
    if (board[startBoardRC.first][startBoardRC.second] != 0) {
        T = -1;
        return;
    }
    queue<pair<int,pair<int, int>>> q;
    q.push({ time,startBoardRC });
    bool visited[20][20] = { 0 };
    visited[startBoardRC.first][startBoardRC.second] = true;
    pair<int, int> backtrack[20][20];
    fill(&backtrack[0][0], &backtrack[0][0] + (20 * 20), pair<int, int>{-1, -1});
    while (!q.empty()) {
        pair<int,pair<int, int>> curr = q.front();
        q.pop();
        if (curr.second.first == exitRC.first && curr.second.second == exitRC.second) {
            T = curr.first;
            break;
        }
        for (int i = 0; i < 4; i++) {
            int nr = curr.second.first + directions[i].first;
            int nc = curr.second.second + directions[i].second;
            if (0 <= nr && nr < N && 0 <= nc && nc < N && !visited[nr][nc] &&
                (board[nr][nc] == 0 || board[nr][nc] == 4)) {
                bool skipflag = false;
                for (const auto &it : timeholes) {
                    if (skipflag) break;

                    int move = (curr.first + 1) / it.moveTime;
                    int mr = it.r;
                    int mc = it.c;
                    for (int i = 1; i <= move; i++) {
                        int mr2 = it.r + i * (directions[it.direction].first);
                        int mc2 = it.c + i * (directions[it.direction].second);
                        if (board[mr2][mc2] == 1 || board[mr2][mc2] == 4) {
                            break;
                        }
                        mr = mr2;
                        mc = mc2;
                    }
                    switch (it.direction)
                    {
                    case 0:
                        if (it.c <= nc && nc <= mc && nr == it.r) {
                            skipflag = true;
                        }
                        break;
                    case 1:
                        if (mc <= nc && nc <= it.c && nr == it.r) {
                            skipflag = true;
                        }
                        break;
                    case 2:
                        if (it.r <= nr && nr <= mr && nc == it.c) {
                            skipflag = true;
                        }
                        break;
                    case 3:
                        if (mr <= nr && nr <= it.r && nc == it.c) {
                            skipflag = true;
                        }
                        break;

                    default:
                        break;
                    }
                }
                if (skipflag) continue;
                visited[nr][nc] = true;
                backtrack[nr][nc] = { curr.second.first,curr.second.second };
                q.push({ curr.first + 1,{nr,nc} });
            }
        }
    }

    if (backtrack[exitRC.first][exitRC.second] == pair<int, int>{-1, -1}) {
        T = -1;
        return;
    }
    
}

int main(void) {
    cin >> N >> M >> F;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> board[i][j];
            if (board[i][j] == 4) {
                exitRC = { i,j };
            }
            else if (board[i][j] == 3) {
                tw.l = min(tw.l, j);
                tw.d = max(tw.d, i);
                tw.r = max(tw.r, j);
                tw.u = min(tw.u, i);
            }
        }
    }
    tw.h = tw.d - tw.u + 1;
    tw.w = tw.r - tw.l + 1;

    for (int i = 0; i < tw.h; i++) {
        if (tw.l > 0 && board[i + tw.u][tw.l - 1] == 0) {
            startBoardRC = { i + tw.u,tw.l - 1 };
            break;
        }
        else if (tw.r < N - 1 && board[i + tw.u][tw.r + 1] == 0) {
            startBoardRC = { i + tw.u,tw.r + 1 };
            break;
        }
    }
    for (int j = 0; j < tw.w; j++) {
        if (tw.u > 0 && board[tw.u - 1][tw.l + j] == 0) {
            startBoardRC = { tw.u - 1,tw.l + j };
            break;
        }
        else if (tw.d < N - 1 && board[tw.d + 1][tw.l + j] == 0) {
            startBoardRC = { tw.d + 1,tw.l + j };
            break;
        }
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < M; j++) {
            for (int k = 0; k < M; k++) {
                cin >> mboard[i][j][k];
                if (mboard[i][j][k] == 2) {
                    startRC = { j,k };
                }
            }
        }
    }

    fill(&mboardOpen[0][0], &mboardOpen[0][0] + (32 * 32), 3);
    makeMap();

    for (int i = 0; i < F; i++) {
        TimeHole temp;
        cin >> temp.r >> temp.c >> temp.direction >> temp.moveTime;
        timeholes.push_back(temp);
        board[temp.r][temp.c] = 5;
    }
    bool twexit = false;
    bool exitSuc = false;
    T = 0;
    

    startRC.first += tw.h + 1;
    startRC.second += tw.w + 1;
    
    while (!twexit)
    {
        
        twexit = exitTW(++T);
        
    }
    exitMap(T);
    
    cout << T;
}