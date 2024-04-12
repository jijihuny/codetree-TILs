#include <iostream>
#include <cmath>

using namespace std;
/**
 * 1. 준비
 * parents 배열
 * 이진트리
 * 
 * authority 몇 번째 부모까지?
 * 
 * 2. on/off
 * 
 * off 자기자신 포함 모든 알림 block
 * 
 * O(D * D)
 * 
 * 3. auth 변경
 * 
 * O(D * D)
 * 
 * 4. 부모 채팅방 변경
 * 같은 depth
 * 
 * O(D * D)
 * 
 * Q <= 100000
 * N <= 100000
 * D <= 20
 * O(QD^2)
*/

const int MAX_N = 100000;
const int MAX_D = 20;
int Q, N;

// 나로부터 부모까지 영향
int count[MAX_N + 1][MAX_D + 1]{};
// 부모
int parents[MAX_N + 1]{};
int authority[MAX_N + 1]{};
bool off[MAX_N + 1]{};

void init() {

    for(int i = 1; i <= N; i++) {
        cin >> parents[i];
    }

    for(int i = 1; i <= N; i++) {
        cin >> authority[i];
        if(authority[i] > MAX_D) authority[i] = MAX_D; 

        for(int d = 1; d <= authority[i]; d++) {
            count[i][d]++;
        }

        int node = parents[i];
        int level = 1;
        while(node != 0) {
            for(int d = 0; d <= authority[i] - level; d++) {
                count[node][d]++;
            }
            node = parents[node];
            level++;
        }
    }
}

void switch_on_off(const int &c) {
    int node = parents[c];
    int level = 1;
    off[c] = !off[c];
    while(node != 0 && !off[node]) {
        for(int d = 0; d < MAX_D + 1 - level; d++) {
            if(off[c]) {
                count[node][d] -= count[c][d + level];
            } else {
                count[node][d] += count[c][d + level];
            }
        }
        node = parents[node];
        level++;
    }
}

void change_authority(const int &c, int power) {
    if(off[c]) { return; }
    
    int prev_power = authority[c];
    power = min(MAX_D, power);
    int node = parents[c];
    int level = 1;    
    if(prev_power < power) {
        for(int d = prev_power + 1; d <= power; d++) {
            count[c][d]++;
        }

        while(level <= power) {
            for(int d = max(prev_power + 1 - level, 0); d <= power - level; d++) {
                count[node][d]++;
            }

            node = parents[node];
            level++;
        }
    } else if(prev_power > power) {
        for(int d = power + 1; d <= prev_power; d++) {
            count[c][d]--;
        }

        while(level <= prev_power) {
            for(int d = max(power + 1 - level, 0); d <= prev_power - level; d++) {
                count[node][d]--;
            }

            node = parents[node];
            level++;
        }
    }

    authority[c] = power;
}

void change_parents(const int &c1, const int &c2) {
    const bool off_c1 = off[c1];
    const bool off_c2 = off[c2];
    if(!off_c1) {
        switch_on_off(c1);
    }
    if(!off_c2) {
        switch_on_off(c2);
    }

    int tmp = parents[c1];
    parents[c1] = parents[c2];
    parents[c2] = tmp;

    if(!off_c1) {
        switch_on_off(c1);
    }
    if(!off_c2) {
        switch_on_off(c2);
    }
}

void display_count(const int &c) {
    cout << count[c][0] << endl;
}

int main() {
    // 여기에 코드를 작성해주세요.
    cin >> N >> Q;
    int code;
    int c, c1, c2, power;
    for(int i = 0; i < Q; i++) {
        cin >> code;

        switch(code) {
        case 100:
            init();
            break;
        case 200:
            cin >> c;
            switch_on_off(c);
            break;
        case 300:
            cin >> c >> power;
            change_authority(c, power);
            break;
        case 400:
            cin >> c1 >> c2;
            change_parents(c1, c2);
            break;
        case 500:
            cin >> c;
            display_count(c);
            break;
        }
        // cout<<code<<endl;
        // cout << code << " " << c << " " << c1 << " " << c2 << " " << power << endl;
        // for(int i = 0; i <= N; ++i) {
        // cout << i << " : "; 
        // for(int j = 0; j < 20; j++) {
        //     cout << count[i][j] << " ";
        // }
        // cout << endl;
        // }
    }
    return 0;
}