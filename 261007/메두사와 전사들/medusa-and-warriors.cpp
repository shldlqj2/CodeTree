#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>

using namespace std;

struct Warrior {
    int id;
    int r;
    int c;
    bool isdead=false;
    int stunTime=false;
    bool isStun=0;
};

struct Medusa {
    int r;
    int c;
    int move = 0;
};

int N, M;
pair<int, int> direction[] = { {-1,0},{1,0},{0,-1},{0,1} };
pair<int, int> direction2[] = { {0,-1},{0,1},{-1,0},{1,0} };
pair<int, int> medusaHouse;
pair<int, int> park;
vector<pair<int, int>> medusaRoute;

int board[50][50];
int defBoard[50][50];
int watchBoard[50][50];
int distmap[50][50];

Medusa medusa;
vector<Warrior> warriors;

int moveSum;
int stunSum;
int attackSum;


bool makeRoute() {
    queue<pair<int, int>> q;
    q.push(medusaHouse);

    pair<int, int> backtrack[50][50];
    fill(&backtrack[0][0], &backtrack[0][0] + (50 * 50), pair<int, int>{-1,-1});
    

    while (!q.empty()) {
        pair<int, int> curr;
        curr = q.front();
        q.pop();

        if (curr.first == park.first && curr.second == park.second) {
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nr = curr.first + direction[i].first;
            int nc = curr.second + direction[i].second;

            if (0 <= nr && nr < N && 0 <= nc && nc < N && board[nr][nc] != 1 && backtrack[nr][nc] == pair<int, int>{-1, -1}) {
                backtrack[nr][nc] = { curr.first,curr.second };
                pair<int, int> next = { nr,nc };
                q.push(next);
            }
        }
    }

    int cr = park.first;
    int cc = park.second;
    
    if (backtrack[cr][cc] == pair<int, int>{-1, -1}) return false;


    medusaRoute.push_back({ cr,cc });
    while (!(cr == medusaHouse.first && cc == medusaHouse.second)) {
        pair<int, int> next = backtrack[cr][cc];
        cr = next.first;
        cc = next.second;
        medusaRoute.push_back({ cr,cc });
    }
    medusaRoute.pop_back();
    reverse(medusaRoute.begin(), medusaRoute.end());
    medusaRoute.pop_back();

    return true;
}

void medusaMove(int time) {
    medusa.r = medusaRoute[time].first;
    medusa.c = medusaRoute[time].second;

    if (board[medusa.r][medusa.c] > 1) {
        for (auto &it : warriors) {
            if (it.isdead) continue;
            if (it.r == medusa.r && it.c == medusa.c) {
                it.isdead = true;
                //attackSum++;
            }
        }
        board[medusa.r][medusa.c] = 0;
    }

    queue<pair<int, pair<int, int>>> q;
    q.push({ 0,{ medusa.r,medusa.c } });
    bool visited[50][50] = { 0 };
    visited[medusa.r][medusa.c] = true;
    distmap[medusa.r][medusa.c] = 0;
    while (!q.empty()) {
        pair<int,pair<int, int>> curr;
        curr = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            int nr = curr.second.first + direction[i].first;
            int nc = curr.second.second + direction[i].second;
            if (0 <= nr && nr < N && 0 <= nc && nc < N && !visited[nr][nc]) {
                visited[nr][nc] = true;
                distmap[nr][nc] = curr.first + 1;
                q.push({ curr.first + 1,{nr,nc} });
            }
        }

    }
}

