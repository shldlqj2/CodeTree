#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;

struct BestSelect {
    int rot;
    int rotr;
    int rotc;
    int score;
    pair<int, int> hubos[25];
};
struct QueMem {
    int r;
    int c;
    int sameNum;
};

int K, M;



int board[5][5];
int rotboard[3][9][5][5];
int remainYumul[301];
int result = 0;
int remainYumulIdx = 0;
pair<int, int> directions[] = { {-1,0},{1,0},{0,1},{0,-1} };
bool finishflag = false;

void rotate90(int r, int c, int times) {
    int tempboard[3][3];
    for (int t = 0; t < times; t++) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                tempboard[i][j] = rotboard[times -1][(r - 1) * 3 + (c - 1)][r - 1 + i][c - 1 + j];
            }
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                rotboard[times - 1][(r - 1) * 3 + (c - 1)][r - 1 + i][c - 1 + j] = tempboard[2 - j][i];
            }
        }
    }
}

void getYumul(BestSelect *selected) {
    sort(&selected->hubos[0], &selected->hubos[0] + (selected->score), [](const auto &a, const auto &b) {
        if (a.second == b.second) {
            return a.first > b.first;
        }
        return a.second < b.second;
    });

    for (int i = 0; i < selected->score; i++) {
        if (remainYumulIdx >= M) {
            remainYumulIdx = 0;
        }
        board[selected->hubos[i].first][selected->hubos[i].second] = remainYumul[remainYumulIdx++];
    }

    while (true) {
        int currscore = 0;
        bool visited[5][5] = { 0 };
        pair<int, int> hubos[25];
        for (int r = 0; r < 5; r++) {
            for (int c = 0; c < 5; c++) {
                if (!visited[r][c]) {
                    int nowscore = 1;
                    visited[r][c] = true;
                    pair<int, int> wheres[25];
                    queue<QueMem> q;
                    QueMem st;
                    st.r = r;
                    st.c = c;
                    st.sameNum = board[r][c];
                    wheres[0] = { r,c };
                    q.push(st);
                    

                    while (!q.empty())
                    {
                        QueMem curr = q.front();
                        q.pop();
                        for (int k = 0; k < 4; k++) {
                            int nr = curr.r + directions[k].first;
                            int nc = curr.c + directions[k].second;
                            if (0 <= nr && nr < 5 && 0 <= nc && nc < 5 && !visited[nr][nc] &&
                                board[nr][nc] == curr.sameNum) {
                                visited[nr][nc] = true;
                                QueMem next;
                                next.r = nr;
                                next.c = nc;
                                next.sameNum = curr.sameNum;
                                q.push(next);
                                wheres[nowscore++] = { nr,nc };
                            }
                        }
                    }
                    if (nowscore >= 3) {
                        for (int s = 0; s < nowscore; s++) {
                            hubos[s + currscore] = wheres[s];
                        }
                        currscore += nowscore;
                    }

                }
            }
        }

        sort(&hubos[0], &hubos[0] + (currscore), [](const auto &a, const auto &b) {
            if (a.second == b.second) {
                return a.first > b.first;
            }
            return a.second < b.second;
        });

        for (int i = 0; i < currscore; i++) {
            if (remainYumulIdx >= M) {
                remainYumulIdx = 0;
            }
            board[hubos[i].first][hubos[i].second] = remainYumul[remainYumulIdx++];
        }

        result += currscore;

        if (currscore == 0) {
            break;
        }
    }

    
}

