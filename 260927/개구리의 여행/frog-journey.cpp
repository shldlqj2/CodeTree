#include <iostream>
#include <vector>
#include <queue>
#include <set>

using namespace std;

struct Frog {
    int r;
    int c;
    int jumpPower;
};

struct Edge {
    pair<int, int> nextNode;
    int power;
    int time;
};

struct EdgeCmp {
    bool operator()(const Edge &a, const Edge &b) const {
        return a.time > b.time;
    }
};

int board[51][51];
pair<int, int> dirs[4] = { {-1,0},{1,0},{0,-1},{0,1} };
vector<Edge> edges[51][51];
int N;

void simulation(int r1, int c1, int r2, int c2) {
    int distmap[51][51][6];
    fill(&distmap[0][0][0], &distmap[0][0][0] + (51 * 51 * 6), 2e9);
    priority_queue<Edge, vector<Edge>, EdgeCmp> pq;
    distmap[r1][c1][1] = 0;
    Edge start;
    start.nextNode = { r1,c1 };
    start.power = 1;
    start.time = 0;
    pq.push(start);


    while (!pq.empty())
    {
        Edge curr = pq.top();
        int cr = curr.nextNode.first;
        int cc = curr.nextNode.second;
        int currtime = curr.time;
        pq.pop();
        if (curr.time > distmap[cr][cc][curr.power]) continue;

        for (const auto& it : edges[cr][cc]) {
            int nr = it.nextNode.first;
            int nc = it.nextNode.second;
            int np = it.power;
            int nexttime=0;

            int tempcurrP = curr.power;

            if (np > curr.power) {
                tempcurrP += 1;
                for (tempcurrP; tempcurrP <= np; tempcurrP++) {
                    nexttime += tempcurrP * tempcurrP;
                }
            }
            else if (np < curr.power) {
                nexttime += 1;
            }
            nexttime++;
            if (distmap[cr][cc][curr.power] + nexttime < distmap[nr][nc][np]) {
                distmap[nr][nc][np] = curr.time + nexttime;
                Edge next;
                next.nextNode = { nr,nc };
                next.power = np;
                next.time = curr.time + nexttime;
                pq.push(next);
            }
        }
    }
    int result = 2e9;
    for (int i = 1; i <= 5; i++) {
        result = min(result, distmap[r2][c2][i]);
    }
    if (result == 2e9) {
        cout << -1 << endl;
        return;
    }
    cout << result << endl;
}

int main() {
    // Please write your code here.
    cin >> N;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            char ch;
            cin >> ch;
            if (ch == '.') {
                board[i][j] = 0;
            }
            else if (ch == 'S') {
                board[i][j] = 1;
            }
            else if (ch == '#') {
                board[i][j] = 2;
            }
        }
    }

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (board[i][j] == 0) {
                for (int m = 0; m < 4; m++) {
                    for (int k = 1; k <= 5; k++) {
                        int nr = i + (k*dirs[m].first);
                        int nc = j + (k*dirs[m].second);
                        if (1 <= nr && nr <= N && 1 <= nc && nc <= N) {
                            if (board[nr][nc] == 2) {
                                break;
                            }
                            else if (board[nr][nc] == 0) {
                                Edge temp;
                                temp.nextNode = { nr,nc };
                                temp.power = k;
                                temp.time = 0;
                                edges[i][j].push_back(temp);
                            }
                        }
                    }
                }
            }
        }
    }



    int Q;
    cin >> Q;
    for (int i = 0; i < Q; i++) {
        int r1, r2, c1, c2;
        cin >> r1 >> c1 >> r2 >> c2;
        simulation(r1, c1, r2, c2);
    }
    return 0;
}