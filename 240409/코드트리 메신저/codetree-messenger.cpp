#include <iostream>
#include <vector>

using namespace std;

int N, Q;

const int MAX_NUMBER = 100000;

int parent[MAX_NUMBER + 1];
pair<int, int> childs[MAX_NUMBER + 1];
int authority[MAX_NUMBER + 1];
bool off[MAX_NUMBER + 1];

void togle_on_off(int c) {
    off[c] = !off[c];
}

void change_authority(int c, int power) {
    authority[c] = power;
}

void change_parents(int c1, int c2) {
    int tmp;
    if(c1 == childs[parent[c1]].first) {
        childs[parent[c1]].first = c2;
    } else {
        childs[parent[c1]].second = c2;
    }
    if(c2 == childs[parent[c2]].first) {
        childs[parent[c2]].first = c1;
    } else {
        childs[parent[c2]].second = c1;
    }
    parent[c1];
    parent[c1] = parent[c2];
    parent[c2] = tmp;

}

void dfs(int &count, const int &c) {
    static int depth = 0;
    depth++;

    if(childs[c].first && !off[childs[c].first]) {
        if(depth <= authority[childs[c].first]) {
            count++;
        }
        dfs(count, childs[c].first);
    }
    if(childs[c].second && !off[childs[c].second]) {
        if(depth <= authority[childs[c].second]) {
            count++;
        }
        dfs(count, childs[c].second);
    }

    depth--;
}

int show_count(int c) {
    int count = 0;
    if(off[c]) return 0;
    dfs(count, c);
    return count;
}

int main() {
    // 여기에 코드를 작성해주세요.

    cin >> N >> Q;
    int code, arg1, arg2;
    for(int i = 0; i < Q; i++) {
        cin >> code;

        switch(code) {
            case 100:
                for(int j = 1; j <= N; j++) {
                    cin >> parent[j];
                }
                for(int j = 1; j <= N; j++) {
                    cin >> authority[j];
                }

                for(int j = 1; j <= N; j++) {
                    pair<int, int> &g = childs[parent[j]];

                    if(!g.first) {
                        g.first = j;
                    } else if(!g.second) {
                        g.second = j;
                    }
                }
                break;
            case 200:
                cin >> arg1;
                togle_on_off(arg1);
                break;
            case 300:
                cin >> arg1 >> arg2;
                change_authority(arg1, arg2);
                break;
            case 400:
                cin >> arg1 >> arg2;
                change_parents(arg1, arg2);
                break;
            case 500:
                cin >> arg1;
                cout << show_count(arg1) << endl;
                break;
        }
    }
    return 0;
}