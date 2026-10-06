#include <iostream>
#include <algorithm>
#include <cmath>
#include <queue>
#include <vector>



using namespace std;

struct Whale {
    int r;
    int c;
    int direction;
    int move;
};

int N;
//pair<int, int> dirs[] = { {-1,0},{1,0},{0,-1},{0,1} };
pair<int, int> dirs[] = { {-1,0},{0,-1},{1,0},{0,1} }; //상 좌 하 우 반시계 90도
pair<int, int> dirs2[] = { {0,-1},{1,0},{0,1},{-1,0} }; //좌 하 우 상 MoveNear용
int board[50][50];
Whale whale;
bool finishFlag = false;
int visited[50][50];
int CanMoveCnt=0;


Whale MoveNearSea(Whale start) {
    Whale result;
    queue<Whale> q;

    start.move = 0;
    q.push(start);

    Whale tempMoves[50][50];
    bool tempVisited[50][50];
    vector<Whale> hubos;


    fill(&tempVisited[0][0], &tempVisited[0][0] + (50 * 50), false);
    tempMoves[start.r][start.c] = start; //이것도 필요 없어 보이는데
    tempVisited[start.r][start.c] = true;


    while (!q.empty())
    {
        Whale curr = q.front();
        q.pop();
        if (!visited[curr.r][curr.c]) {
            hubos.push_back(curr);
            continue;
        }
        for (int i = 0; i < 4; i++) {
            int nr = curr.r + dirs2[i].first;
            int nc = curr.c + dirs2[i].second;
            if (0 <= nr && nr < N && 0 <= nc && nc < N && 
                !tempVisited[nr][nc] && board[nr][nc] == 0) {

                tempVisited[nr][nc] = true;
                Whale next;
                next.r = nr;
                next.c = nc;
                next.direction = i;
                next.move = curr.move + 1;
                q.push(next);
            }
        }
    }

    if (hubos.empty()) { //후보 없다는 건 이동 불가라는 거
        result.r = -1;
        result.c = -1;
        return result;
    }
    else {
        sort(hubos.begin(), hubos.end(), [](const Whale &a, const Whale &b) {
            if (a.move == b.move) {
                if (a.r == b.r) {
                    return a.c < b.c;
                }
                return a.r < b.r;
            }
            return a.move < b.move;
        });
        result = hubos[0];
        int resultdir = result.direction;
        switch (resultdir) {
        case 0:
            result.direction = 1;
            break;
        case 1:
            result.direction = 2;
            break;
        case 2:
            result.direction = 3;
            break;
        case 3:
            result.direction = 0;
            break;
        }

        return result;
    }
}

void WhaleMove() {
    queue<Whale> q;
    q.push(whale);
    visited[whale.r][whale.c] = true;

    while (!q.empty())
    {
        Whale curr;
        bool flag = false;
        curr = q.front();
        q.pop();

        int nr = curr.r + dirs[curr.direction].first;
        int nc = curr.c + dirs[curr.direction].second;
        if (0 <= nr && nr < N && 0 <= nc && nc < N) {
            if (!visited[nr][nc] && board[nr][nc] == 0) {
                visited[nr][nc] = true;
                Whale next;
                next.r = nr;
                next.c = nc;
                next.direction = curr.direction;
                q.push(next);
                cout << nr + 1 << " " << nc + 1 << endl;
            }
            else {
                flag = true;
            }
        }
        else {
            flag = true;
        }

        if (flag) {
            int turnCnt = 0;
            int nextDir = curr.direction;
            while (turnCnt < 3) {
                switch (turnCnt)
                {
                case 0:
                    nextDir += 1;
                    break;
                case 1:
                    nextDir += 2;
                    break;
                case 2:
                    nextDir += 3;
                    break;
                default:
                    break;
                }
                nextDir = nextDir % 4;

                nr = curr.r + dirs[nextDir].first;
                nc = curr.c + dirs[nextDir].second;

                if (0 <= nr && nr < N && 0 <= nc && nc < N && 
                    !visited[nr][nc] && board[nr][nc] == 0) {
                    visited[nr][nc] = true;
                    Whale next;
                    next.r = nr;
                    next.c = nc;
                    next.direction = nextDir;
                    q.push(next);
                    cout << nr + 1 << " " << nc + 1 << endl;
                    break;
                }

                turnCnt++;
            }

            if (turnCnt == 3) {
                Whale next = MoveNearSea(curr);
                if (next.r == -1 && next.c == -1) {
                    //그냥 끝임 더 이상 이동 가능 한 곳 없음
                    return;
                }
                else {
                    visited[next.r][next.c] = true;
                    cout << next.r + 1 << " " << next.c + 1 << endl;
                    q.push(next);
                }
            }
        }
    }

}

int main(void) {
    cin >> N >> whale.r >> whale.c >> whale.direction;
    whale.move = 0;
    whale.r -= 1;
    whale.c -= 1;

    //여기 방향 수정 해야함 
    //상 좌 하 우 반시계 90도
    int direction = whale.direction;
    switch (direction)
    {
    case 1:
        whale.direction = 0;
        break;
    case 2:
        whale.direction = 2;
        break;
    case 3:
        whale.direction = 1;
        break;
    case 4:
        whale.direction = 3;
        break;
    default:
        break;
    }

    cout << whale.r + 1 << " " << whale.c + 1 << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> board[i][j];
        }
    }
    WhaleMove();

    return 0;
}