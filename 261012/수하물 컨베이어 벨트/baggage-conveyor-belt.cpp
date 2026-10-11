#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>
#include <cmath>

using namespace std;

struct Box {
    int id;
    int inputTime;
    int cr;
    int cc;
    int nr;
    int nc;
    int exittime = 0;
    int wating = 0;
    int delayed = 0;
    int weight;

    bool exited = false;
    bool skipthisturn = false;
    bool onConv = false;
    bool broken = false;
};

int N, B;
int board[8][8];
int boxBoard[8][8];
pair<int, int> directions[] = { {1,0},{0,-1},{-1,0},{0,1} };
vector<Box> boxes;

void moveBox(int time) {
    int tempBoxBoard[8][8];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            tempBoxBoard[i][j] = boxBoard[i][j];
        }
    }

    for (auto &box : boxes) {
        if (!box.onConv) continue;
        if (box.exited) continue;
        int dirnum = board[box.cr][box.cc] - 1;
        int nr = box.cr + directions[dirnum].first;
        int nc = box.cc + directions[dirnum].second;
        tempBoxBoard[box.cr][box.cc]--;
        tempBoxBoard[nr][nc]++;
        box.nr = nr;
        box.nc = nc;
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (tempBoxBoard[i][j] >= 2) {
                vector<Box> hubos;
                for (const auto &box : boxes) {
                    if (!box.onConv) continue;
                    if (box.exited) continue;
                    if (box.nr == i && box.nc == j) {
                        hubos.push_back(box);
                    }
                }
                sort(hubos.begin(), hubos.end(), [](const Box &a, const Box &b) {
                    if (a.weight == b.weight) {
                        return a.id < b.id;
                    }
                    return a.weight > b.weight;
                });
                for (int k = 1; k < hubos.size(); k++) {
                    boxes[hubos[k].id].skipthisturn = true;
                }
            }
        }
    }

    fill(&tempBoxBoard[0][0], &tempBoxBoard[0][0] + (8 * 8), 0);

    for (auto &it : boxes) {
        if (!it.onConv) continue;
        if (it.exited) continue;
        tempBoxBoard[it.cr][it.cc]++;
    }

    for (int i = 0; i < B; i++) {
        for (auto &it : boxes) {
            if (!it.onConv) continue;
            if (it.exited) continue;

            if (it.skipthisturn) {
                continue;
            }
            if (tempBoxBoard[it.nr][it.nc] > 0) {
                it.wating++;
                continue;
            }
            tempBoxBoard[it.cr][it.cc]--;
            tempBoxBoard[it.nr][it.nc]++;
            it.cr = it.nr;
            it.cc = it.nc;
            it.delayed = 0;
        }
    }

    for (auto &box : boxes) {
        if (!box.onConv) continue;
        if (box.exited) continue;
        if (box.skipthisturn) {
            box.skipthisturn = false;
            box.delayed++;
            box.wating = 0;
            continue;
        }
        if (box.wating == B) {
            box.delayed++;
        }
        box.wating = 0;
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            boxBoard[i][j] = tempBoxBoard[i][j];
        }
    }
}

void jukjaeAndBroke(int time) {
    for (auto &box : boxes) {
        if (!box.onConv) continue;
        if (box.exited) continue;
        if (board[box.cr][box.cc] == 9) {
            box.exited = true;
            box.exittime = time;
            boxBoard[box.cr][box.cc]--;
        }
        if (box.delayed >= 3) {
            box.broken = true;
        }
    }
}

void inputBox(int time) {
    int tempInputBox[8][8];
    fill(&tempInputBox[0][0], &tempInputBox[0][0] + (8 * 8), 0);
    for (auto &box : boxes) {
        if (box.onConv) continue;
        if (box.exited) continue;

        if (boxBoard[box.cr][box.cc] > 0) {
            continue;
        }

        if (box.inputTime > time) {
            continue;
        }

        tempInputBox[box.cr][box.cc]++;
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (tempInputBox[i][j] >= 2) {
                vector<Box> hubos;
                for (const auto &box : boxes) {
                    if (box.onConv) continue;
                    if (box.exited) continue;
                    if (box.cr == i && box.cc == j) {
                        hubos.push_back(box);
                    }
                }
                sort(hubos.begin(), hubos.end(), [](const Box &a, const Box &b) {
                    return(a.inputTime < b.inputTime);
                });
                boxes[hubos[0].id].onConv = true;
                boxBoard[boxes[hubos[0].id].cr][boxes[hubos[0].id].cc]++;
            }
            else if (tempInputBox[i][j] == 1) {
                for (auto &box : boxes) {
                    if (box.onConv) continue;
                    if (box.exited) continue;

                    if (boxBoard[box.cr][box.cc] > 0) {
                        continue;
                    }

                    if (box.inputTime > time) {
                        continue;
                    }

                    if (box.cr == i && box.cc == j) {
                        boxBoard[box.cr][box.cc]++;
                        box.onConv = true;
                    }
                }
            }
        }
    }
}

void initSimul(void) {
    fill(&board[0][0], &board[0][0] + (8 * 8), 0);
    fill(&boxBoard[0][0], &boxBoard[0][0] + (8 * 8), 0);
    boxes.clear();
}

int main(void) {
    initSimul();

    cin >> N;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> board[i][j];
        }
    }

    cin >> B;
    for (int i = 0; i < B; i++) {
        Box temp;
        temp.id = boxes.size();
        cin >> temp.inputTime >> temp.cr >> temp.cc >> temp.weight;
        temp.cr -= 1;
        temp.cc -= 1;
        boxes.push_back(temp);
    }

    for (int t = 1; t <= 200; t++) {
        moveBox(t);
        jukjaeAndBroke(t);
        inputBox(t);
    }

    for (const auto &box : boxes) {
        cout << box.exittime * (box.broken ? -1 : 1) << " ";
    }
    cout << endl;
}