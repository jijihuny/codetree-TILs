#include <iostream>
#include <queue>
#include <map>
#include <vector>

using namespace std;

const int MAX_N = 100000, MAX_M = 100000;
const int MAX_P = 2000;
int Q, P;

typedef unsigned long long bigint;
bigint N = 0ull, M = 0ull;

map<int, int> pid_to_idx;
int pids[MAX_P];
int row[MAX_P];
int col[MAX_P];
int jump[MAX_P];
bigint dist[MAX_P];
bigint score[MAX_P];
bigint moved[MAX_P];
bigint total = 0;

struct QCompare {
    bool operator()(const int &a, const int &b) {
        if(jump[a] != jump[b]) return jump[a] > jump[b];
        else if(row[a] + col[a] != row[b] + col[b])
            return row[a] + col[a] > row[b] + col[b];
        else if(row[a] != row[b]) return row[a] > row[b];
        else if(col[a] != col[b]) return col[a] > col[b];
        else return pids[a] > pids[b];
    }
};

struct KCompare {
    bool operator()(const int &a, const int &b) {
        if(row[a] + col[a] != row[b] + col[b])
            return row[a] + col[a] < row[b] + col[b];
        else if(row[a] != row[b]) return row[a] < row[b];
        else if(col[a] != col[b]) return col[a] < col[b];
        else return pids[a] < pids[b];
    }
};

priority_queue<int, vector<int>, QCompare> qq;
/**
 * 1. 경주 진행
 * 우선순위 토끼 선정
 * O(logP)
 * 새로운 방향
 * O(4)
 * 점수 업데이트
 * O(1)
 * P <= 2000
 * 
 * K(<=100) 번 반복
 * 
 * K번 반복 후 새로운 우선순위 토끼
 * => O(KlogK)
 * 
 * 최대 2000번 명령
 * 
 * 한 번의 명령어마다 O(KlogP)
 * 전체 복잡도는 O(QK(logP + logK))
 * Q * K <= 20000
 * Q * K * (logP + logK) <= 400000
*/
bigint dr[] = {-1ull, 1ull, 0ull, 0ull};
bigint dc[] = {0ull, 0ull, -1ull, 1ull};

bool greater_pos(int ar, int ac, int br, int bc) {
    if(ar + ac != br + bc) return ar + ac > br + bc;
    if(ar != br) return ar > br;
    return ac > bc;
}

priority_queue<int, vector<int>, KCompare> pq;

void progress(const int &K, const int &S) {
    if(!pq.empty()) { pq = {}; }
    for(int i = 0; i < K; i++) {
        const int rabbit = qq.top();
        qq.pop();
        // cout<<rabbit<<" "<< jump[rabbit] <<" "<<row[rabbit] + col[rabbit] <<" "<<row[rabbit]<<" "<<col[rabbit]<<" "<<pids[rabbit]<<" "<<score[rabbit]<<'\n';
        bigint d = dist[rabbit];
        int fr = 0, fc = 0;
        for(int dir = 0; dir < 4; dir++) {
            bigint tr = (bigint)row[rabbit] + dr[dir] * d - 1ull;
            bigint tc = (bigint)col[rabbit] + dc[dir] * d - 1ull;
            // tr *= dr[dir] ? dr[dir] : 1;
            // tc *= dc[dir] ? dc[dir] : 1;
            int nr, nc;
            /**
             * 1 - 1
             * 2 - 2
             * 3 - 3
             * 4 - 4
             * 5 - 5
             * 6 - 4 
             * 7 - 3
             * 8 - 2
             * 9 - 1
             * 10 - 2
             * 11 - 3
             * 12 - 4
             * 13 - 5
            */
           if(tr < 0ull) tr *= -1ull;
           if(tc < 0ull) tc *= -1ull;
            nr = (tr / (N - 1ull)) % 2ull == 0ull ? tr % (N - 1ull) + 1ull : N - tr % (N - 1ull);
            nc = (tc / (M - 1ull)) % 2ull == 0ull ? tc % (M - 1ull) + 1ull : M - tc % (M - 1ull);
            
            if(greater_pos(nr, nc, fr, fc)) {
                fr = nr;
                fc = nc;
            }
        }

        total += fr + fc;
        moved[rabbit] += fr + fc;
        row[rabbit] = fr;
        col[rabbit] = fc;
        jump[rabbit]++;
        qq.push(rabbit);
        pq.push(rabbit);
    }

    const int kr = pq.top();
    score[kr] += S;
}

/**
 * 2. 이동거리 변경
 * O(1)
 * 
 * 최대 O(Q)
*/

void change_dist(const int &pid, const bigint &L) {
    dist[pid_to_idx[pid]] *= L;
}

/**
 * 3. 최고의 토끼 선정
 * 제일 높은 점수 찾기
 * O(P)
*/
int best_rabbit() {
    int bscore = 0;
    for(int i = 0; i < P; i++) {
        if(score[i] + total - moved[i] > bscore) { bscore = score[i] + total - moved[i]; }
    }
    return bscore;
}

/**
 * 따라서 전체 복잡도는
 * O(QK(logP + logK))
*/


int main() {
    // 여기에 코드를 작성해주세요.

    cin >> Q;
    int code;
    for(int i = 0; i < Q; i++) {
        cin >> code;

        switch(code) {
            case 100:
                cin >> N >> M >> P;
                bigint tmp_d;
                for(int r = 0; r < P; r++) {
                    cin >> pids[r] >> tmp_d;
                    dist[r] = tmp_d > (bigint)MAX_N ? MAX_N : (int)tmp_d;
                    pid_to_idx[pids[r]] = r;
                    row[r] = 1;
                    col[r] = 1;
                }

                for(int r = 0; r < P; r++) {qq.push(r);}
                break;
            case 200:
                int K, S;
                cin >> K >> S;
                progress(K, S);
                break;
            case 300:
                int pid;
                bigint L;
                cin >> pid >> L;
                change_dist(pid, L);
                break;
            case 400:
                cout << best_rabbit();
        }
    }
    return 0;
}