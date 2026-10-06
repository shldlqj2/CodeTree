#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <set>

using namespace std;

struct MisangMul {
    int id;
    //좌하단
    int r1;
    int c1;

    //우상단
    int r2;
    int c2;

    int size=0; // 사이즈

    pair<int,int> real[257];

    bool dead = false;
};
struct queMem
{
    int r;
    int c;
    int size;
};

int N, Q;
int board[16][16];
int MisangMulCnt = 1;
unordered_map<int, MisangMul> misangmuls;
pair<int, int> dirs[] = { {-1,0},{1,0},{0,-1},{0,1} };

void clearBoard(int targetGroup) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == targetGroup) {
                board[i][j] = 0;
            }
        }
    }
}

void printBoard() {
    cout << "###########################" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void setMisangMul() {
    MisangMul temp;
    cin >> temp.r1 >> temp.c1 >> temp.r2 >> temp.c2;
    temp.id = MisangMulCnt++;

    for (int i = temp.r1; i < temp.r2; i++) {
        for (int j = temp.c1; j < temp.c2; j++) {
            board[i][j] = temp.id;
        }
    }

    int groupCnt[51] = { 0 };
    int groupSize[51] = { 0 };
    bool visited[16][16];
    fill(&visited[0][0], &visited[0][0] + (16 * 16), false);

    misangmuls[temp.id] = temp;


    //printBoard();

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            if (board[i][j] != 0 && !visited[i][j]) {
                groupCnt[board[i][j]]++;

                if (groupCnt[board[i][j]] >= 2) {
                    misangmuls[board[i][j]].dead = true;
                    clearBoard(board[i][j]);
                    continue;
                }

                misangmuls[board[i][j]].real[groupSize[board[i][j]]] = { 0,0 };
                groupSize[board[i][j]]++;
                queue<queMem> q;
                queMem start;
                start.r = i;
                start.c = j;
                start.size = 1;
                q.push(start);
                visited[i][j] = true;

                

                while (!q.empty())
                {
                    queMem curr;
                    curr = q.front();
                    q.pop();
                    for (int k = 0; k < 4; k++) {
                        int nr = curr.r + dirs[k].first;
                        int nc = curr.c + dirs[k].second;
                        if (0 <= nr && nr < N && 0 <= nc && nc < N
                            && !visited[nr][nc] && board[nr][nc] == board[i][j]) {
                            visited[nr][nc] = true;
                            misangmuls[board[i][j]].real[groupSize[board[i][j]]] = { nr - i,nc - j };
                            groupSize[board[i][j]]++;
                            queMem next;
                            next.r = nr;
                            next.c = nc;
                            q.push(next);
                        }
                    }
                }

                misangmuls[board[i][j]].size = groupSize[board[i][j]];
            }
        }
    }

    for (auto &it : misangmuls) {
        if (it.second.dead) continue;
        if (groupCnt[it.second.id] == 0) {
            it.second.dead = true;
        }
    }

    //printBoard();
    

}

