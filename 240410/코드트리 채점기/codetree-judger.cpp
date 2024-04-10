#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <functional>

using namespace std;

typedef long long Id;

typedef struct {
    int priority;
    int t;
    string domain;
    Id id;
    int start;
    int end;
    string url;
} Task;

struct Cmp {
    bool operator()(const Task &a, const Task &b) const {
        if(a.priority == b.priority) return a.t < b.t;
        return a.priority > b.priority;
    }
};

bool compare(const Task &a, const Task &b) {
        if(a.priority == b.priority) return a.t < b.t;
        return a.priority > b.priority;
    }

typedef priority_queue<Task, vector<Task>, Cmp> WaitingQueue;

// waiting queues
map<string, WaitingQueue> wq;
// blacklist
map<string, int> bl;
priority_queue<int, vector<int>, greater<int>> waiting_judgers;
set<string> waiting_url;
map<int, Task> judging;
set<string> judgind_domain;

pair<string, Id> split_url(const string &url) {
    auto sep = url.find_first_of('/');

    return {url.substr(0, sep), stoll(url.substr(sep + 1))};
} 

void request(const int &t, const int &p, const string &url) {
    if(waiting_url.find(url) != waiting_url.end()) { return; }
    const auto s = split_url(url);
    string domain = s.first;
    long long id = s.second;
    wq[domain].push({p, t, domain, id, 0, 0, url});
    waiting_url.insert(url);
}

void fetch(const int &t) {
    if(waiting_judgers.empty()) { return; }

    bool f = false;
    Task tk = {0, (1 << 20)};
    for(auto &domainTask : wq) {
        string domain = domainTask.first;
        auto task = domainTask.second;
        if(bl[domain] > t || judgind_domain.find(domain) != judgind_domain.end()) { continue; }
        if(compare(task.top(), tk)) {
            f = true;
            tk = task.top();
        }
    }
    if(!f) return;
    waiting_url.erase(tk.url);
    int j_id = waiting_judgers.top();
    waiting_judgers.pop();
    wq[tk.domain].pop();
    tk.start = t;
    judging.insert({j_id, tk});
    judgind_domain.insert(tk.domain);
}

void terminate(const int &t, const int &j_id) {
    if(judging.find(j_id) == judging.end()) { return; }

    Task &task = judging[j_id];
    task.end = t;

    bl[task.domain] = task.start + (task.end - task.start) * 3;
    judgind_domain.erase(task.domain);
    judging.erase(j_id);
    waiting_judgers.push(j_id);
}

void count(const int &t) {
    cout << waiting_url.size() << "\n";
}

int main() {
    int Q;
    int code, t, N, p, j_id;
    string url;
    cin >> Q;

    for(int i = 0; i < Q; ++i) {
        cin >> code;

        switch(code) {
            case 100:
                cin >> N >> url;
                for(int n = 1; n <= N; n++) {
                    waiting_judgers.push(n);
                }
                request(0, 1, url);
                break;
            case 200:
                cin >> t >> p >> url;
                request(t, p, url);
                break;
            case 300:
                cin >> t;
                fetch(t);
                break;
            case 400:
                cin >> t >> j_id;
                terminate(t, j_id);
                break;
            case 500:
                cin >> t;
                count(t);
                break;
        }
    }   
}