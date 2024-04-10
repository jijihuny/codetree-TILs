#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <functional>

using namespace std;

typedef struct {
    int priority;
    int t;
    string domain;
    string url;
    int start;
    int end;
} Task;

struct Cmp {
    bool operator()(const Task a, const Task b) const {
        if(a.priority == b.priority) return a.t < b.t;
        return a.priority > b.priority;
    }
};

struct Cmp compare{};

typedef priority_queue<Task, vector<Task>, Cmp> WaitingQueue;

// waiting queues

// blacklist
map<string, int> bl;

map<string, WaitingQueue> waiting_queues;
priority_queue<int, vector<int>, greater<int>> waiting_judgers;
set<string> waiting_url;

map<int, Task> judging_task;
set<string> judging_domain;

string split_url(const string url) {
    auto sep = url.find_first_of('/');
    return url.substr(0, sep);
} 

void request(const int t, const int p, const string url) {
    if(waiting_url.find(url) != waiting_url.end()) { return; }
    string domain = split_url(url);

    waiting_queues[domain].push({p, t, domain, url});
    waiting_url.insert(url);
}

void fetch(const int t) {
    if(waiting_judgers.size() == 0) { return; }

    Task task = {0, INT32_MAX};

    for(auto dq : waiting_queues) {
        auto domain = dq.first;
        auto que = dq.second;
        if(que.empty()) { continue; }
        if(judging_domain.find(domain) != judging_domain.end()) { continue; }
        if(bl.find(domain) != bl.end() && bl[domain] > t) { continue; }

        if(compare(task, que.top())) { continue; }

         task = que.top();
    }

    if(task.priority == 0) { return; }

    judging_domain.insert(task.domain);
    Task nt = {task.priority, task.t, task.domain, task.url, t, 0};
    int j_id = waiting_judgers.top();
    waiting_judgers.pop();
    judging_task.insert({j_id, nt});
    waiting_url.erase(task.url);
    waiting_queues[task.domain].pop();
}

void terminate(const int t, const int j_id) {
    if(judging_task.empty()) { return; }
    if(judging_task.find(j_id) == judging_task.end()) { return; }
    string domain = judging_task[j_id].domain;
    int t_ = judging_task[j_id].start + (t - judging_task[j_id].start) * 3;
    bl.insert({domain, t_});
    judging_domain.erase(domain);
    judging_task.erase(j_id);
    waiting_judgers.push(j_id);
}

void count(const int t) {
    if(waiting_url.empty()) {
        cout<< 0 << '\n';
        return;
    }
    cout << waiting_url.size() << '\n';
}

int main() {
    int Q;
    int code, t, N, p, j_id;
    string url;
    cin >> Q;

    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    for(int i = 0; i < Q; ++i) {
        cin >> code;
        // cout<<"hello"<<"\n";
        // cout << i << " "<<code<<endl;
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
        // cout<<"done"<<endl;
    }   
}