void medusaWatch(int time) {
    int leftWatch[50][50] = { 0 };
    int rightWatch[50][50] = { 0 };
    int upWatch[50][50] = { 0 };
    int downWatch[50][50] = { 0 };

    vector<int> leftStun;
    vector<int> rightStun;
    vector<int> upStun;
    vector<int> downStun;

    for (int i = 0; i < 4; i++) {
        /*
        메두사가 왼쪽 볼때 전사 위치가 메두사 위 아래 중요
        아래 볼때는 왼쪽 오른쪽 중요
        이건 오른쪽 위쪽 볼때도 마찬가지
        왼쪽 오른쪽 위에 아래 볼 때 스턴되는 id 저장해서 따지면 될 듯?
        */
        queue<pair<int, int>> q;
        
        if (i == 0) {
            if (0 <= medusa.r - 1 && medusa.r - 1 < N) {
                upWatch[medusa.r - 1][medusa.c] = 1;
                q.push({ medusa.r - 1, medusa.c });
            }

            for (int r = medusa.r - 1; r >= 0; r--) {
                int nc;

                nc = medusa.c + abs(r - medusa.r);
                if (0 <= nc && nc < N) {
                    upWatch[r][nc] = 1;
                    q.push({ r,nc });
                }

                nc = medusa.c - abs(r - medusa.r);
                if (0 <= nc && nc < N) {
                    upWatch[r][nc] = 1;
                    q.push({ r,nc });
                }
            }
            while (!q.empty())
            {
                pair<int, int> curr;
                curr = q.front();
                q.pop();

                if (upWatch[curr.first][curr.second] == 2) continue;


                if (board[curr.first][curr.second] > 1) {
                    for (const auto &it : warriors) {
                        if (it.isdead) continue;
                        if (it.r == curr.first && it.c == curr.second) {
                            upStun.push_back(it.id);
                        }
                    }
                    
                    queue<pair<int, int>> rq;

                    if (0 <= curr.first - 1 && curr.first - 1 < N) {
                        upWatch[curr.first - 1][curr.second] = 2;
                        rq.push({ curr.first - 1, curr.second });
                    }

                    for (int r = curr.first - 1; r >= 0; r--) {
                        int nc;
                        nc = curr.second + abs(r - curr.first);

                        if (curr.second > medusa.c && 0 <= nc && nc < N) {
                            upWatch[r][nc] = 2;
                            rq.push({ r,nc });
                        }

                        nc = curr.second - abs(r - curr.first);
                        if (curr.second < medusa.c && 0 <= nc && nc < N) {
                            upWatch[r][nc] = 2;
                            rq.push({ r,nc });
                        }

                        while (!rq.empty()) {
                            pair<int, int> removeCurr;
                            removeCurr = rq.front();
                            rq.pop();

                            int nr = removeCurr.first + direction[0].first;
                            int nc = removeCurr.second + direction[0].second;

                            if (0 <= nr && nr < N && 0 <= nc && nc < N) {
                                upWatch[nr][nc] = 2;
                                rq.push({ nr,nc });
                            }
                        }
                    }
                    continue;
                }


                int nr = curr.first + direction[0].first;
                int nc = curr.second + direction[0].second;

                if (0 <= nr && nr < N && 0 <= nc && nc < N) {
                    upWatch[nr][nc] = 1;
                    q.push({ nr,nc });
                }
            }
        }
        else if (i == 1) {
            if (0 <= medusa.r + 1 && medusa.r + 1 < N) {
                downWatch[medusa.r + 1][medusa.c] = 1;
                q.push({ medusa.r + 1, medusa.c });
            }

            for (int r = medusa.r + 1; r  < N; r++) {
                int nc;

                nc = medusa.c + abs(r - medusa.r);
                if (0 <= nc && nc < N) {
                    downWatch[r][nc] = 1;
                    q.push({ r,nc });
                }

                nc = medusa.c - abs(r - medusa.r);
                if (0 <= nc && nc < N) {
                    downWatch[r][nc] = 1;
                    q.push({ r,nc });
                }
            }
            while (!q.empty())
            {
                pair<int, int> curr;
                curr = q.front();
                q.pop();

                if (downWatch[curr.first][curr.second] == 2) continue;


                if (board[curr.first][curr.second] > 1) {
                    for (const auto &it : warriors) {
                        if (it.isdead) continue;
                        if (it.r == curr.first && it.c == curr.second) {
                            downStun.push_back(it.id);
                        }
                    }

                    queue<pair<int, int>> rq;

                    if (0 <= curr.first + 1 && curr.first + 1 < N) {
                        downWatch[curr.first + 1][curr.second] = 2;
                        rq.push({ curr.first + 1, curr.second });
                    }

                    for (int r = curr.first + 1; r < N; r++) {
                        int nc;
                        nc = curr.second + abs(r - curr.first);

                        if (curr.second > medusa.c && 0 <= nc && nc < N) {
                            downWatch[r][nc] = 2;
                            rq.push({ r,nc });
                        }

                        nc = curr.second - abs(r - curr.first);
                        if (curr.second < medusa.c && 0 <= nc && nc < N) {
                            downWatch[r][nc] = 2;
                            rq.push({ r,nc });
                        }

                        while (!rq.empty()) {
                            pair<int, int> removeCurr;
                            removeCurr = rq.front();
                            rq.pop();

                            int nr = removeCurr.first + direction[1].first;
                            int nc = removeCurr.second + direction[1].second;

                            if (0 <= nr && nr < N && 0 <= nc && nc < N) {
                                downWatch[nr][nc] = 2;
                                rq.push({ nr,nc });
                            }
                        }
                    }

                    continue;
                }



                int nr = curr.first + direction[1].first;
                int nc = curr.second + direction[1].second;

                if (0 <= nr && nr < N && 0 <= nc && nc < N) {
                    downWatch[nr][nc] = 1;
                    q.push({ nr,nc });
                }
            }
        }
        else if (i == 2) {
            if (0 <= medusa.c - 1 && medusa.c - 1 < N) {
                leftWatch[medusa.r][medusa.c - 1] = 1;
                q.push({ medusa.r, medusa.c - 1 });
            }
            
            for (int c = medusa.c - 1; c >= 0; c--) {
                int nr;

                nr = medusa.r + abs(c - medusa.c);
                if (0 <= nr && nr < N) {
                    leftWatch[nr][c] = 1;
                    q.push({ nr,c });
                }

                nr = medusa.r - abs(c - medusa.c);
                if (0 <= nr && nr < N) {
                    leftWatch[nr][c] = 1;
                    q.push({ nr,c });
                }
            }
            while (!q.empty())
            {
                pair<int, int> curr;
                curr = q.front();
                q.pop();
                

                if (leftWatch[curr.first][curr.second] == 2) continue;


                if (board[curr.first][curr.second] > 1) {
                    for (const auto &it : warriors) {
                        if (it.isdead) continue;
                        if (it.r == curr.first && it.c == curr.second) {
                            leftStun.push_back(it.id);
                        }
                    }

                    queue<pair<int, int>> rq;

                    if (0 <= curr.second - 1 && curr.second - 1 < N) {
                        leftWatch[curr.first][curr.second - 1] = 2;
                        rq.push({ curr.first, curr.second - 1 });
                    }

                    for (int c = curr.second - 1; c >= 0; c--) {
                        int nr;
                        nr = curr.first + abs(c - curr.second);

                        if (curr.first > medusa.r && 0 <= nr && nr < N) {
                            leftWatch[nr][c] = 2;
                            rq.push({ nr,c });
                        }

                        nr = curr.first - abs(c - curr.second);
                        if (curr.first < medusa.r && 0 <= nr && nr < N) {
                            leftWatch[nr][c] = 2;
                            rq.push({ nr,c });
                        }

                        while (!rq.empty()) {
                            pair<int, int> removeCurr;
                            removeCurr = rq.front();
                            rq.pop();

                            int nr = removeCurr.first + direction[2].first;
                            int nc = removeCurr.second + direction[2].second;

                            if (0 <= nr && nr < N && 0 <= nc && nc < N) {
                                leftWatch[nr][nc] = 2;
                                rq.push({ nr,nc });
                            }
                        }
                    }

                    continue;
                }



                int nr = curr.first + direction[2].first;
                int nc = curr.second + direction[2].second;

                if (0 <= nr && nr < N && 0 <= nc && nc < N) {
                    leftWatch[nr][nc] = 1;
                    q.push({ nr,nc });
                }
            }
        }
        else if (i == 3) {
            if (0 <= medusa.c + 1 && medusa.c + 1 < N) {
                rightWatch[medusa.r][medusa.c + 1] = 1;
                q.push({ medusa.r, medusa.c + 1 });
            }

            for (int c = medusa.c + 1; c < N; c++) {
                int nr;

                nr = medusa.r + abs(c - medusa.c);
                if (0 <= nr && nr < N) {
                    rightWatch[nr][c] = 1;
                    q.push({ nr,c });
                }

                nr = medusa.r - abs(c - medusa.c);
                if (0 <= nr && nr < N) {
                    rightWatch[nr][c] = 1;
                    q.push({ nr,c });
                }
            }
            while (!q.empty())
            {
                pair<int, int> curr;
                curr = q.front();
                q.pop();

                if (rightWatch[curr.first][curr.second] == 2) continue;


                if (board[curr.first][curr.second] > 1) {
                    for (const auto &it : warriors) {
                        if (it.isdead) continue;
                        if (it.r == curr.first && it.c == curr.second) {
                            rightStun.push_back(it.id);
                        }
                    }
                    queue<pair<int, int>> rq;

                    if (0 <= curr.second + 1 && curr.second + 1 < N) {
                        rightWatch[curr.first][curr.second + 1] = 2;
                        rq.push({ curr.first, curr.second + 1 });
                    }

                    for (int c = curr.second + 1; c < N; c++) {
                        int nr;
                        nr = curr.first + abs(c - curr.second);

                        if (curr.first > medusa.r && 0 <= nr && nr < N) {
                            rightWatch[nr][c] = 2;
                            rq.push({ nr,c });
                        }

                        nr = curr.first - abs(c - curr.second);
                        if (curr.first < medusa.r && 0 <= nr && nr < N) {
                            rightWatch[nr][c] = 2;
                            rq.push({ nr,c });
                        }

                        while (!rq.empty()) {
                            pair<int, int> removeCurr;
                            removeCurr = rq.front();
                            rq.pop();

                            int nr = removeCurr.first + direction[3].first;
                            int nc = removeCurr.second + direction[3].second;

                            if (0 <= nr && nr < N && 0 <= nc && nc < N) {
                                rightWatch[nr][nc] = 2;
                                rq.push({ nr,nc });
                            }
                        }
                    }


                    continue;
                }



                int nr = curr.first + direction[3].first;
                int nc = curr.second + direction[3].second;

                if (0 <= nr && nr < N && 0 <= nc && nc < N) {
                    rightWatch[nr][nc] = 1;
                    q.push({ nr,nc });
                }
            }
        }
    }

    vector<pair<int, int>> temp;
    temp.push_back({ upStun.size(),0 });
    temp.push_back({ downStun.size(),1 });
    temp.push_back({ leftStun.size(),2 });
    temp.push_back({ rightStun.size(),3 });

    sort(temp.begin(), temp.end(), [](const auto &a, const auto &b) {
        if (a.first == b.first) {
            return a.second < b.second;
        }
        return a.first > b.first;
    });

    stunSum += temp[0].first;
    
    if (temp[0].second == 0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                watchBoard[i][j] = upWatch[i][j];
            }
        }

        for (int i = 0; i < upStun.size(); i++) {
            warriors[upStun[i] - 10].isStun = true;
            warriors[upStun[i] - 10].stunTime = time;
        }
    }
    else if (temp[0].second == 1) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                watchBoard[i][j] = downWatch[i][j];
            }
        }

        for (int i = 0; i < downStun.size(); i++) {
            warriors[downStun[i] - 10].isStun = true;
            warriors[downStun[i] - 10].stunTime = time;
        }
    }
    else if (temp[0].second == 2) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                watchBoard[i][j] = leftWatch[i][j];
            }
        }

        for (int i = 0; i < leftStun.size(); i++) {
            warriors[leftStun[i] - 10].isStun = true;
            warriors[leftStun[i] - 10].stunTime = time;
        }
    }
    else {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                watchBoard[i][j] = rightWatch[i][j];
            }
        }

        for (int i = 0; i < rightStun.size(); i++) {
            warriors[rightStun[i] - 10].isStun = true;
            warriors[rightStun[i] - 10].stunTime = time;
        }
    }

    
}



