#include <iostream>
#include <queue>
#include <cmath>

using namespace std;

/**
 *  N * M 격자
 *  모든 위치 포탑 존재
 *  if attack <= 0 then portress broken;
 * 하나의 턴은 4 액션, K 턴
 * 멀쩡한 포탑 1 => game done
 * 
 * N, M <= 10
 * K <= 1000
 * 공격력 <= 5000
 * 
*/


/**
 * 1. 공격자 선정
 * 
 * O(N)
 * let weak
 * for turret of turrets
 *      
 *  
*/
int N, M, K;
int attack[10][10]{};
int last[10][10]{};
int dead[10][10]{};

bool greaterThan(const int &r1, const int &c1, const int &r2, const int &c2) {
    if(attack[r1][c1] != attack[r2][c2]) return attack[r1][c1] > attack[r2][c2];
    if(last[r1][c1] != last[r2][c2]) return last[r1][c1] < last[r2][c2];
    if(r1 + c1 != r2 + c2) return r1 + c1 < r2 + c2;
    return c1 < c2;
}

bool lessThan(const int &r1, const int &c1, const int &r2, const int &c2) {
    if(attack[r1][c1] != attack[r2][c2]) return attack[r1][c1] < attack[r2][c2];
    if(last[r1][c1] != last[r2][c2]) return last[r1][c1] > last[r2][c2];
    if(r1 + c1 != r2 + c2) return r1 + c1 > r2 + c2;
    return c1 > c2;
}

vector<int> select_strong_weak() {
    int strong_row, strong_col;
    int weak_row, weak_col;

    strong_row = strong_col = weak_row = weak_col = -1;

    for(int r = 0; r < N; r++) {
        for(int c = 0; c < M; c++) {
            if(dead[r][c]) { continue; }
            if(strong_row == -1) {
                strong_row = weak_row = r;
                strong_col = weak_col = c;
                continue;
            }

            if(lessThan(r, c, weak_row, weak_col)) {
                weak_row = r;
                weak_col = c;
            }

            if(greaterThan(r, c, strong_row, strong_col)) {
                strong_row = r;
                strong_col = c;
            }
        }
    }

    attack[weak_row][weak_col] += N + M;

    return {weak_row, weak_col, strong_row, strong_col};
}

queue<pair<int, int>> q;

int dr[] = {0, 1, 0, -1};
int dc[] = {1, 0, -1, 0};

int dp[10][10]{};
int dir[10][10]{};
bool visited[10][10]{};

int razor_attack(const int &weak_row, const int &weak_col, const int &strong_row, const int &strong_col) {
    int nr, nc;

    fill(dp[0], dp[N-1] + M, 1000);
    fill(dir[0], dir[N - 1] + M, 5);
    dp[strong_row][strong_col] = 0;
    for(int i = 0; i < 4; ++i) {
        nr = (strong_row - dr[i]) % N;
        if(nr < 0) nr += N;
        nc = (strong_col - dc[i]) % M;
        if(nc < 0) nc += M;

        if(dead[nr][nc]) { continue; }

        dp[nr][nc] = 1;
        dir[nr][nc] = i;
        q.push({nr, nc});
    }

    int cr, cc;
    while(!q.empty()) {
        cr = q.front().first;
        cc = q.front().second;
        q.pop();

        for(int i = 0; i < 4; i++) {
            nr = (cr - dr[i]) % N;
            if(nr < 0) nr += N;
            nc = (cc - dc[i]) % M;
            if(nc < 0) nc += M;

            if(dead[nr][nc]) { continue; }

            if(
                dp[nr][nc] > dp[cr][cc] + 1 ||
                dp[nr][nc] == dp[cr][cc] + 1 &&
                i < dir[nr][nc]
            ) {
                dp[nr][nc] = dp[cr][cc] + 1;
                dir[nr][nc] = i;
                q.push({nr, nc});
            }
        }    
    }

    if(dp[weak_row][weak_col] == 1000) return -1;

    int d = dir[weak_row][weak_col];

    nr = weak_row;
    nc = weak_col;

    int att = attack[nr][nc];
    int half = att / 2;


    nr = (nr + dr[d]) % N;
    if(nr < 0) nr += N;
    nc = (nc + dc[d]) % M;
    if(nc < 0) nc += M;
    d = dir[nr][nc];

    while(nr != strong_row || nc != strong_col) {
        attack[nr][nc] = max(attack[nr][nc] - half, 0);
        if(attack[nr][nc] == 0) {
            dead[nr][nc] = true;
        }

        visited[nr][nc] = true;
        nr = (nr + dr[d]) % N;
        if(nr < 0) nr += N;
        nc = (nc + dc[d]) % M;
        if(nc < 0) nc += M;
        d = dir[nr][nc];
    }
    
    attack[nr][nc] = max(attack[nr][nc] - att, 0);
    
    if(attack[nr][nc] == 0) {
        dead[nr][nc] = true;
    }
    visited[nr][nc] = true;

    return 0;
}

int ar[] = {-1, -1, 0, 1, 1, 1, 0, -1};
int ac[] = {0, 1, 1, 1, 0, -1, -1, -1};

void attack_bomb(const int &weak_row, const int &weak_col, const int &strong_row, const int &strong_col) {
    int att = attack[weak_row][weak_col];
    int half = att / 2;
    int nr = strong_row, nc = strong_col;
    attack[nr][nc] = max(attack[nr][nc] - att, 0);
    if(attack[nr][nc] == 0) {
        dead[nr][nc] = true;
    }
    visited[nr][nc] = true;

    for(int i = 0; i < 8; i++) {
        nr = (strong_row + ar[i]) % N;
        if(nr < 0) nr += N;
        nc = (strong_col + ac[i]) % M;
        if(nc < 0) nc += M;

        if(dead[nr][nc]) { continue; }

        attack[nr][nc] = max(attack[nr][nc] - half, 0);
        if(attack[nr][nc] == 0) {
            dead[nr][nc] = true;
        }
        visited[nr][nc] = true;
    }

    attack[strong_row][strong_col] = max(attack[strong_row][strong_col] - att, 0);
    if(attack[strong_row][strong_col] == 0) {
        dead[strong_row][strong_col] = true;
    }
    visited[strong_row][strong_col]  = true;
}

void progress(const int &turn) {
    auto list = select_strong_weak();

    const int weak_row = list[0];
    const int weak_col = list[1];
    const int strong_row = list[2];
    const int strong_col = list[3];

    fill(visited[0], visited[N - 1] + M, false);

    if(razor_attack(weak_row, weak_col, strong_row, strong_col) < 0) {
        attack_bomb(weak_row, weak_col, strong_row, strong_col);
    }

    visited[weak_row][weak_col] = true;
    last[weak_row][weak_col] = turn;

    for(int r = 0; r < N; r++) {
        for(int c = 0; c < M; c++) {
            if(dead[r][c] || visited[r][c]) { continue; }

            attack[r][c]++;
        }
    }
}

int main() {
    cin >> N >> M >> K;

    for(int r = 0; r < N; r++) {
        for(int c = 0; c < M; c++) {
            cin >> attack[r][c];
            if(attack[r][c] <= 0) {
                dead[r][c] = true;
            }
        }
    }

    for(int turn = 0; turn < K; turn++) {
        progress(turn);
    }

    auto result = select_strong_weak();

    cout << attack[result[2]][result[3]] << endl;
}