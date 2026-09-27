#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

vector<int> house(1,0);
vector<bool> alive(1,true);

void buildMaeul() {
    int N;
    cin >> N;

    for (int i = 0; i < N; i++) {
        int x;
        cin >> x;
        house.push_back(x);
        alive.push_back(true);
    }
}

void buildAntHouse() {
    int x;
    cin >> x;
    house.push_back(x);
    alive.push_back(true);
}

void removeAntHouse() {
    int target;
    cin >> target;
    alive[target] = false;
}

bool check(vector<int> &v, int r, int time) {
    if (v.empty()) return true;
    int antCnt = 1;
    int start = v[0];
    for (int i = 0; i < v.size(); i++) {
        if (v[i] - start > time) {
            antCnt++;
            start = v[i];

            if (antCnt > r) return false;
        }
    }

    return true;
}

void patrolHouse() {
    int r;
    cin >> r;
    vector<int> v;
    for (int i = 1; i < house.size(); i++) {
        if (alive[i]) v.push_back(house[i]);
    }
    int left = 0;
    int right = v.back() - v.front();
    int answer = right;

    while (left <= right) {
        int mid = (right + left) / 2;
        
        if (check(v,r,mid)) {
            answer = mid;
            right = mid - 1;
        }
        else {
            left = mid+1;
        }
    }

    cout << answer << endl;
}

int main() {
    // Please write your code here.
    int Q;
    cin >> Q;
    for (int i = 0; i < Q; i++) {
        int command;
        cin >> command;
        switch (command)
        {
        case 100:
            buildMaeul();
            break;
        case 200:
            buildAntHouse();
            break;
        case 300:
            removeAntHouse();
            break;
        case 400:
            patrolHouse();
            break;

        default:
            break;
        }
    }
    return 0;
}