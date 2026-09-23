#include <iostream>
#include <vector>
#include <queue>

#define EGG 10
#define PACMAN 2
#define GHOST 1

using namespace std;

int board[4][4]; //0은 빈칸 1은 팩맨 2는 고스트 3은 고스트알
int deadBoard[4][4];
pair<int, int> ghostDir[] = { {-1,0},{-1,-1},{0,-1},{1,-1},{1,0}, {1,1},{0,1},{-1,1} };
pair<int, int> pacmanDir[] = { {-1,0},{0,-1},{1,0},{0,1} };
int ghostCnt;

struct Pacman {
    int r=-1;
    int c=-1;
};

struct Ghost {
    int r=-1;
    int c=-1;
    int dir=0;
    bool dead=false;
    int deadTime = -1;
};

struct Egg {
    int r=-1;
    int c=-1;
    int dir=0;
};

struct PacmanMove {
    int r=-1;
    int c=-1;
    int move=0;
    int kill=0;
    int movedir[4] = { -1,-1,-1,-1 };
    pair<int, int> killedRC[4] = { {-1,-1},{-1,-1},{-1,-1},{-1,-1} };
};

struct PacmanMoveCmp
{
    bool operator()(const PacmanMove &a, const PacmanMove &b)const {
        if (a.kill == b.kill) {
            if (a.movedir[0] == b.movedir[0]) {
                if (a.movedir[1] == b.movedir[1]) {
                    if (a.movedir[2] == b.movedir[2]) {
                        return a.movedir[3] > b.movedir[3];
                    }
                    return a.movedir[2] > b.movedir[2];
                }
                return a.movedir[1] > b.movedir[1];
            }
            return a.movedir[0] > b.movedir[0];
        }
        return a.kill < b.kill;
    }
};

vector<Ghost> ghost;
vector<Egg> eggs;
Pacman pacman;


void ghostDup(){
    for (int i = 0; i < ghost.size(); i++) {
        if (ghost[i].dead) continue;

        Egg temp;
        temp.r = ghost[i].r;
        temp.c = ghost[i].c;
        temp.dir = ghost[i].dir;
        eggs.push_back(temp);
    }
}
void ghostMove() {
    for (int i = 0; i < ghost.size(); i++) {
        if (ghost[i].dead) {
            continue;
        }
        int nr = ghost[i].r + ghostDir[ghost[i].dir].first;
        int nc = ghost[i].c + ghostDir[ghost[i].dir].second;
        int chkcnt = 0;
        int firstdir = ghost[i].dir;
        while (0 > nr || nr >= 4 || 0 > nc || nc >= 4 || (nr == pacman.r && nc == pacman.c) || (deadBoard[nr][nc])) {
            if (chkcnt++ >= 7) {
                ghost[i].dir = firstdir;
                break;
            }
            ghost[i].dir = (ghost[i].dir+1)%8;
            nr = ghost[i].r + ghostDir[ghost[i].dir].first;
            nc = ghost[i].c + ghostDir[ghost[i].dir].second;
        }
        if (0 <= nr && nr < 4 && 0 <= nc && nc < 4 && !(nr == pacman.r && nc == pacman.c) && !deadBoard[nr][nc]) {
            board[ghost[i].r][ghost[i].c] -= 1;

            ghost[i].r = nr;
            ghost[i].c = nc;
            
            board[ghost[i].r][ghost[i].c] += 1;
        }
        
    }
}
void pacMove(int time) {
    queue<PacmanMove> q;
    priority_queue<PacmanMove,vector<PacmanMove>,PacmanMoveCmp> pq;
    PacmanMove start;
    start.r = pacman.r;
    start.c = pacman.c;
    start.move = 0;
    start.kill = 0;
    q.push(start);
    
    while (!q.empty()) {
        PacmanMove curr = q.front();
        q.pop();
        if (curr.move >= 3) {
            pq.push(curr);
            continue;
        }
        
        int cr = curr.r;
        int cc = curr.c;

        

        for (int i = 0; i < 4; i++) {
            int nr = cr + pacmanDir[i].first;
            int nc = cc + pacmanDir[i].second;

            if (0 <= nr && nr < 4 && 0 <= nc && nc < 4) {
                bool isKilled = false;

                PacmanMove next;
                next = curr;
                next.r = nr;
                next.c = nc;
                next.move = curr.move + 1;
                next.kill = curr.kill;
                
                for (int j = 0; j < 4; j++) {
                    if (curr.killedRC[j] == pair<int,int>{nr, nc}) {
                        isKilled = true;
                    }
                }
                if (board[nr][nc] >= 0 && (!isKilled)) {
                    next.killedRC[curr.move] = { nr,nc };
                    next.kill += board[nr][nc];
                }
                next.movedir[curr.move] = i;
                

                q.push(next);
            }
        }
    }
    PacmanMove finishMove = pq.top();
    for (int i = 0; i < 3; i++) {
        int killedr = finishMove.killedRC[i].first;
        int killedc = finishMove.killedRC[i].second;
        if (killedr == -1 && killedc == -1) continue;
        int ghstsz = ghost.size();
        for (int j = 0; j < ghstsz; j++) {
            if (ghost[j].dead) continue;
            if (ghost[j].r == killedr && ghost[j].c == killedc) {
                ghost[j].dead = true;
                ghost[j].deadTime = time;
                board[killedr][killedc] -= 1;
                deadBoard[killedr][killedc] += 1;
            }
        }
    }
    pacman.r = finishMove.r;
    pacman.c = finishMove.c;

    ghostCnt -= finishMove.kill;
}
void removeGhost(int time) {
    //필요없어보이는데
    
    for (int i = 0; i < ghost.size(); i++) {
        if (ghost[i].dead && (time - ghost[i].deadTime) == 2) {
            int killedr = ghost[i].r;
            int killedc = ghost[i].c;
            deadBoard[killedr][killedc] -= 1;

        }
    }
}
void finishDup() {
    for (int i = 0; i < eggs.size(); i++) {
        Ghost temp;
        temp.r = eggs[i].r;
        temp.c = eggs[i].c;
        temp.dead = false;
        temp.dir = eggs[i].dir;
        board[temp.r][temp.c] += 1;
        ghost.push_back(temp);
    }

    ghostCnt += eggs.size();

    eggs.clear();
}

int main() {
    // Please write your code here.
    int m, t;
    int pr, pc;
    
    cin >> m >> t;
    cin >> pr >> pc;
    
    pacman.r = pr-1;
    pacman.c = pc-1;
    ghostCnt = m;

    for (int i = 0; i < m; i++) {
        int gr, gc, gdir;
        cin >> gr >> gc >> gdir;
        Ghost temp;
        temp.r = gr - 1;
        temp.c = gc - 1;
        temp.dir = gdir - 1;

        board[temp.r][temp.c] += 1;

        ghost.push_back(temp);
    }
    

    for (int i = 0; i < t; i++) {
        ghostDup();
        ghostMove();
        pacMove(i);
        removeGhost(i);
        finishDup();
    }

    cout << ghostCnt;

    return 0;
}