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
    int l = 2e9;
    int d = -2e9;
    int r = -2e9;
    int u = 2e9;
    int h;
    int w;
};

int board[20][20];
pair<int, int> directions[] = { {0,1},{0,-1},{1,0},{-1,0} };
int mboard[5][10][10];
pair<int, int> exitRC;
pair<int, int> exitTwRC;
pair<int, int> startRC;
pair<int, int> startBoardRC;
vector<TimeHole> timeholes;
Timewall tw;
int mboardOpen[32][32];
int danger[20][20];
int T;

bool inBoard(int r, int c) {
    return 0 <= r && r < N && 0 <= c && c < N;
}

int boardCell(int r, int c) {
    if (!inBoard(r, c)) return 1;
    return board[r][c];
}

void rotateBoard(int num, int times) {
    for (int t = 0; t < times; t++) {
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
        mboardOpen[tw.h + 1 + i][0] = boardCell(tw.u + i, tw.l - 1);

        if (mboardOpen[tw.h + 1 + i][0] == 0) {
            exitTwRC = { tw.h + 1 + i, 0 };
        }
    }
}

void makeRight() {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            mboardOpen[1 + tw.h + i][1 + 2 * tw.w + j] = mboard[0][i][j];
        }
    }

    for (int j = 0; j < M; j++) {
        mboardOpen[tw.h][1 + 2 * tw.w + j] = 10;
        mboardOpen[tw.h + 1 + M][1 + 2 * tw.w + j] = 10;
    }

    for (int i = 0; i < M; i++) {
        mboardOpen[tw.h + 1 + i][1 + 2 * tw.w + M] = boardCell(tw.u + i, tw.r + 1);

        if (mboardOpen[tw.h + 1 + i][1 + 2 * tw.w + M] == 0) {
            exitTwRC = { tw.h + 1 + i, 1 + 2 * tw.w + M };
        }
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
        mboardOpen[0][1 + tw.w + i] = boardCell(tw.u - 1, tw.l + i);

        if (mboardOpen[0][1 + tw.w + i] == 0) {
            exitTwRC = { 0, 1 + tw.w + i };
        }
    }
}

