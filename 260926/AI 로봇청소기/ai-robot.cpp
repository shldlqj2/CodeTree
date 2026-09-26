#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int board[31][31];
int dusts;
bool robotRC[31][31];
pair<int, int> dirs[] = { {1,0},{0,1},{-1,0},{0,-1} };
int N, K, L;
//0이면 먼지 X 양수면 먼지 수 -1이면 벽 또는 물건
//먼지가 있으면 1~100 먼지를가짐

struct Robot{
    int r;
    int c;
};

struct queueMem {
    int r;
    int c;
    int move;
};
struct queSort
{
    bool operator()(const queueMem &a, const queueMem &b) const {
        if (a.move == b.move) {
            if (a.r == b.r) {
                return a.c > b.c;
            }
            return a.r > b.r;
        }
        return a.move > b.move;
    }
};

struct dustMem {
    int removes;
    int dirs;
};

struct dustMemCmp {
    bool operator()(const dustMem &a, const dustMem &b) const {
        if (a.removes == b.removes) {
            return a.dirs < b.dirs;
        }
        return a.removes < b.removes;
    }
};

vector<Robot> robots;


/*
로봇청소기는 "순서대로" "이동거리가 가장 가까운" 오염 격자로 이동
물건이 있거나 청소기가 있는 곳 지나갈 수 없음
이동거리는 상하좌우로 1칸씩이동 최소 이동거리
가까운게 여러개면 행번호 작은격자, 행번호가 같을 경우 열번호 작은 격자
*/
void moveRobot() {
    for (int i = 0; i < K; i++) {
        queue<queueMem> q;
        priority_queue<queueMem,vector<queueMem>,queSort> pq;
        queueMem start;
        start.r = robots[i].r;
        start.c = robots[i].c;
        start.move = 0;
        q.push(start);

        vector<vector<int>> visited(N+1,vector<int>(N+1,0));
        visited[start.r][start.c] = 1;
        

        if (board[start.r][start.c] > 0) {
            queueMem nst;
            nst.r = start.r;
            nst.c = start.c;
            nst.move = 0;
            pq.push(nst);
        }

        while (!q.empty()) {
            queueMem curr;
            curr = q.front();
            q.pop();

            if (!pq.empty() && curr.move >= pq.top().move) {
                continue;
            }

            for (int k = 0; k < 4; k++) {
                int nr = curr.r + dirs[k].first;
                int nc = curr.c + dirs[k].second;
                if (1 <= nr && nr <= N && 1 <= nc && nc <= N && !visited[nr][nc] && board[nr][nc]!=-1 && !robotRC[nr][nc]) {
                    
                    queueMem next;
                    next.r = nr;
                    next.c = nc;
                    next.move = curr.move + 1;
                    visited[nr][nc] = 1;
                    if (board[nr][nc] > 0) {
                        pq.push(next);
                    }
                    q.push(next);
                }
            }
        }

        
        if (pq.empty()) continue;

        robotRC[start.r][start.c] = false;
        robots[i].r = pq.top().r;
        robots[i].c = pq.top().c;

        robotRC[robots[i].r][robots[i].c] = true;
    }
}

/*
바라 보고 있는 방향 기준으로 자기 위치, 왼쪽, 위쪽, 오른쪽 격자 청소
청소할 수 있는 4가지 격자에서 청소할 수 있는 먼지량이 가장 큰 방향에서 청소를 시작합니다.
"격자마다" 청소할 수 있는 "최대 먼지량은 20"입니다.
합이 같은 방향이 여러개인 경우, 오른쪽, 아래쪽, 왼쪽, 위쪽 방향의 우선순위로 방향을 선택합니다.
청소는 청소기마다 순서대로 진행됩니다.
*/
void cleanBoard() {
    for (int i = 0; i < K; i++) {
        Robot curr;
        curr = robots[i];
        int fiveDir = board[curr.r][curr.c];
        //priority_queue<pair<int, int>> dirDusts;
        priority_queue<dustMem, vector<dustMem>, dustMemCmp> dirDusts;

        for (int k = 0; k < 4; k++) {
            int nr = curr.r + dirs[k].first;
            int nc = curr.c + dirs[k].second;

            if (1 <= nr && nr <= N && 1 <= nc && nc <= N && board[nr][nc] != -1) {

                fiveDir += min(board[nr][nc], 20);
                
            }
        }

        for (int k = 0; k < 4; k++) {
            int nr = curr.r + dirs[k].first;
            int nc = curr.c + dirs[k].second;

            if (1 <= nr && nr <= N && 1 <= nc && nc <= N) {
                dustMem temp;
                if (board[nr][nc] == -1) {
                    temp.removes = fiveDir;
                }
                else {
                    temp.removes = fiveDir - min(board[nr][nc], 20);
                }
                temp.dirs = k;
                dirDusts.push(temp);
            }
            else if (1 > nr || nr > N || 1 > nc || nc > N) {
                dustMem temp;
                temp.removes = fiveDir;
                temp.dirs = k;
                dirDusts.push(temp);
            }
        }
        
        int passDir = dirDusts.top().dirs;


        dusts -= min(board[curr.r][curr.c], 20);
        board[curr.r][curr.c] = max(board[curr.r][curr.c] - 20, 0);

        for (int k = 0; k < 4; k++) {
            if (k == passDir) continue;

            int nr = curr.r + dirs[k].first;
            int nc = curr.c + dirs[k].second;

            if (1 <= nr && nr <= N && 1 <= nc && nc <= N && board[nr][nc] != -1) {
                dusts -= min(board[nr][nc], 20);
                board[nr][nc] = max(board[nr][nc] - 20, 0);
            }
        }
        
    }
}

void addDust() {
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (board[i][j] > 0) {
                board[i][j] += 5;
                dusts += 5;
            }
        }
    }
}

void spreadDust() {
    int tempBoard[31][31];
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            tempBoard[i][j] = board[i][j];
        }
    }

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (tempBoard[i][j] == 0) {
                
                int spreading = 0;

                for (int k = 0; k < 4; k++) {
                    int nr = i + dirs[k].first;
                    int nc = j + dirs[k].second;
                    if (1 <= nr && nr <= N && 1 <= nc && nc <= N && tempBoard[nr][nc] != -1) {
                        spreading += tempBoard[nr][nc];
                    }
                }

                spreading = spreading / 10;
                board[i][j] += spreading;
                dusts += spreading;
            }
        }
    }
}

int main() {
    // Please write your code here.
    

    cin >> N >> K >> L;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> board[i][j];
            if (board[i][j] > 0) {
                dusts += board[i][j];
            }
        }
    }

    for (int i = 0; i < K; i++) {
        Robot temp;
        cin >> temp.r >> temp.c;
        robotRC[temp.r][temp.c] = true;
        robots.push_back(temp);
    }

    for (int i = 0; i < L; i++) {
        moveRobot();

        cleanBoard();

        addDust();

        spreadDust();

        cout << dusts << endl;
    }

    

    return 0;
}