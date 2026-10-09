#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>

using namespace std;

int R, C, K;

pair<int, int> directions[] = { {-1,0},{0,1},{1,0},{0,-1} };
pair<int, int> checkDown[] = { {1,1},{1,-1},{2,0} };
pair<int, int> checkLeft[] = { {-1,-1},{0,-2},{1,-1} };
pair<int, int> checkRight[] = { {-1,1},{0,2},{1,1} };

int board[75][70];
int result;
int golemNum;
bool lastSpirit[75][70] = { 0 };

struct Sprit {
    int r;//정령의 r,c
    int c;
};

bool canDown(int r, int c) {
    int goDown = 0;
    for (int i = 0; i < 3; i++) {
        int nr = r + checkDown[i].first;
        int nc = c + checkDown[i].second;
        if (0 <= nr && nr < R + 3 && 0 <= nc && nc < C &&board[nr][nc] == 0) {
            goDown++;
        }
    }
    if (goDown != 3) {
        return false;
    }
    return true;
}

bool canLeft(int r, int c) {
    int goLeft = 0;
    for (int i = 0; i < 3; i++) {
        int nr = r + checkLeft[i].first;
        int nc = c + checkLeft[i].second;
        if (0 <= nr && nr < R + 3 && 0 <= nc && nc < C &&board[nr][nc] == 0) {
            goLeft++;
        }
    }
    if (goLeft != 3) {
        return false;
    }
    return true;
}

bool canRight(int r, int c) {
    int goRight = 0;
    for (int i = 0; i < 3; i++) {
        int nr = r + checkRight[i].first;
        int nc = c + checkRight[i].second;
        if (0 <= nr && nr < R + 3 && 0 <= nc && nc < C &&board[nr][nc] == 0) {
            goRight++;
        }
    }
    if (goRight != 3) {
        return false;
    }
    return true;
}

void clearBoard(int r, int c, int exitDir) {
    board[r][c] = 0;
    for (int i = 0; i < 4; i++) {
        board[r + directions[i].first][c + directions[i].second] = 0;
    }

}

void drawBoard(int r, int c, int exitDir, int golNum) {
    board[r][c] = golNum;
    for (int i = 0; i < 4; i++) {
        board[r + directions[i].first][c + directions[i].second] = golNum;
    }
    board[r + directions[exitDir].first][c + directions[exitDir].second] = golNum + 10000;
    
}

void clearAll() {
    for (int i = 0; i < R + 3; i++) {
        for (int j = 0; j < C; j++) {
            board[i][j] = 0;
            lastSpirit[i][j] = false;
        }
    }
}

void bestMove(int r, int c) {
    queue<pair<int,pair<int, int>>> q;
    q.push({ board[r][c], { r,c } });
    bool visited[75][70] = { 0 };
    visited[r][c] = true;

    

    int bestR = -2e9;

    while (!q.empty()) {
        pair<int,pair<int, int>> curr = q.front();
        q.pop();

        int currGolNum = curr.first;

        if (currGolNum > 10000) {
            currGolNum -= 10000;
        }

        int cr = curr.second.first;
        int cc = curr.second.second;
        if (bestR < cr) {
            bestR = cr;
        }

        for (int i = 0; i < 4; i++) {
            int nr = cr + directions[i].first;
            int nc = cc + directions[i].second;
            if (0 <= nr && nr < 3 + R && 0 <= nc && nc < C && !visited[nr][nc] && !board[nr][nc] == 0 ) {
                //범위 안에 있고, 방문 안한 곳이고, 보드가 0이 아니고(골렘이고), 다른 정령이 차지하고 있지 않으면 if문 안으로 들어옴
                if (board[nr][nc] == currGolNum || board[nr][nc] == currGolNum+10000) {//다음 이동할 곳이 현재 골렘이면
                    visited[nr][nc] = true;
                    q.push({ currGolNum,{ nr,nc } });
                }
                else {//다음 이동할 곳이 현재 골렘과 달라진다면
                    if (board[cr][cc] > 10000) { //지금 위치가 출구라면 이동 가능
                        visited[nr][nc] = true;
                        q.push({ board[nr][nc],{ nr,nc } });
                        
                    }
                }
            }
        }
    }

    result += (bestR-2);
}

void simulation(int startC, int exitDir) {
    Sprit curr;
    curr.c = startC;
    curr.r = 1;
    int currExit = exitDir;
    drawBoard(curr.r, curr.c, currExit,golemNum);
    
    
    while (true) {
        if (canDown(curr.r,curr.c)) {
            clearBoard(curr.r, curr.c, currExit);
            curr.r += 1;
            drawBoard(curr.r, curr.c, currExit,golemNum);
            continue;
        }
        
        if (canLeft(curr.r, curr.c)) {
            int tempc = curr.c;
            clearBoard(curr.r, curr.c, currExit);
            tempc -= 1;
            if (canDown(curr.r, tempc)) {
                curr.c = tempc;
                curr.r += 1;
                currExit = (currExit + 3) % 4;
                drawBoard(curr.r, curr.c, currExit,golemNum);
                continue;
            }
            else {
                drawBoard(curr.r, curr.c, currExit,golemNum);
            }
        }

        if (canRight(curr.r, curr.c)) {
            int tempc = curr.c;

            clearBoard(curr.r, curr.c, currExit);
            tempc += 1;
            if (canDown(curr.r, tempc)) {
                curr.c = tempc;
                curr.r += 1;
                currExit = (currExit + 1) % 4;
                drawBoard(curr.r, curr.c, currExit,golemNum);
                continue;
            }
            else {
                drawBoard(curr.r, curr.c, currExit,golemNum);
            }
        }


        break;
    }
    
    bool isValid=true;

    for (int i = 0; i < 3; i++) {
        if (!isValid) break;
        for (int j = 0; j < C; j++) {
            if (board[i][j] != 0) {
                isValid = false;
                break;
            }
        }
    }

    if (!isValid) {//삐져나온 골렘 있으면
        clearAll();
        return;
    }

    bestMove(curr.r, curr.c);

}

int main(void) {
    cin >> R >> C >> K;

    result = 0;
    for (int i = 0; i < K; i++) {
        int c, exitDir;
        cin >> c >> exitDir;
        c -= 1;
        golemNum++;
        simulation(c, exitDir);
    }
    cout << result;
}