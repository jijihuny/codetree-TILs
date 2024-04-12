#include <iostream>
#include <algorithm>
using namespace std;

const int MAX_N = 29;
const int MAX_G = MAX_N * MAX_N;
int N;
int art[MAX_N][MAX_N];
int group[MAX_N][MAX_N];
int tmp[MAX_N][MAX_N];
pair<int, int> info[MAX_G];
int gr_count;
int joint[MAX_G][MAX_G];

int total = 0;
/**
 * 1. 그룹을 만들기
 * O(N*N) dfs
 * visit 배열을 만들어 계산할 수 있다.
 * info[i] = {칸의 수, 숫자값}
 * joint[i][j] 그룹 i, j가 맞닿은 변의 수
*/
int dr[] = {0, -1, 0, 1};
int dc[] = {-1, 0, 1, 0};
void dfs(const int &r, const int &c) {
    for(int i = 0; i < 4; i++) {
        const int nr = r + dr[i];
        const int nc = c + dc[i];

        if(
            nr < 0 || nr >= N ||
            nc < 0 || nc >= N
        ) { continue; }

        
        if(group[nr][nc]) {
            if(group[nr][nc] != group[r][c]) {
                joint[group[r][c]][group[nr][nc]]++;
            }
            continue;
        }

        if(art[nr][nc] != art[r][c]) {
            continue;
        }
        group[nr][nc] = group[r][c];
        info[group[r][c]].first++;
        dfs(nr, nc);
    }
}

void find_group() {
    for(auto row : group) {
        fill(row, row + MAX_N, 0);
    }
    // fill(info, info + gr_count, (pair<int, int>){});
    for(auto row : joint) {
        fill(row, row + MAX_G, 0);
    }

    gr_count = 0;

    for(int r = 0; r < N; r++) {
        for(int c = 0; c < N; c++) {
            if(group[r][c]) { continue; }
            gr_count++;
            info[gr_count] = {1, art[r][c]};
            group[r][c] = gr_count;
            dfs(r, c);
        }
    }
}

/**
 * 2. 조화로움 계산
 * O(N^4)
*/
void cal_harmony() {
    for(int i = 1; i <= gr_count; i++) {
        for(int j = i + 1; j <= gr_count; j++) {
            total += (info[i].first + info[j].first) * info[i].second * info[j].second * (joint[i][j] + joint[j][i]);
        }
    }
}

/**
 * 3. 회전
 * 
 * Given size
 * 
 * i, j => j, size + i - 1
 * 
 * O(N * N) 
*/
int rr[] = {0, 0, 1, 1};
int rc[] = {0, 1, 0, 1};
void rotate() {
    int half = N / 2;
    for(int r = 0; r < N; r++) {
        copy(art[r], art[r] + N, tmp[r]);
    }
    for(int i = 0; i < 4; i++) {
        int diffr = rr[i] * half + (rr[i] ? 1 : 0);
        int diffc = rc[i] * half + (rc[i] ? 1 : 0);

        for(int r = 0; r < half; r++) {
            for(int c = 0; c < half; c++) {
                art[c + diffr][half - r - 1 + diffc] = tmp[r + diffr][c + diffc];
            }
        }
    }

    // i, j => N - i - 1
    for(int c = 0; c < N; c++) {
        art[N - c - 1][half] = tmp[half][c];
    }
    for(int r = 0; r < N; r++) {
        art[N - half - 1][r] = tmp[r][half];
    }
}

int main() {
    // 여기에 코드를 작성해주세요.
    cin >> N;

    for(int r = 0; r < N; r++) {
        for(int c = 0; c < N; c++) {
            cin >> art[r][c];
        }
    }

    // cout << " a";
    find_group();
    cal_harmony();

    for(int i = 0; i < 3; i++) {
        rotate();
        find_group();
        cal_harmony();
    }

    cout << total;
    return 0;
}