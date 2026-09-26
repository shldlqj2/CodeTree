#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int Q;
long long N;
int M;

struct Garodeung {
    long long l = 0;

    int leftid = -1;
    int rightid = -1;

    bool removed = false;
};

struct QueueMem {
    int leftid;
    int rightid;

    long long len;
    long long leftPos;
};

struct QueueMemCmp {
    bool operator()(const QueueMem& a, const QueueMem& b) const {

        // 거리 긴 것이 우선
        if (a.len != b.len) {
            return a.len < b.len;
        }

        // 거리가 같으면 왼쪽 좌표가 작은 것이 우선
        return a.leftPos > b.leftPos;
    }
};

vector<Garodeung> garodeung;

priority_queue<
    QueueMem,
    vector<QueueMem>,
    QueueMemCmp
> pq;

int headid;
int tailid;
int nextId;


// 현재 PQ의 구간이 실제로 유효한지 검사
bool isValid(const QueueMem& curr) {

    int leftid = curr.leftid;
    int rightid = curr.rightid;

    if (leftid < 0 || rightid < 0) {
        return false;
    }

    if (garodeung[leftid].removed ||
        garodeung[rightid].removed) {
        return false;
    }

    // 현재도 실제 인접한 가로등이어야 함
    if (garodeung[leftid].rightid != rightid) {
        return false;
    }

    if (garodeung[rightid].leftid != leftid) {
        return false;
    }

    return true;
}


// PQ 맨 위의 무효 구간 제거
void cleanPQ() {

    while (!pq.empty() && !isValid(pq.top())) {
        pq.pop();
    }
}


// 두 가로등 사이 구간 PQ 삽입
void pushInterval(int leftid, int rightid) {

    if (leftid == -1 || rightid == -1) {
        return;
    }

    QueueMem temp;

    temp.leftid = leftid;
    temp.rightid = rightid;

    temp.len =
        garodeung[rightid].l
        - garodeung[leftid].l;

    temp.leftPos =
        garodeung[leftid].l;

    pq.push(temp);
}


void initGarodeung() {

    cin >> N >> M;

    // 최대 추가 개수는 Q개 이하
    garodeung.assign(
        M + Q + 5,
        Garodeung()
    );

    while (!pq.empty()) {
        pq.pop();
    }

    headid = 1;
    tailid = M;

    nextId = M + 1;


    for (int id = 1; id <= M; id++) {

        cin >> garodeung[id].l;

        if (id == 1) {
            garodeung[id].leftid = -1;
        }
        else {
            garodeung[id].leftid = id - 1;
        }

        if (id == M) {
            garodeung[id].rightid = -1;
        }
        else {
            garodeung[id].rightid = id + 1;
        }

        garodeung[id].removed = false;
    }


    // 초기 인접 구간 등록
    for (int id = 1; id < M; id++) {

        pushInterval(
            id,
            id + 1
        );
    }
}


void addGarodeung() {

    cleanPQ();

    // 문제 조건상 보통 발생하지 않지만 안전 처리
    if (pq.empty()) {
        return;
    }


    // 가장 긴 구간
    // 동일 길이면 왼쪽 좌표가 가장 작은 구간
    QueueMem curr = pq.top();
    pq.pop();


    int leftid = curr.leftid;
    int rightid = curr.rightid;

    int newId = nextId++;


    // ceil((L + R) / 2)
    long long newPos =
        (garodeung[leftid].l+ garodeung[rightid].l+ 1) / 2;


    garodeung[newId].l = newPos;

    garodeung[newId].leftid = leftid;
    garodeung[newId].rightid = rightid;

    garodeung[newId].removed = false;


    // left <-> new <-> right

    garodeung[leftid].rightid = newId;
    garodeung[rightid].leftid = newId;


    // 새로운 두 구간 등록
    pushInterval(
        leftid,
        newId
    );

    pushInterval(
        newId,
        rightid
    );
}


void removeGarodeung() {

    int target;
    cin >> target;


    int leftid =
        garodeung[target].leftid;

    int rightid =
        garodeung[target].rightid;


    garodeung[target].removed = true;


    /*
        left - target - right

        target 삭제 후

        left -------- right
    */


    // target이 head
    if (leftid == -1) {
        headid = rightid;
    }
    else {

        garodeung[leftid].rightid =    rightid;
    }


    // target이 tail
    if (rightid == -1) {

        tailid = leftid;
    }
    else {

        garodeung[rightid].leftid = leftid;
    }


    if (leftid != -1 &&
        rightid != -1) {

        pushInterval(
            leftid,
            rightid
        );
    }
}


void calculateLight() {

    cleanPQ();

    long long answer =
        (garodeung[headid].l - 1) * 2;


    answer = max(answer,(N - garodeung[tailid].l) * 2);

    if (!pq.empty()) {
        answer = max(
            answer,
            pq.top().len
        );
    }


    cout << answer << '\n';
}


int main() {


    cin >> Q;


    for (int q = 0; q < Q; q++) {

        int command;
        cin >> command;


        if (command == 100) {

            initGarodeung();
        }
        else if (command == 200) {

            addGarodeung();
        }
        else if (command == 300) {

            removeGarodeung();
        }
        else if (command == 400) {

            calculateLight();
        }
    }


    return 0;
}