void startTamsa() {
    for (int i = 1; i < 4; i++) {
        for (int j = 1; j < 4; j++) {
            for (int rot = 1; rot < 4; rot++) {
                
                for (int r = 0; r < 5; r++) {
                    for (int c = 0; c < 5; c++) {
                        rotboard[rot - 1][(i - 1) * 3 + (j - 1)][r][c] = board[r][c];
                    }
                }
                rotate90(i, j, rot);

            }
        }
    }


    BestSelect sel[27];

    

    for (int rot = 0; rot < 3; rot++) {
        for (int i = 0; i < 9; i++) {
            int currScore = 0;
            bool visited[5][5] = { 0 };
            for (int r = 0; r < 5; r++) {
                for (int c = 0; c < 5; c++) {
                    if (!visited[r][c]) {
                        int nowscore = 1;
                        
                        queue<QueMem> q;
                        QueMem start;
                        start.r = r;
                        start.c = c;
                        start.sameNum = rotboard[rot][i][r][c];

                        q.push(start);
                        visited[r][c] = true;

                        pair<int, int> wheres[25];
                        wheres[0] = { r,c };

                        while (!q.empty())
                        {
                            QueMem curr = q.front();
                            q.pop();
                            for (int k = 0; k < 4; k++) {
                                int nr = curr.r + directions[k].first;
                                int nc = curr.c + directions[k].second;
                                if (0 <= nr && nr < 5 && 0 <= nc && nc < 5 && !visited[nr][nc] &&
                                    rotboard[rot][i][nr][nc] == curr.sameNum) {
                                    visited[nr][nc] = true;
                                    QueMem next;
                                    next.r = nr;
                                    next.c = nc;
                                    next.sameNum = curr.sameNum;
                                    q.push(next);
                                    wheres[nowscore++] = { nr,nc };
                                }
                            }
                        }

                        if (nowscore >= 3) {
                            
                            for (int s = 0; s < nowscore; s++) {
                                sel[9 * rot + i].hubos[s + currScore] = wheres[s];
                            }
                            currScore += nowscore;
                        }
                    }
                    
                }
            }
            sel[9 * rot + i].score = currScore;
            sel[9 * rot + i].rot = rot;
            switch (i)
            {
            case 0:
                sel[9 * rot + i].rotr = 1;
                sel[9 * rot + i].rotc = 1;
                break;
            case 1:
                sel[9 * rot + i].rotr = 1;
                sel[9 * rot + i].rotc = 2;
                break;
            case 2:
                sel[9 * rot + i].rotr = 1;
                sel[9 * rot + i].rotc = 3;
                break;
            case 3:
                sel[9 * rot + i].rotr = 2;
                sel[9 * rot + i].rotc = 1;
                break;
            case 4:
                sel[9 * rot + i].rotr = 2;
                sel[9 * rot + i].rotc = 2;
                break;
            case 5:
                sel[9 * rot + i].rotr = 2;
                sel[9 * rot + i].rotc = 3;
                break;
            case 6:
                sel[9 * rot + i].rotr = 3;
                sel[9 * rot + i].rotc = 1;
                break;
            case 7:
                sel[9 * rot + i].rotr = 3;
                sel[9 * rot + i].rotc = 2;
                break;
            case 8:
                sel[9 * rot + i].rotr = 3;
                sel[9 * rot + i].rotc = 3;
                break;
            default:
                break;
            }
            
        }
    }

    sort(&sel[0], &sel[0]+27, [](const BestSelect &a, const BestSelect &b) {
        if (a.score == b.score) {
            if (a.rot == b.rot) {
                if (a.rotc == b.rotc) {
                    return a.rotr < b.rotr;
                }
                return a.rotc < b.rotc;
            }
            return a.rot < b.rot;
        }
        return a.score > b.score;
    });

    result += sel[0].score;

    if (sel[0].score == 0) {
        finishflag = true;
        return;
    }
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            board[i][j] = rotboard[sel[0].rot][(sel[0].rotr - 1) * 3 + (sel[0].rotc - 1)][i][j];
        }
    }

    getYumul(&sel[0]);

    

}



int main(void) {
    cin >> K >> M;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            cin >> board[i][j];
        }
    }
    for (int i = 0; i < M; i++) {
        cin >> remainYumul[i];
    }
    for (int i = 0; i < K; i++) {
        if (finishflag) break;
        result = 0;
        startTamsa();
        if (!finishflag) {
            cout << result << " ";
        }
    }
    
}