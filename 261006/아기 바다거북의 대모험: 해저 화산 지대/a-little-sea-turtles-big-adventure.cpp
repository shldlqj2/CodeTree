#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>

using namespace std;

struct Turtle {
    int id;
    int r;
    int c;
    bool dead=false;
    bool exited = false;

};

struct Volcano {
    int r;
    int c;
    int power=0;
    int maxPower;
    bool fire = false;
    bool alreadyFire = false;
};

int N;
int M;
int K;
int board[21][21];
int damageBoard[21][21];
int exited[11];
int needToExit;


pair<int, int> dirs[] = { {0,1},{1,0},{0,-1},{-1,0} };
vector<Turtle> turtles;
vector<Volcano> volcanos;

void MoveTurtle(int time) {
    int tsz = turtles.size();
    for (int i = 0; i < tsz; i++) {
        if (turtles[i].dead || turtles[i].exited) continue;

        queue<Turtle> q;
        q.push(turtles[i]);
        bool visited[21][21] = { 0 };
        visited[turtles[i].r][turtles[i].c] = true;
        pair<int, int> backtracks[21][21];
        fill(&backtracks[0][0], &backtracks[0][0]+(21*21), pair<int,int>{ -1,-1 });

        

        while (!q.empty())
        {
            Turtle curr = q.front();
            q.pop();

            if (curr.r == N - 1 && curr.c == N - 1) {
                break;
            }

            for (int k = 0; k < 4; k++) {
                int nr = curr.r + dirs[k].first;
                int nc = curr.c + dirs[k].second;
                if (0 <= nr && nr < N && 0 <= nc && nc < N &&
                    !visited[nr][nc] && board[nr][nc] == 0) {
                    visited[nr][nc] = true;
                    Turtle next;
                    next.r = nr;
                    next.c = nc;
                    next.id = curr.id;

                    backtracks[nr][nc] = { curr.r,curr.c };

                    q.push(next);
                }
            }
        }

        if (backtracks[N - 1][N - 1].first == -1 && backtracks[N - 1][N - 1].second == -1) {
            continue;
        }
        else {
            board[turtles[i].r][turtles[i].c] = 0;

            int cr = N - 1;
            int cc = N - 1;
            while (!(backtracks[cr][cc].first == turtles[i].r && backtracks[cr][cc].second == turtles[i].c)) {
                int nr = backtracks[cr][cc].first;
                int nc = backtracks[cr][cc].second;
                    
                cr = nr;
                cc = nc;
            }

            
            

            turtles[i].r = cr;
            turtles[i].c = cc;
            
            if (cr == N - 1 && cc == N - 1) {
                turtles[i].exited = true;
                exited[turtles[i].id] = time + 1;
                needToExit -= 1;
                continue;
            }
            else {
                board[turtles[i].r][turtles[i].c] = 2;
            }
        }
    }
}

void VolcanoAdd() {
    for (auto &it : volcanos) {
        it.power += 10;
        if (it.power >= it.maxPower) {
            it.fire = true;

        }
    }
}



void VolcanoBoom() {
    
    /*for (auto &it : volcanos) {
        if (it.fire && !it.alreadyFire) {
            damageBoard[it.r][it.c] += it.maxPower;
            it.alreadyFire = true;
                
            for (int k = 0; k < 4; k++) {
                int currPower = it.maxPower;
                int nr=it.r;
                int nc=it.c;
                while (currPower > 0) {
                    nr += dirs[k].first;
                    nc += dirs[k].second;
                    if (0 <= nr && nr < N && 0 <= nc && nc < N) {
                        if (board[nr][nc] == 1) {
                            currPower = 0;
                            break;
                        }
                        else {
                            currPower /= 2;
                            damageBoard[nr][nc] += currPower;
                        }
                    }
                    else {
                        currPower = 0;
                    }
                }
            }
        }
    }*/
    while (true) {
        bool isThereNew = false;

        for (auto &it : volcanos) {
            if (it.alreadyFire) continue;

            

            if (it.power + damageBoard[it.r][it.c] >= it.maxPower) {
                it.fire = true;
                it.alreadyFire = true;
                isThereNew = true;
                damageBoard[it.r][it.c] += it.maxPower;

                for (int k = 0; k < 4; k++) {
                    int currPower = it.maxPower;
                    int nr = it.r;
                    int nc = it.c;
                    while (currPower > 0) {
                        nr += dirs[k].first;
                        nc += dirs[k].second;
                        if (0 <= nr && nr < N && 0 <= nc && nc < N) {
                            if (board[nr][nc] == 1) {
                                currPower = 0;
                                break;
                            }
                            else {
                                currPower /= 2;
                                damageBoard[nr][nc] += currPower;
                            }
                        }
                        else {
                            currPower = 0;
                        }
                    }
                }
            }

        }

        if (!isThereNew) break;
    }
    

    for (auto &it : turtles) {
        if (it.dead || it.exited) continue;
        if (damageBoard[it.r][it.c] >= 20) {
            it.dead = true;
            needToExit -= 1;
        }
    }
}

void initHwanGyung() {
    fill(&damageBoard[0][0], &damageBoard[0][0] + (21 * 21), 0);
    for (auto &it : volcanos) {
        if (it.alreadyFire && it.fire) {
            it.fire = false;
            it.alreadyFire = false;
            it.power = 0;
        }
    }
}

void printBoard() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int main(void) {
    pair<int, int> backtracks[21][21];
    fill(&backtracks[0][0], &backtracks[0][0] + (21 * 21), pair<int, int>{ -1, -1 });
    fill(&exited[0], &exited[0] + 11, -1);

    cin >> N >> M >> K;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> board[i][j];
        }
    }

    int id = 1;
    for (int i = 0; i < M; i++) {
        Turtle temp;
        cin >> temp.r >> temp.c;
        temp.id = id++;
        turtles.push_back(temp);
        board[temp.r][temp.c] = 2;
    }

    for (int j = 0; j < K; j++) {
        Volcano temp;
        cin >> temp.r >> temp.c >> temp.maxPower;
        volcanos.push_back(temp);

    }
    needToExit = M;
    for (int i = 0; i < 100; i++) {
        /*cout << "Turn : " << i + 1 << endl;
        printBoard();
        MoveTurtle(i);
        cout << "#########" << endl;
        printBoard();
        cout << endl;*/

        MoveTurtle(i);
        VolcanoAdd();
        VolcanoBoom();
        initHwanGyung();
        if (needToExit == 0)break;
    }

    for (int i = 1; i < id; i++) {
        cout << exited[i] << endl;
    }

    return 0;
}