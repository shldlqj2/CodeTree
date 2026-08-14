#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int n;
int tot = 0;
int dr[4] = { 1,-1,0,0 };
int dc[4] = { 0,0,1,-1 };
int idx = 0;
int groupmap[29][29] = { 0 };
int groupcnts[4][1000] = { 0 };

void rotate90(int mid, int startx, int endx, int starty, int endy, vector<vector<int>> &artmap) {
    vector<vector<int>> temp = artmap;

    for (int i = 0; i < mid; i++) {
        for (int j = 0; j < mid; j++) {
            artmap[starty + i][startx + j] = temp[endy - 1 - j][startx + i];
        }
    }

}

void rotate270(int mid, vector<vector<int>> &artmap) {
    vector<vector<int>> temp = artmap;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == mid || j == mid) {
                artmap[i][j] = temp[j][n - 1 - i]; //0,0 <- 0,2   0,1 <- 1,2  0,2 <- 2,2
            }
        }
    }
}

int findart(const vector<vector<int>> &artmap) {
    int visited[29][29] = { 0 };
    int groupnum = 0;
    int resultsum = 0;


    //그루핑 그룹별로 몇개 있는지 카운팅 필요
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!visited[i][j]) {
                queue<pair<int, int>> q;
                q.push(pair<int, int>{i, j}); // y,x
                visited[i][j] = 1;
                groupmap[i][j] = groupnum;
                int cnt = 0;

                
                while (!q.empty()) {
                    int cr = q.front().first, cc = q.front().second;
                    q.pop();

                    for (int k = 0; k < 4; k++) {
                        int nr = cr + dr[k];
                        int nc = cc + dc[k];
                        if (nr >= 0 && nr < n && nc >= 0 && nc < n && !visited[nr][nc]) {
                            if (artmap[nr][nc] == artmap[cr][cc]) {
                                q.push(pair<int, int>{nr, nc});
                                visited[nr][nc] = 1;
                                groupmap[nr][nc] = groupnum;
                            }
                            else {
                                continue;
                            }
                        }
                    }

                    cnt++;
                }
                groupcnts[idx][groupnum] = cnt;
                groupnum++;
            }
        }
    }
    //그루핑, 그룹별 개수 카운팅 끝

    //이제 얼마나 변이 겹치는지? 확인 필요

    
    
    int visitedByeon[29][29] = { 0 };
    vector<vector<int>> byeons(1000,vector<int>(1000,0));
    

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!visitedByeon[i][j]) {
                queue<pair<int, int>> q;
                vector<pair<int, int>> backtrack;
                q.push(pair<int, int>{i, j}); // y,x
                visitedByeon[i][j] = 1;
                int cnt = 0;
                

                while (!q.empty()) {
                    int cr = q.front().first, cc = q.front().second;
                    q.pop();

                    for (int k = 0; k < 4; k++) {
                        int nr = cr + dr[k];
                        int nc = cc + dc[k];
                        if (nr >= 0 && nr < n && nc >= 0 && nc < n && !visitedByeon[nr][nc]) {
                            if (groupmap[nr][nc] == groupmap[cr][cc]) {
                                q.push(pair<int, int>{nr, nc});
                                visitedByeon[nr][nc] = 1;

                            }
                            else {
                                byeons[groupmap[cr][cc]][groupmap[nr][nc]]++;
                                backtrack.push_back(pair<int, int>{nr,nc});
                                continue;
                            }
                        }
                    }


                }
                int currsum = 0;
                int btsz = backtrack.size();
                for (int k = 0; k < groupnum; k++) {
                    if (!byeons[groupmap[i][j]][k]) continue;
                    int basecnt = groupcnts[idx][groupmap[i][j]];
                    int targetcnt = 0;
                    int basenum=artmap[i][j];
                    int targetnum=0;
                    for (int m = 0; m < btsz; m++) {
                        if (groupmap[backtrack[m].first][backtrack[m].second]== k) {
                            targetcnt = groupcnts[idx][groupmap[backtrack[m].first][backtrack[m].second]];
                            targetnum = artmap[backtrack[m].first][backtrack[m].second];
                            break;
                        }
                    }
                    currsum += byeons[groupmap[i][j]][k] * (basecnt+targetcnt) * basenum * targetnum;
                }
                resultsum += currsum;
            }
        }
    }

    /*cout << "artmap" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << artmap[i][j];
        }
        cout << endl;
    }

    
    cout << "groupmap" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << groupmap[i][j];
        }
        cout << endl;
    }*/
    
    return resultsum;
}


int main(void) {
    cin >> n;

    int mid = n / 2;
    vector<vector<int>> artmap(n,vector<int>(n,0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> artmap[i][j];
        }
    }

    for (int i = 0; i < 4; i++) {
        tot += findart(artmap);
        
        rotate90(mid,0, mid, 0, mid, artmap); //startx, endx, starty, endy
        rotate90(mid,mid + 1, n, 0, mid, artmap);
        rotate90(mid,0, mid, mid + 1, n, artmap);
        rotate90(mid,mid + 1, n, mid + 1, n, artmap);

        rotate270(mid, artmap);

        idx++;
    }

    cout << tot;
}