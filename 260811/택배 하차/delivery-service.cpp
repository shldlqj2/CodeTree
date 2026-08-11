#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

typedef struct tackbae {
    int k;
    int h;
    int w;
    int c;
    int bottomh;
    bool exited = false;
};

void gravity(void);
void removeleft(void);
void removeright(void);

int n;
int m;
vector<vector<int>> map;
vector<tackbae> tbs;

void gravity(void) {
    int size = tbs.size();
    for (int i = 0; i < size; i++) {
        if (tbs[i].exited) continue;

        int newbottom = tbs[i].bottomh;
        queue<pair<int, int>> q;
        for (int c = tbs[i].c; c < tbs[i].c + tbs[i].w; c++) {
            q.push({ tbs[i].bottomh, c });//y,x
        }
        while (!q.empty()) {
            pair<int, int> xys;
            xys = q.front();
            q.pop();

            int nr, nc;
            nr = xys.first+1;
            nc = xys.second;

            if (nr < n) {
                if (map[nr][nc] == 0) {
                    if (nr > newbottom) {
                        newbottom = nr;
                    }
                    q.push({ nr,nc });
                }
                else {
                    newbottom = nr - 1;
                    break;
                }
            }
        }
        if (tbs[i].bottomh != newbottom) {
            for (int r = tbs[i].bottomh; r > tbs[i].bottomh - tbs[i].h; r--) {
                for (int c = tbs[i].c; c < tbs[i].c + tbs[i].w; c++) {
                    map[r][c] = 0;
                }
            }

            tbs[i].bottomh = newbottom;

            for (int r = tbs[i].bottomh; r > tbs[i].bottomh - tbs[i].h; r--) {
                for (int c = tbs[i].c; c < tbs[i].c + tbs[i].w; c++) {
                    map[r][c] = tbs[i].k;
                }
            }
            gravity();
            return;
        }
    }
}

void removeleft(void) {
    int size = tbs.size();
    for (int i = 0; i < size; i++) {
        if (tbs[i].exited) continue;
        queue<pair<int, int>> q;
        
        for (int j = tbs[i].bottomh; j > tbs[i].bottomh - tbs[i].h; j--) {
            q.push({ j,tbs[i].c }); //y,x
        }
        int cnt = 0;

        while (!q.empty()) {
            pair<int, int> xys;
            xys = q.front();
            q.pop();
            
            int nr, nc;
            nr = xys.first;
            nc = xys.second - 1;
            if (0 <= nc) {
                if (map[nr][nc] == 0) {
                    q.push({ nr,nc });
                }
                else {
                    break;
                }
            }
            if (nc == -1) {
                cnt++;
            }
        }
        if (cnt == tbs[i].h) {
            for (int r = tbs[i].bottomh; r > tbs[i].bottomh - tbs[i].h; r--) {
                for (int c = tbs[i].c; c < tbs[i].c + tbs[i].w; c++) {
                    map[r][c] = 0;
                }
            }
            tbs[i].exited = true;
            cout << tbs[i].k << endl;
            gravity();
            return;
        }
    }
}

void removeright(void) {
    int size = tbs.size();
    for (int i = 0; i < size; i++) {
        if (tbs[i].exited) continue;
        queue<pair<int, int>> q;

        for (int j = tbs[i].bottomh; j > tbs[i].bottomh - tbs[i].h; j--) {
            q.push({ j,tbs[i].c + tbs[i].w - 1 }); //y,x
        }
        int cnt=0;

        while (!q.empty()) {
            pair<int, int> xys;
            xys = q.front();
            q.pop();

            int nr, nc;
            nr = xys.first;
            nc = xys.second + 1;
            if (nc < n) {
                if (map[nr][nc] == 0) {
                    q.push({ nr,nc });
                }
                else {
                    break;
                }
            }
            if (nc == n) {
                cnt++;
            }
        }
        if (cnt==tbs[i].h) {
            for (int r = tbs[i].bottomh; r > tbs[i].bottomh - tbs[i].h; r--) {
                for (int c = tbs[i].c; c < tbs[i].c + tbs[i].w; c++) {
                    map[r][c] = 0;
                }
            }
            tbs[i].exited = true;
            cout << tbs[i].k << endl;
            gravity();
            return;
        }
    }
}

int main() {
    cin >> n >> m;
    map = vector<vector<int>>(n, vector<int>(n, 0));

    for (int i = 0; i < m; i++) {
        tackbae temp;
        cin >> temp.k >> temp.h >> temp.w >> temp.c;
        
        temp.c -= 1;
        temp.bottomh = temp.h-1;

        tbs.push_back(temp);


        for (int y = temp.h-1; y >= 0; y--) {
            for (int x = temp.c; x < temp.c + temp.w; x++) {
                map[y][x] = temp.k;
            }
        }

        gravity();
    }
    sort(tbs.begin(), tbs.end(), [](const tackbae &a, const tackbae &b) {
        return a.k < b.k;
    });

    while (true) {
        int exited = 0;
        for (int i = 0; i < m; i++) {
            if (tbs[i].exited) exited++;
        }
        if (exited == m) {
            break;
        }
        
        removeleft();
        removeright();
    }

    return 0;
}

/*
for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << map[i][j];
            }
            cout << endl;
        }*/