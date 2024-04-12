#include <iostream>
#include <list>
#include <map>

using namespace std;

/**
 * 각 벨트마다 linked list 유지
 * 각 선물에 대한 레퍼런스
 * 
*/
typedef unsigned long long bigint;
typedef struct Node {
    bigint id;
    bigint w;
    Node *next;
    Node *prev;
    int belt;
    bool valid = true;
} Box;

class Belt {
    public:
    Box *head;
    Box *tail;
    bool broken;
    int next;

    Box *pop_front() {
        if(head == 0) {
            return 0;
        }

        Box *ref = this->head;
        if(this->head->next) {
            this->head = this->head->next;
            this->head->prev = 0;
        } else {
            this->head = this->tail = 0;
        }
        ref->next = 0;

        return ref;
    }

    Box *pop_back() {
        if(tail == 0) {
            return 0;
        }

        Box *ref = this->tail;
        if(this->tail->prev) {
            this->tail = this->tail->prev;
            this->tail->next = 0; 
        } else {
            this->head = this->tail = 0;
        }
        ref->prev = 0;

        return ref;
    }

    void push_back(Box *box) {
        if(this->tail) {
            this->tail->next = box;
            box->prev = this->tail;
            this->tail = this->tail->next;
        } else {
            this->head = this->tail = box;
        }
    }
};

const int MAX_N = 100000;
const int MAX_M = 10;
int Q, M, N;
map<bigint, int> idx;
Box box[MAX_N + 1]{};
Belt belt[MAX_M + 1]{};

Belt *find_unbroken(const int &b_num) {
    if(!belt[b_num].broken) {
        return &belt[b_num];
    }
    Belt *ref = &belt[belt[b_num].next];

    while(ref->broken) {
        ref = &belt[ref->next];
    }

    return ref;
}

/**
 * 1. 준비
 * m 벨트
 * n 상자
 * 
 * 각 벨트 위에 n/m개의 상자
 * 
 * 상자 {
 * 번호 id unique
 * 무게 w
 * }
 * 
*/
void init() {
    cin >> N >> M;
    int sz = N / M;
    bigint id, w;
    for(int b = 1; b <= M; b++) {
        belt[b].next = b;
    }
    for(int i = 0; i < N; i++) {
        cin >> id;
        idx[id] = i;
        box[i].id = id;
        box[i].belt = (i / sz) + 1;
        belt[(i / sz) + 1].push_back(&box[i]);
    }
    for(int i = 0; i < N; i++) {
        cin >> w;
        box[i].w = w;
    }
}

/**
 * 2. 하차
 * Given w_max
 * total
 * for  belt of belts
 *    box = belt.pop()
*          if( box.w <= w_max) 
                하차
                total += box.w
*          else belt.push_back(box)
    print(total)


   O(n)
*/
void second(const bigint &w_max) {
    bigint total = 0;
    for(int b = 1; b <= M; b++) {
        Box *ref = belt[b].head;
        if(belt[b].broken) { continue; }
        if(!belt[b].head) { continue; }
        Box *box = belt[b].pop_front();
        if(box->w <= w_max) {
            total += box->w;
            box->valid = false;
        } else {
            belt[b].push_back(box);
        }
    }

    cout << total << endl;
}

/**
 * 3. 제거
 * 
 * Given r_id
 * if(belt exists s.t belt.has(r_id))
 *  belt.erase(r_id)
 *  print(r_id)
 * else 
 *  print(-1)
 * 
 * O(1)
*/
void third(const bigint &r_id) {
    if(idx.find(r_id) != idx.end() && box[idx[r_id]].valid) {
        Box *box_ref = &box[idx[r_id]];
        Belt *belt_ref = find_unbroken(box_ref->belt);
        if(belt_ref->head == box_ref) {
            belt_ref->pop_front();
        } else if(belt_ref->tail == box_ref) {
            belt_ref->pop_back();
        } else {
            Box *prev = box_ref->prev;
            Box *next = box_ref->next;
            prev->next = next;
            next->prev = prev;
        }
        box_ref->valid = false;
        cout << r_id << endl;
    } else {
        cout << -1 << endl;
    }
}

/**
 * 4. 물건 확인
 * Given f_id
 * 
 * if(belt exists s.t belt.has(f_id))
 *  b1 = belt.splice(f_id, end)
 *  belt.push_front(b1)
 * 
 * O(1)
*/
void fourth(const bigint &f_id) {
    if(idx.find(f_id) == idx.end()) {
        cout << -1 << endl;
        return;
    }
    Box *box_ref = &box[idx[f_id]];
    Belt *belt_ref;
    if(box_ref->valid) {
        belt_ref = find_unbroken(box_ref->belt);
        if(box_ref == belt_ref->head) {
            cout << belt_ref->next << endl;
            return;
        }
        Box* head = belt_ref->head;
        Box* tail = belt_ref->tail;
    
        belt_ref->head = box_ref;
        tail->next = head;
        head->prev = tail;

        belt_ref->tail = box_ref->prev;
        box_ref->prev->next = 0;
        box_ref->prev = 0;

        cout << belt_ref->next << endl;
    } else {
        cout << -1 << endl;
    }
}

/**
 * 5. 벨트 고장
 * 
 * Given b_num
 * 오른쪽부터 순서대로 옮겨주기
 * 
 * O(m)
*/
void fifth(const int &b_num) {
    if(b_num > M || belt[b_num].broken) {
        cout << -1 << endl;
        return;
    }
    belt[b_num].broken = true;
    int next;
    for(next = (b_num + 1) % M; next != b_num; next = (next + 1) % M) {
        if(!belt[next].broken) { break; }
    }

    belt[b_num].next = next;

    if(belt[next].tail) {
        belt[next].tail->next = belt[b_num].head;
        if(belt[b_num].head) {
            belt[b_num].head->prev = belt[next].tail;
        }
        belt[next].tail = belt[b_num].tail;
    } else {
        belt[next].head = belt[b_num].head;
        if(belt[b_num].head) {
            belt[next].tail = belt[b_num].tail;
        } else {
            belt[next].tail = 0;
        }
    }

    cout << b_num << endl;
}

int main() {
    // 여기에 코드를 작성해주세요.
    cin >> Q;
    int code;
    bigint input;
    int b_num;
    for(int q = 0; q < Q; q++) {
        cin >> code;

        switch(code) {
        case 100:
            init();
            break;
        case 200:
            cin >> input;
            second(input);
            break;
        case 300:
            cin >> input;
            third(input);
            break;
        case 400:
            cin >> input;
            fourth(input);
            break;
        case 500:
            cin >> b_num;
            fifth(b_num);
            break;
        }
    }
    return 0;
}