void warriorMove(int time) {
    for (int i = 0; i < M; i++) {
        if (warriors[i].isdead) continue;
        if (warriors[i].isStun) {
            if (warriors[i].stunTime - time == 0) {
                continue;
            }
            else {
                warriors[i].isStun = false;
            }
        }

        int boardtemp;
        int cr = warriors[i].r;
        int cc = warriors[i].c;
        bool moved = false;

        //1회이동
        boardtemp = board[cr][cc] - warriors[i].id;


        for (int j = 0; j < 4; j++) {
            int nr = cr + direction[j].first;
            int nc = cc + direction[j].second;
            if (0 <= nr && nr < N && 0 <= nc && nc < N &&
                distmap[nr][nc] < distmap[cr][cc] && watchBoard[nr][nc] != 1) {
                warriors[i].r = nr;
                warriors[i].c = nc;
                moveSum++;
                moved = true;
                break;
            }
        }

        if (moved) {
            board[warriors[i].r][warriors[i].c] += warriors[i].id;
            board[cr][cc] = boardtemp;
            moved = false;
        }
        else {
            continue;
        }
        
        
        if (distmap[warriors[i].r][warriors[i].c] == 0) {
            warriors[i].isdead = true;
            board[warriors[i].r][warriors[i].c] -= warriors[i].id;
            attackSum++;
            continue;
        }

        //2회 이동
        cr = warriors[i].r;
        cc = warriors[i].c;

        boardtemp = board[cr][cc] - warriors[i].id;
        

        for (int j = 0; j < 4; j++) {
            int nr = cr + direction2[j].first;
            int nc = cc + direction2[j].second;
            if (0 <= nr && nr < N && 0 <= nc && nc < N &&
                distmap[nr][nc] < distmap[cr][cc] && watchBoard[nr][nc] != 1) {
                warriors[i].r = nr;
                warriors[i].c = nc;
                moveSum++;
                moved = true;
                break;
            }
        }

        if (moved) {
            board[warriors[i].r][warriors[i].c] += warriors[i].id;
            board[cr][cc] = boardtemp;
            moved = false;
        }

        if (distmap[warriors[i].r][warriors[i].c] == 0) {
            warriors[i].isdead = true;
            board[warriors[i].r][warriors[i].c] -= warriors[i].id;
            attackSum++;
            continue;
        }
    }
}



int main(void) {
    cin >> N >> M;
    cin >> medusaHouse.first >> medusaHouse.second >> park.first >> park.second;
    for (int i = 0; i < M; i++) {
        Warrior temp;
        temp.id = i + 10;
        cin >> temp.r;
        cin >> temp.c;
        warriors.push_back(temp);
        
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int k;
            cin >> k;
            board[i][j] += k;
            defBoard[i][j] = k;
        }
    }

    if (!makeRoute()) {
        cout << -1 << endl;
        return 0;
    }

    for (const auto &it : warriors) {
        board[it.r][it.c] += it.id;
    }

    int maxtime = medusaRoute.size();
    for (int i = 0; i < maxtime; i++) {
        moveSum = 0;
        stunSum = 0;
        attackSum = 0;
        medusaMove(i);
        medusaWatch(i);
        warriorMove(i);
        cout << moveSum << " " << stunSum << " " << attackSum << endl;
    }
    cout << 0 << endl;
}