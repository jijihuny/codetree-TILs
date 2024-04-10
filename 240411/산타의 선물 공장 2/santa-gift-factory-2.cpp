#include <iostream>

using namespace std;
const int MAX = 100000;

int n, m, q;

typedef struct nn {
    nn* next;
    nn* prev;
    int p_num;
} Node;

typedef struct ll {
    Node *head;
    Node *tail;
    int count;
} LinkedList;

Node present[MAX + 1];
LinkedList belt[MAX + 1];

/**
 * 1. 공장설립
 * 
 * n 벨트, m 물건
 * 
*/
void init() {
    int code;
    cin >> q;
    cin >> code >> n >> m;

    if(code != 100) return;

    int b_num;
    for(int p_num = 1; p_num <= m; p_num++) {
        cin >> b_num;
        present[p_num].p_num = p_num;
        if(belt[b_num].count++ == 0) {
            belt[b_num].head = belt[b_num].tail = &present[p_num];
            continue;
        }
        present[p_num].prev = belt[b_num].tail;
        belt[b_num].tail->next = &present[p_num];
        belt[b_num].tail = &present[p_num];
    }
}

/**
 * 2. 물건 모두 옮기기
 * m_src, m_dst
 * h_src = head(m_src)
 * t_src = tail(m_src)
 * h_dst = head(m_dst)
 * head(m_dst) = h_src
 * prev(h_dst) = t_src
 * next(t_src) = h_dst
 * 
 * O(1)
*/
void move_all(const int &m_src, const int &m_dst) {
    if(belt[m_src].count == 0) { 
        cout << belt[m_dst].count << endl;
        return;
    }

    Node *h_dst = belt[m_dst].head;
    belt[m_dst].head = belt[m_src].head;
    belt[m_src].tail->next = h_dst;
    if(h_dst)
        h_dst->prev = belt[m_src].tail;
    belt[m_src].head = belt[m_src].tail = 0;
    belt[m_dst].count += belt[m_src].count;
    belt[m_src].count = 0;
    cout << belt[m_dst].count << endl;
}


/**
 * 3. 앞 물건만 교체하기
 * m_src, m_dst
 * swap(m_src.head, m_dst.head)
 * show count(m_dst)
 * 
 * h_src = head(m_src)
 * h_dst = head(m_dst)
 * head(m_dst) = h_src
 * head(m_src) = h_dst
 * prev(next(h_src)) = h_dst
 * prev(next(h_dst)) = h_src
 * tmp = next(h_dst)
 * next(h_dst) = next(h_src)
 * next(h_src) = tmp
 * 
 * O(1)
*/
void swap_head(const int &m_src, const int &m_dst) {
    Node *h_src = 0, *h_src_next = 0, *h_dst = 0, *h_dst_next = 0;

    h_src = belt[m_src].head;
    h_dst = belt[m_dst].head;

    if(h_src && h_src->next) {
        h_src_next = h_src->next;
    }
    if(h_dst && h_dst->next) {
        h_dst_next = h_dst->next;
    } 
    
    if(h_src && h_dst) {
        belt[m_src].head = h_dst;
        belt[m_dst].head = h_src;
        if(h_src_next) {
            belt[m_src].head->next = h_src_next;
            h_src_next->prev = belt[m_src].head;
        } else {
            belt[m_src].tail = h_dst;
        }
        if(h_dst_next) {
            belt[m_dst].head->next = h_dst_next;
            h_dst_next->prev = belt[m_dst].head;
        } else {
            belt[m_dst].tail = h_src;
        }
    } else if(!h_src && h_dst) {
        belt[m_src].head = belt[m_src].tail = h_dst;
        belt[m_src].count++;
        belt[m_dst].head = h_dst_next;
        belt[m_dst].count--;
        if(!h_dst_next) {
            belt[m_dst].tail = 0;
        } 
    } else if(h_src && !h_dst) {
        belt[m_dst].head = belt[m_dst].tail = h_src;
        belt[m_dst].count++;
        belt[m_src].head = h_src_next;
        belt[m_src].count--;
        if(!h_src_next) {
            belt[m_src].tail = 0;
        }
    }

    
    cout << belt[m_dst].count << endl;
}

/**
 * 4. 물건 나누기
 * 
 * floor(n/2) where n = count(m_src)
 * move to m_dst if count(m_src) > 1
 * show count(m_dst)
 * O(n * 100)
*/
void divide(const int &m_src, const int &m_dst) {
    int n = belt[m_src].count;
    if(n <= 1) {
        cout << n << endl;
        return;
    }

    n /= 2;
    
    int i = 1;
    Node *head = belt[m_src].head;
    Node *tail = head;
    while(i < n) {
        ++i;
        tail = tail->next;
    }
    belt[m_src].head = tail->next;
    if(belt[m_src].head)
        belt[m_src].head->prev = 0;
    belt[m_src].count -= n;
    tail->next = belt[m_dst].head;
    if(tail->next) {
        tail->next->prev = tail;
    }
    belt[m_dst].head = head;
    belt[m_dst].count += n;
    cout << belt[m_dst].count << endl;
}

/**
 * 5. 선물 정보 얻기
 * 
 * Given p_num
 * a = present(p_num).prev ?? -1
 * b = present(p_num).next ?? -1
 * show a + 2 * b
*/
void get_present_info(const int &p_num) {
    Node *node = &present[p_num];

    int a = -1, b = -1;

    if(node->prev) a = node->prev->p_num;
    if(node->next) b = node->next->p_num;

    cout << a + 2 * b << endl;
}

void get_belt_info(const int &b_num) {
    int a, b, c;

    c = belt[b_num].count;
    if(!c) a = b = -1;
    else {
        a = belt[b_num].head->p_num;
        b = belt[b_num].tail->p_num;
    }

    cout << a + 2 * b + 3 * c << endl;
}

/**
 * 6. 벨트 정보 얻기
 * Given b_num
 * 
 * a = present(b_num).head ?? -1
 * b = present(b_num).tail ?? -1
 * c = count(present(b_num))
 * 
 * show a + 2 * b + 3 * c
*/

int main() {
    // 여기에 코드를 작성해주세요.
    int code, m_src, m_dst, p_num, b_num;
    init();
    for(int i = 1; i < q; i++) {
        cin >> code;
        switch(code) {
            case 200:
                cin >> m_src >> m_dst;
                move_all(m_src, m_dst);
                break;
            case 300:
                cin >> m_src >> m_dst;
                swap_head(m_src, m_dst);
                break;
            case 400:
                cin >> m_src >> m_dst;
                divide(m_src, m_dst);
                break;
            case 500:
                cin >> p_num;
                get_present_info(p_num);
                break;
            case 600:
                cin >> b_num;
                get_belt_info(b_num);
                break;
        }
    }
    return 0;
}