void moveMisangMul() {
    int tempBoard[16][16] = { 0 };
    vector<MisangMul> tempMisang;
    for (const auto &it : misangmuls) {
        tempMisang.push_back(it.second);
    }

    sort(tempMisang.begin(), tempMisang.end(), [](const MisangMul &a, const MisangMul &b) {
        if (a.size == b.size) {
            return a.id < b.id;
        }
        return a.size > b.size;
    });

    for (int k = 0; k < tempMisang.size(); k++) {
        if (tempMisang[k].dead) continue;
        int h = tempMisang[k].r2 - tempMisang[k].r1;
        int w = tempMisang[k].c2 - tempMisang[k].c1;
        bool isSet = false;

        for (int i = 0; i < N; i++) {
            if (isSet) break;
            for (int j = 0; j < N; j++) {
                if (isSet) break;
                if (tempBoard[i][j] == 0) {
                    bool breakFlag = false;

                    for (int n = 0; n < tempMisang[k].size; n++) {
                        int cr = i + tempMisang[k].real[n].first;
                        int cc = j + tempMisang[k].real[n].second;
                        if (cr >= N || cc >= N || cr<0 || cc<0) breakFlag = true;
                        if (tempBoard[cr][cc]!=0 && tempBoard[cr][cc] != tempMisang[k].id) breakFlag = true;
                    }

                    /*if (i + h > N || j + w > N) {
                        continue;
                    }

                    for (int r = i; r < i + h; r++) {
                        for (int c = j; c < j + w; c++) {
                            if (tempBoard[r][c] != 0) {
                                breakFlag = true;
                                break;
                            }
                        }
                        if (breakFlag) break;
                    }*/

                    if (breakFlag) continue;
                    else {
                        /*int realh=-2e9;
                        int realw=-2e9;
                        for (int r = i; r < i + h; r++) {
                            for (int c = j; c < j + w; c++) {
                                if (tempMisang[k].id == board[tempMisang[k].r1 + r - i][tempMisang[k].c1 + c - j]) {
                                    tempBoard[r][c] = board[tempMisang[k].r1 + r - i][tempMisang[k].c1 + c - j];
                                    realh = max(realh, r);
                                    realw = max(realw, c);
                                }
                                
                            }
                        }

                        realh = realh - i + 1;
                        realw = realw - j + 1;*/

                        for (int n = 0; n < tempMisang[k].size; n++) {
                            int cr = tempMisang[k].real[n].first;
                            int cc = tempMisang[k].real[n].second;
                            tempBoard[i + cr][j + cc] = tempMisang[k].id;
                        }

                        misangmuls[tempMisang[k].id].r1 = i;
                        misangmuls[tempMisang[k].id].r2 = i + h;
                        misangmuls[tempMisang[k].id].c1 = j;
                        misangmuls[tempMisang[k].id].c2 = j + w;


                        isSet = true;
                    }
                }
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            board[i][j] = tempBoard[i][j];
        }
    }
}

void writeTest() {
    //printBoard();

    int result = 0;
    //unordered_set<pair<int, int>,vecto> uos;
    set<pair<int, int>> ords;
    for (const auto& it : misangmuls) {
        if (it.second.dead) continue;

        queue<queMem> q;
        queMem start;
        start.r = it.second.r1;
        start.c = it.second.c1;
        q.push(start);

        bool visited[16][16];
        fill(&visited[0][0], &visited[0][0] + (16 * 16), false);
        visited[start.r][start.c] = true;

        while (!q.empty()) {
            queMem curr = q.front();
            q.pop();
            for (int i = 0; i < 4; i++) {
                int nr = curr.r + dirs[i].first;
                int nc = curr.c + dirs[i].second;

                if (0 <= nr && nr < N && 0 <= nc && nc < N && !visited[nr][nc]) {
                    visited[nr][nc] = true;
                    if (board[nr][nc] == board[start.r][start.c]) {
                        queMem next;
                        next.r = nr;
                        next.c = nc;
                        q.push(next);
                    }
                    else {
                        if (board[nr][nc] == 0) continue;
                        if (board[start.r][start.c] > board[nr][nc]) {
                            ords.insert({ board[nr][nc],board[start.r][start.c] });
                        }
                        else {
                            ords.insert({ board[start.r][start.c],board[nr][nc] });
                        }
                    }
                }
            }
        }
    }
    for (const auto &it : ords) {
        if (it.first == 0) continue;
        result += misangmuls[it.first].size * misangmuls[it.second].size;
    }

    cout << result << endl;
}

int main(void) {
    ios_base::sync_with_stdio(false);
    cout.tie();
    cin.tie();

    cin >> N >> Q;

    for (int i = 0; i < Q; i++) {
        setMisangMul();
        moveMisangMul();
        writeTest();
    }
}