void makeDown() {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            mboardOpen[1 + 2 * tw.h + i][1 + tw.w + j] = mboard[2][i][j];
        }
    }

    for (int i = 0; i < M; i++) {
        mboardOpen[1 + 2 * tw.h + i][tw.w] = 10;
        mboardOpen[1 + 2 * tw.h + i][1 + M + tw.w] = 10;
    }

    for (int i = 0; i < M; i++) {
        mboardOpen[1 + 2 * tw.h + M][1 + tw.w + i] = boardCell(tw.d + 1, tw.l + i);

        if (mboardOpen[1 + 2 * tw.h + M][1 + tw.w + i] == 0) {
            exitTwRC = { 1 + 2 * tw.h + M, 1 + tw.w + i };
        }
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

void makeDanger() {
    fill(&danger[0][0], &danger[0][0] + 20 * 20, (int)2e9);

    for (const auto &it : timeholes) {
        for (int step = 0; step < N; step++) {
            int nr = it.r + step * directions[it.direction].first;
            int nc = it.c + step * directions[it.direction].second;

            if (!inBoard(nr, nc)) break;
            if (board[nr][nc] == 1 || board[nr][nc] == 3 || board[nr][nc] == 4) break;

            danger[nr][nc] = min(danger[nr][nc], step * it.moveTime);
        }
    }
}

int exitTW() {
    queue<pair<int, int>> q;
    int dist[32][32];

    fill(&dist[0][0], &dist[0][0] + 32 * 32, -1);

    q.push(startRC);
    dist[startRC.first][startRC.second] = 0;

    while (!q.empty()) {
        auto curr = q.front();
        q.pop();

        int cr = curr.first;
        int cc = curr.second;

        if (curr == exitTwRC) return dist[cr][cc];

        for (int i = 0; i < 4; i++) {
            int nr = cr + directions[i].first;
            int nc = cc + directions[i].second;

            if (nr < 0 || nr >= 3 * tw.h + 2 || nc < 0 || nc >= 3 * tw.w + 2) continue;

            if (mboardOpen[nr][nc] == 10) {
                if (cr < 1 + tw.h) {
                    int offset = 1 + tw.h - cr;
                    nr = cr + offset;
                    nc = (i == 0 ? cc + offset : cc - offset);
                }
                else if (cr >= 1 + 2 * tw.h) {
                    int offset = cr - (1 + 2 * tw.h) + 1;
                    nr = cr - offset;
                    nc = (i == 0 ? cc + offset : cc - offset);
                }
                else if (cc < 1 + tw.w) {
                    int offset = 1 + tw.w - cc;
                    nr = (i == 3 ? cr - offset : cr + offset);
                    nc = cc + offset;
                }
                else if (cc >= 1 + 2 * tw.w) {
                    int offset = cc - (1 + 2 * tw.w) + 1;
                    nr = (i == 3 ? cr - offset : cr + offset);
                    nc = cc - offset;
                }
            }

            if (nr < 0 || nr >= 3 * tw.h + 2 || nc < 0 || nc >= 3 * tw.w + 2) continue;
            if (dist[nr][nc] != -1 || mboardOpen[nr][nc] != 0) continue;

            dist[nr][nc] = dist[cr][cc] + 1;
            q.push({ nr, nc });
        }
    }

    return -1;
}

int exitMap(int time) {
    if (!inBoard(startBoardRC.first, startBoardRC.second)) return -1;
    if (board[startBoardRC.first][startBoardRC.second] != 0) return -1;
    if (danger[startBoardRC.first][startBoardRC.second] <= time) return -1;

    queue<pair<int, pair<int, int>>> q;
    q.push({ time, startBoardRC });

    bool visited[20][20] = { 0 };
    visited[startBoardRC.first][startBoardRC.second] = true;

    while (!q.empty()) {
        auto curr = q.front();
        q.pop();

        int cr = curr.second.first;
        int cc = curr.second.second;

        if (curr.second == exitRC) return curr.first;

        for (int i = 0; i < 4; i++) {
            int nr = cr + directions[i].first;
            int nc = cc + directions[i].second;
            int nextTime = curr.first + 1;

            if (!inBoard(nr, nc) || visited[nr][nc]) continue;
            if (board[nr][nc] != 0 && board[nr][nc] != 4) continue;
            if (danger[nr][nc] <= nextTime) continue;

            visited[nr][nc] = true;
            q.push({ nextTime, { nr, nc } });
        }
    }

    return -1;
}

int main(void) {
    cin >> N >> M >> F;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> board[i][j];

            if (board[i][j] == 4) {
                exitRC = { i, j };
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
        if (boardCell(i + tw.u, tw.l - 1) == 0) {
            startBoardRC = { i + tw.u, tw.l - 1 };
        }

        if (boardCell(i + tw.u, tw.r + 1) == 0) {
            startBoardRC = { i + tw.u, tw.r + 1 };
        }
    }

    for (int j = 0; j < tw.w; j++) {
        if (boardCell(tw.u - 1, tw.l + j) == 0) {
            startBoardRC = { tw.u - 1, tw.l + j };
        }

        if (boardCell(tw.d + 1, tw.l + j) == 0) {
            startBoardRC = { tw.d + 1, tw.l + j };
        }
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < M; j++) {
            for (int k = 0; k < M; k++) {
                cin >> mboard[i][j][k];

                if (i == 4 && mboard[i][j][k] == 2) {
                    startRC = { j, k };
                }
            }
        }
    }

    fill(&mboardOpen[0][0], &mboardOpen[0][0] + 32 * 32, 3);
    makeMap();

    for (int i = 0; i < F; i++) {
        TimeHole temp;
        cin >> temp.r >> temp.c >> temp.direction >> temp.moveTime;

        timeholes.push_back(temp);
        board[temp.r][temp.c] = 5;
    }

    makeDanger();

    startRC.first += tw.h + 1;
    startRC.second += tw.w + 1;

    T = exitTW();

    if (T != -1) {
        T = exitMap(T);
    }

    cout << T;

    return 0;
}