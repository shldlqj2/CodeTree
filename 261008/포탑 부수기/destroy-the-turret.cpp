#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>


using namespace std;

struct Tower {
    int id;
    int r;
    int c;
    int damage;
    int lastAtkTime = 0;
    bool destroy = false;
};

int N, M, K;
Tower board[10][10];
vector<Tower> towers;
pair<int, int> directions[] = { {0,1},{1,0},{0,-1},{-1,0} };
pair<int, int> potanDir[] = { {1,1},{-1,1},{-1,-1},{1,-1},
                                {0,1},{1,0},{0,-1},{-1,0} };

bool skipFlag[100];

void printBoard() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cout << board[i][j].damage << " ";
        }
        cout << endl;
    }
}

int makeInner(int target, int num) {
    if (target >= num) {
        return target % num;
    }
    else if (target < 0) {
        return num + target;
    }

    return target;
}

bool laserAttack(Tower &atk, Tower &dfd) {
    queue<pair<int, int>> q;
    pair<int, int> backtrack[10][10];
    bool visited[10][10] = { 0 };
    fill(&backtrack[0][0], &backtrack[0][0] + (10 * 10), pair<int, int>{-1, -1});
    board[atk.r][atk.c].damage = atk.damage;

    q.push({ atk.r,atk.c });
    visited[atk.r][atk.c] = true;

    while (!q.empty()) {
        pair<int, int> curr;
        curr = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            int nr = curr.first + directions[i].first;
            int nc = curr.second + directions[i].second;

            nr = makeInner(nr, N);
            nc = makeInner(nc, M);

            if (!visited[nr][nc] && !board[nr][nc].destroy) {
                backtrack[nr][nc] = { curr.first,curr.second };
                visited[nr][nc] = true;
                q.push({ nr,nc });
            }

        }
    }
    /*cout << "#######################" << endl;
    cout << "레이저 공격중 보드 상태 " << endl;
    printBoard();*/
    if (backtrack[dfd.r][dfd.c].first == -1 && backtrack[dfd.r][dfd.c].second == -1) {
        //-1,-1이면 안맞은거
        return false;
    }
    else {
        
        int dmg = atk.damage;
        board[dfd.r][dfd.c].damage = max(board[dfd.r][dfd.c].damage - dmg, 0);
        skipFlag[dfd.id] = true;
        pair<int, int> next = backtrack[dfd.r][dfd.c];
        dmg = dmg / 2;
        while (!(next.first == atk.r && next.second == atk.c)) {//-1,-1이 아니면 즉 atk까지 안가면
            board[next.first][next.second].damage = max(board[next.first][next.second].damage - dmg, 0);
            skipFlag[board[next.first][next.second].id] = true;
            next = backtrack[next.first][next.second];

        }
        return true;
    }
    

    
}

void potanAttack(Tower &atk, Tower &dfd) {
    
    int dmg = atk.damage;
    board[dfd.r][dfd.c].damage = max(board[dfd.r][dfd.c].damage - dmg, 0);
    skipFlag[dfd.id] = true;
    dmg /= 2;
    for (int i = 0; i < 8; i++) {
        int nr = dfd.r + potanDir[i].first;
        int nc = dfd.c + potanDir[i].second;

        nr = makeInner(nr, N);
        nc = makeInner(nc, M);
        if (nr == atk.r && nc == atk.c) continue;
        if (!board[nr][nc].destroy) {
            board[nr][nc].damage = max(board[nr][nc].damage - dmg, 0);
            skipFlag[board[nr][nc].id] = true;
        }
    }

}

void repiarTowers() {
    for (auto &it : towers) {
        if (skipFlag[it.id]) {
            skipFlag[it.id] = false;
            continue;
        }
        if (it.destroy) {
            continue;
        }
        it.damage += 1;
        board[it.r][it.c].damage += 1;
    }
}

void destroyTower() {
    for (auto &it : towers) {
        if (board[it.r][it.c].damage == 0) {
            board[it.r][it.c].destroy = true;
            it.destroy = true;
            it.damage = 0;
        }
        else {
            it.damage = board[it.r][it.c].damage;
        }
        it.lastAtkTime = board[it.r][it.c].lastAtkTime;
    }
}


void simulation(int time) {
    Tower attacker;
    Tower defender;
    bool atkset = false;
    bool defset = false;

    sort(towers.begin(), towers.end(), [](const Tower &a, const Tower &b) {
        if (a.damage == b.damage) {
            if (a.lastAtkTime == b.lastAtkTime) {
                if (a.r + a.c == b.r + b.c) {
                    return a.c > b.c;
                }
                return a.r + a.c > b.r + b.c;
            }
            return a.lastAtkTime > b.lastAtkTime;
        }
        return a.damage < b.damage;
    });

    for (auto& it : towers) {
        if (it.destroy) continue;
        attacker = it;
        atkset = true;
        skipFlag[it.id] = true;
        break;
    }
    attacker.damage += (M + N);
    
    
    sort(towers.begin(), towers.end(), [](const Tower &a, const Tower &b) {
        if (a.damage == b.damage) {
            if (a.lastAtkTime == b.lastAtkTime) {
                if (a.r + a.c == b.r + b.c) {
                    return a.c < b.c;
                }
                return a.r + a.c < b.r + b.c;
            }
            return a.lastAtkTime < b.lastAtkTime;
        }
        return a.damage > b.damage;
    });

    for (auto &it : towers) {
        if (it.destroy) continue;
        if (it.id == attacker.id) continue;
        defender = it;
        defset = true;
        break;
    }

    attacker.lastAtkTime = time;
    board[attacker.r][attacker.c].lastAtkTime = time;



    if (atkset && defset) {
        if (!laserAttack(attacker, defender)) {
            potanAttack(attacker, defender);
        }

        destroyTower();
        repiarTowers();
    }
}

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(0);


    cin >> N >> M >> K;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            Tower temp;
            temp.id = towers.size();
            temp.r = i;
            temp.c = j;
            cin >> temp.damage;
            if (temp.damage == 0) temp.destroy = true;
            towers.push_back(temp);
            board[i][j] = temp;
        }
    }

    for (int i = 1; i <= K; i++) {
        /*cout << "시뮬전" << endl;
        printBoard();
        cout << endl;
        simulation(i);
        cout << "시뮬후" << endl;
        printBoard();
        cout << endl;*/

        simulation(i);
    }

    sort(towers.begin(), towers.end(), [](const Tower &a, const Tower &b) {
        return a.damage > b.damage;
    });

    cout << towers[0].damage << endl;

    return 0;
}