#include <iostream>

using namespace std;

/**
 * 
 * L: 체스판의 크기 (3≤L≤40)
 * N: 기사의 수 (1≤N≤30)
 * Q: 명령의 수 (1≤Q≤100)
 * k: 기사의 체력 (1≤k≤100)
 * 
 * r, c, h, w, k
 * 
 * death[N] 죽음 여부(k <= 0)
 * trap[N] 기사 위의 함정 개수
 * board[L][L]
 * EMPTY 0
 * DEAMAGE 1
 * WALL 2
 * 
 * 
 * 
 * 1. 기사 이동
 * 
 * 상 우 하 좌 0 1 2 3
 * 
 * 
 * O(max(h, w)*N) 최대 40 * 30 = 12000
 * trap[i] = trap[i] - trap[prev col or row] + trap[new col or row]
*/

const int MAX_L = 40;
const int MAX_N = 30;

const int EMPTY = 0;
const int DAMAGE = 1;
const int WALL = 2;

const int UP    = 0;
const int RIGHT = 1;
const int DOWN  = 2;
const int LEFT  = 3;
int L, N, Q;

bool death[MAX_N + 1];
int board[MAX_L + 1][MAX_L + 1];
int trap[MAX_N + 1];
int width[MAX_N + 1];
int height[MAX_N + 1];
int heart[MAX_N + 1];
pair<int, int> position[MAX_N + 1];
int knight[MAX_L + 1][MAX_L + 1];
int damaged[MAX_N + 1];

bool movable(const int &i, const int &d) {
    // cout << i << endl;
    const int r = position[i].first;
    const int c = position[i].second;

    if(death[i]) { return false; }

    switch(d) {
    case UP:
        if(r == 1) { return false; }
        for(int j = 0; j < width[i]; j++) {
            const int nr = r - 1;
            const int nc = j + c;
            if(board[nr][nc] == WALL) { return false; }
            if(
                knight[nr][nc] &&
                !death[knight[nr][nc]] &&
                !movable(knight[nr][nc], UP)
            ) {
                return false;
            }
        }
        break;
    case RIGHT:
        if(c == L) { return false; }
        for(int j = 0; j < height[i]; j++) {
            const int nr = r + j;
            const int nc = c + width[i];
            if(board[nr][nc] == WALL) { return false; }
            if(
                knight[nr][nc] &&
                !death[knight[nr][nc]] &&
                !movable(knight[nr][nc], RIGHT)
            ) {
                return false;
            }
        }
        break;
    case DOWN:
        if(r == L) { return false; }
        for(int j = 0; j < width[i]; j++) {
            const int nr = r + height[i];
            const int nc = j + c;
            if(board[nr][nc] == WALL) { return false; }
            if(
                knight[nr][nc] &&
                !death[knight[nr][nc]] &&
                !movable(knight[nr][nc], DOWN)
            ) {
                return false;
            }
        }
        break;
    case LEFT:
        if(c == 1) { return false; }
        for(int j = 0; j < height[i]; j++) {
            const int nr = r + j;
            const int nc = c - 1;
            if(board[nr][nc] == WALL) { return false; }
            if(
                knight[nr][nc] &&
                !death[knight[nr][nc]] &&
                !movable(knight[nr][nc], LEFT)
            ) {
                return false;
            }
        }
        break;
    }

    return true;
}


void move_knight(const int &i, const int &d) {
    // cout << i << endl;
    const int r = position[i].first;
    const int c = position[i].second;

    switch(d) {
    case UP:
        for(int j = 0; j < width[i]; j++) {
            const int nr = r - 1;
            const int nc = c + j;
            const int pr = r + height[i] - 1;
            const int pc = nc;
            if(knight[nr][nc] && !death[knight[nr][nc]]) {
                move_knight(knight[nr][nc], UP);
            }
            knight[nr][nc] = knight[pr][pc];
            knight[pr][pc] = 0;
            if(board[pr][pc] == DAMAGE) {
                trap[i]--;
            }
            if(board[nr][nc] == DAMAGE) {
                trap[i]++;
            }
        }
        position[i] = {r - 1, c};
        break;
    case RIGHT:
        for(int j = 0; j < height[i]; j++) {
            const int nr = r + j;
            const int nc = c + width[i];
            const int pr = nr;
            const int pc = c;
            if(knight[nr][nc] && !death[knight[nr][nc]]) {
                move_knight(knight[nr][nc], RIGHT);
            }
            knight[nr][nc] = knight[pr][pc];
            knight[pr][pc] = 0;
            if(board[pr][pc] == DAMAGE) {
                trap[i]--;
            }
            if(board[nr][nc] == DAMAGE) {
                trap[i]++;
            }
        }
        position[i] = {r, c + 1};
        break;
    case DOWN:
        for(int j = 0; j < width[i]; j++) {
            const int nr = r + height[i];
            const int nc = c + j;
            const int pr = r;
            const int pc = nc;
            if(knight[nr][nc] && !death[knight[nr][nc]]) {
                move_knight(knight[nr][nc], DOWN);
            }
            knight[nr][nc] = knight[pr][pc];
            knight[pr][pc] = 0;
            if(board[pr][pc] == DAMAGE) {
                trap[i]--;
            }
            if(board[nr][nc] == DAMAGE) {
                trap[i]++;
            }
        }
        position[i] = {r + 1, c};
        break;
    case LEFT:
        for(int j = 0; j < height[i]; j++) {
            const int nr = r + j;
            const int nc = c - 1;
            const int pr = nr;
            const int pc = c + width[i] - 1;
            if(knight[nr][nc] && !death[knight[nr][nc]]) {
                move_knight(knight[nr][nc], LEFT);
            }
            knight[nr][nc] = knight[pr][pc];
            knight[pr][pc] = 0;
            if(board[pr][pc] == DAMAGE) {
                trap[i]--;
            }
            if(board[nr][nc] == DAMAGE) {
                trap[i]++;
            }
        }
        position[i] = {r, c - 1};
        break;
    }
}

int main() {
    // 여기에 코드를 작성해주세요.
    cin >> L >> N >> Q;

    for(int r = 0; r < L; r++) {
        for(int c = 0; c < L; c++) {
            cin >> board[r][c];
        }
    }

    for(int i = 1; i <= N; i++) {
        cin >> position[i].first >> position[i].second 
            >> height[i] >> width[i] >> heart[i];
        
        for(int r = 0; r < height[i]; r++) {
            for(int c = 0; c < width[i]; c++) {
                if(board[r + position[i].first][c + position[i].second] == DAMAGE) {
                    trap[i]++;
                }
                knight[r + position[i].first][c + position[i].second] = i;
            }
        }
    }

    // for(int r = 1; r <= L; r++) {
    //     for(int c = 1; c <= L; c++) {
    //         cout << knight[r][c] << " ";
    //     }
    //     cout << endl;
    // }

    int i, d;
    for(int t = 0; t < Q; t++) {
        cin >> i >> d;
        if(movable(i, d)) {
            move_knight(i, d);
        }
        for(int n = 1; n <= N; n++) {
            heart[n] -= trap[n];
            damaged[n] += trap[n];
            if(heart[n] <= 0) {
                death[n] = true;
            }
        }
    }
    int total = 0;
    for(int i = 1; i <= N; i++) {
        if(death[i]) { continue; }
        total += damaged[i];
    }

    cout << total << endl;

    return 0;
}