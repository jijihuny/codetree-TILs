#include <iostream>
#include <map>

using namespace std;

const int MAX_NAME = 15000;
typedef long long bigint;
map<string, int> idx;
int count;
bigint pos[MAX_NAME];
map<bigint, int, greater<bigint>> sushi[MAX_NAME];
int needs[MAX_NAME];
int sushi_count = 0;
int customer_count = 0;
bigint prev_t = 0;
int Q, L;

void eat(const bigint &t) {
    for(auto cit = idx.begin(); cit != idx.end();) {
        if(needs[cit->second] == 0) { cit++; continue; }
        if(sushi[cit->second].empty()) { cit++; continue; }
        if(t - prev_t >= L) {
            for(auto it = sushi[cit->second].begin(); it != sushi[cit->second].end();) {
                needs[cit->second] -= it->second;
                sushi_count -= it->second;
                it = sushi[cit->second].erase(it);
            }
            pos[cit->second] = (pos[cit->second] - (t - prev_t)) % L;
            if(pos[cit->second] < 0) pos[cit->second] += L;
        } else {
            bigint prev_pos = pos[cit->second];
            bigint curr_pos = (pos[cit->second] - (t - prev_t)) % L;
            if(curr_pos < 0) curr_pos += L;

            auto begin = sushi[cit->second].lower_bound(prev_pos);
            auto end = sushi[cit->second].upper_bound(curr_pos);
            
            if(curr_pos <= prev_pos) {
                for(auto it = begin; it != end;) {
                    needs[cit->second] -= it->second;
                    sushi_count -= it->second;
                    it = sushi[cit->second].erase(it);
                }
            } else {
                for(auto it = sushi[cit->second].begin(); it != end;) {
                    needs[cit->second] -= it->second;
                    sushi_count -= it->second;
                    it = sushi[cit->second].erase(it);
                }
                for(auto it = begin; it != sushi[cit->second].end();) {
                    needs[cit->second] -= it->second;
                    sushi_count -= it->second;
                    it = sushi[cit->second].erase(it);
                }
            }

            pos[cit->second] = curr_pos;
        }

        if(needs[cit->second] == 0) {
            customer_count--;
            cit = idx.erase(cit);
        } else {
            cit++;
        }
    }
}

void make_sushi(const bigint &t, const bigint &x, const string &name) {
    if(idx.find(name) == idx.end()) {
        idx[name] = ++count;
    }
    bigint pos = (x - t) % L;
    if(pos < 0) pos += L;
    sushi[idx[name]][pos]++;
    sushi_count++;
    eat(t);
    prev_t = t;
}

void insert(const bigint &t, const bigint &x, const string &name, const int &n) {
    eat(t);
    if(idx.find(name) == idx.end()) {
        idx[name] = ++count;
    }
    pos[idx[name]] = (x - t) % L;
    if(pos[idx[name]] < 0) pos[idx[name]] += L;
    needs[idx[name]] = n;
    customer_count++;

    auto it = sushi[idx[name]].find(pos[idx[name]]);

    if(it != sushi[idx[name]].end()) {
        needs[idx[name]] -= it->second;
        sushi_count -= it->second;
        sushi[idx[name]].erase(it);
        if(needs[idx[name]] == 0) {
            idx.erase(name);
            customer_count--;
        }
    }
    
    prev_t = t;
}

void display(const bigint &t) {
    eat(t);

    cout << customer_count << " " << sushi_count << endl;

    prev_t = t;
}

int main() {
    // 여기에 코드를 작성해주세요.

    cin >> L >> Q;
    int code;
    bigint t, x;
    string name;
    int n;
    for(int i = 0; i < Q; i++) {
        cin >> code;
        switch(code) {
        case 100:
            cin >> t >> x >> name;
            make_sushi(t, x, name);
            break;
        case 200:
            cin >> t >> x >> name >> n;
            insert(t, x, name, n);
            break;
        case 300:
            cin >> t;
            display(t);
            break;
        }
    }
    return 0;
}