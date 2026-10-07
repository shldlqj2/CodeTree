#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>
#include <cmath>


using namespace std;


struct Player {
    int id;
    int power;
    int r;
    int c;
    int direction;
    int currGunID = -1;
    int currGunPower = 0;
    int point = 0;
    bool isdead = false;
    bool gotGun = false;
    bool moved = false;
};

struct Pistol {
    int id;
    int r;
    int c;
    int Damage;
    bool use = false;
};

int N, M, K;
int gunBoard[20][20];
int playerBoard[20][20];

pair<int, int> directions[] = { {-1,0},{0,1},{1,0},{0,-1} };//위 오 아 왼
vector<Pistol> guns;
vector<Player> players;

bool whoWin(Player &a, Player &b) {//true면 a가승리 false면 b가 승리
    if (a.currGunPower + a.power == b.currGunPower + b.power) {
        return a.power > b.power;
    }
    return a.currGunPower + a.power > b.currGunPower + b.power;
}

void LoserMove(Player &loser) {
    int cr = loser.r;
    int cc = loser.c;

    int nr;
    int nc;
    int offset;
    for (offset = 0; offset < 4; offset++) { //오프셋 따라 위 오 아 왼 90도씩 움직
        nr = cr + directions[(loser.direction + offset)%4].first;
        nc = cc + directions[(loser.direction + offset)%4].second;
        if (0 <= nr && nr < N && 0 <= nc && nc < N) {
            if (playerBoard[nr][nc] == 0) break;
        }
    }


    loser.direction = (loser.direction + offset) % 4;

    playerBoard[cr][cc] -= 1;
    playerBoard[nr][nc] += 1;

    
    
    if (loser.currGunID != -1) { //패배자가 총 가지고 있으면
        guns[loser.currGunID].use = false;//쓰고있는 총 내려놔
        guns[loser.currGunID].r = cr;//이동 전 위치에 내려
        guns[loser.currGunID].c = cc;//이동 전 위치
        gunBoard[cr][cc] += 1;//이동 전 위치 총 개수 증가
    }

    loser.currGunID = -1;
    loser.currGunPower = 0;


    //loser.moved = true;
    loser.r = nr;
    loser.c = nc;

    if (gunBoard[nr][nc] > 0) {
        int getGun = loser.currGunPower;
        int cnt = 0;
        for (auto &it : guns) {
            if (it.use) continue;

            if (it.r == nr && it.c == nc) {
                if (getGun < it.Damage) {
                    if (loser.currGunID != -1) {//총 있는 상태면
                        gunBoard[nr][nc] += 1; //현재 총 내려놓음
                        guns[loser.currGunID].use = false;
                        guns[loser.currGunID].r = nr;
                        guns[loser.currGunID].c = nc;
                    }
                    loser.currGunID = it.id;
                    loser.currGunPower = it.Damage;
                    gunBoard[nr][nc] -= 1;
                    getGun = it.Damage;//현재 가진 무기 데미지 갱신
                    it.use = true;
                }

            }
        }
    }


}

void WinnerAction(Player &winner) {
    int cr = winner.r;
    int cc = winner.c;

    if(winner.currGunID != -1) {
        gunBoard[cr][cc] += 1;
        guns[winner.currGunID].use = false;
        guns[winner.currGunID].r = cr;
        guns[winner.currGunID].c = cc;
    }

    winner.currGunID = -1;
    winner.currGunPower = 0;

    if (gunBoard[cr][cc] > 0) {
        int getGun = winner.currGunPower;

        for (auto &it : guns) {

            if (it.use) continue;

            if (it.r == cr && it.c == cc) {
                if (getGun < it.Damage) {
                    if (winner.currGunID != -1) { //총 있으면
                        gunBoard[cr][cc] += 1;
                        guns[winner.currGunID].use = false;
                        guns[winner.currGunID].r = cr;
                        guns[winner.currGunID].c = cc;
                    }
                    winner.currGunID = it.id;
                    winner.currGunPower = it.Damage;
                    gunBoard[cr][cc] -= 1;
                    getGun = it.Damage;
                    it.use = true;
                }
            }

        }
    }
}

void simulation() {
    /*for (int i = 0; i < M; i++) {
        players[i].moved = false;
    }*/

    for (int i = 0; i < M; i++) {
        Player *curr = &players[i];
        /*if (curr->moved) continue;*/

        int nr = curr->r + directions[curr->direction].first;
        int nc = curr->c + directions[curr->direction].second;
        if (!(0 <= nr && nr < N && 0 <= nc && nc < N)) {
            curr->direction = (curr->direction + 2) % 4;
            nr = curr->r + directions[curr->direction].first;
            nc = curr->c + directions[curr->direction].second;
        }

        playerBoard[curr->r][curr->c] -= 1;//이동전 칸 1 감소

        curr->r = nr;
        curr->c = nc;

        playerBoard[curr->r][curr->c] += 1;//이동 후 칸 1증가

        //curr->moved = true;

        if (playerBoard[curr->r][curr->c] <= 1) {//나밖에 없다면
            if (gunBoard[curr->r][curr->c] > 0) {
                int getGun=curr->currGunPower;
                for (auto &it : guns) {
                    if (it.use) continue;

                    if (it.r == curr->r && it.c == curr->c) {
                        if (getGun < it.Damage) {
                            if (curr->currGunID != -1) {//총 있는 상태면
                                gunBoard[curr->r][curr->c] += 1; //현재 총 내려놓음
                                guns[curr->currGunID].use = false;
                                guns[curr->currGunID].r = nr;
                                guns[curr->currGunID].c = nc;
                            }
                            it.use = true;
                            curr->currGunID = it.id;
                            curr->currGunPower = it.Damage;
                            getGun = it.Damage;
                            gunBoard[curr->r][curr->c] -= 1;
                            
                        }
                    }
                }
            }
        }
        else if (playerBoard[curr->r][curr->c] > 1) {//플레이어 있다면
            for (int p = 0; p < M; p++) {
                Player *rival = &players[p];
                if (rival->id == curr->id) continue;//자기랑 싸울 순 없어...
                if (rival->r == curr->r && rival->c == curr->c) {
                    //같은 위치에 있다면
                    int currDmg = curr->power + curr->currGunPower;
                    int rivDmg = rival->power + rival->currGunPower;
                    int winnerID = -1;
                    int loserID = -1;
                    bool winnerab = whoWin(*curr, *rival);

                    if (winnerab) {//a가 이긴경우
                        winnerID = curr->id;
                        loserID = rival->id;
                    }
                    else {
                        winnerID = rival->id;
                        loserID = curr->id;
                    }
                    players[winnerID].point += abs((curr->currGunPower + curr->power)
                        - (rival->currGunPower + rival->power));
                    LoserMove(players[loserID]);
                    WinnerAction(players[winnerID]);

                }
            }
        }

    }
}


int main(void) {
    cin >> N >> M >> K;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Pistol pst;
            pst.id = guns.size();
            pst.r = i;
            pst.c = j;

            cin >> pst.Damage;

            if (pst.Damage == 0) continue;

            gunBoard[i][j] += 1;
            guns.push_back(pst);
        }
    }

    for (int i = 0; i < M; i++) {
        Player player;
        cin >> player.r >> player.c >> player.direction >> player.power;
        player.id = players.size();
        player.c -= 1;
        player.r -= 1;

        playerBoard[player.r][player.c] += 1;
        players.push_back(player);
    }

    for (int i = 0; i < K; i++) {
        simulation();
    }

    for (const auto &it : players) {
        cout << it.point << " ";
    }
}